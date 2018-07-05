
/* 
 * Source code from 'Incremental-Fluids'
 * link: https://github.com/tunabrain/incremental-fluids/tree/master/2-better-advection
 */

/*
  Copyright (c) 2013 Benedikt Bitterli

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
  distribution.
*/

#include <algorithm>
#include <vector>
#include <cmath>

#include <pool_bench.hpp>

/* Length of vector (x, y) */
double length(double x, double y) {
    return sqrt(x*x + y*y);
}

/* Cubic pulse function.
 * Returns a value in range [0, 1].
 * Return value is 0 for x <= -1 and x >= 1; value is 1 for x=0
 * Smoothly interpolates between 0 and 1 between these three points.
 */
double cubicPulse(double x) {
    x = std::min(fabs(x), 1.0);
    return 1.0 - x*x*(3.0 - 2.0*x);
}


struct FluidQuantity {
    std::vector<double> _src;
    std::vector<double> _dst;

    int _w;
    int _h;
    double _ox;
    double _oy;
    double _hx;

    FluidQuantity(int w, int h, double ox, double oy, double hx)
        :_src(w * h, 0.0), _dst(w * h, 0.0),
         _w(w), _h(h), _ox(ox), _oy(oy), _hx(hx)
    {}
    
    double lerp(double a, double b, double x) const {
        return a*(1.0 - x) + b*x;
    }
    
    /* Cubic intERPolate using samples a through d for x ranging from 0 to 1.
     * A Catmull-Rom spline is used. Over- and undershoots are clamped to
     * prevent blow-up.
     */
    double cerp(double a, double b, double c, double d, double x) const {
        double xsq = x*x;
        double xcu = xsq*x;
        
        double minV = std::min(a, std::min(b, std::min(c, d)));
        double maxV = std::max(a, std::max(b, std::max(c, d)));

        double t =
            a*(0.0 - 0.5*x + 1.0*xsq - 0.5*xcu) +
            b*(1.0 + 0.0*x - 2.5*xsq + 1.5*xcu) +
            c*(0.0 + 0.5*x + 2.0*xsq - 1.5*xcu) +
            d*(0.0 + 0.0*x - 0.5*xsq + 0.5*xcu);
        
        return std::min(std::max(t, minV), maxV);
    }
    
    /* Third order Runge-Kutta for velocity integration in time */
    void rungeKutta3(double &x, double &y, double timestep, const FluidQuantity &u, const FluidQuantity &v) const {
        double firstU = u.lerp(x, y)/_hx;
        double firstV = v.lerp(x, y)/_hx;

        double midX = x - 0.5*timestep*firstU;
        double midY = y - 0.5*timestep*firstV;

        double midU = u.lerp(midX, midY)/_hx;
        double midV = v.lerp(midX, midY)/_hx;

        double lastX = x - 0.75*timestep*midU;
        double lastY = y - 0.75*timestep*midV;

        double lastU = u.lerp(lastX, lastY);
        double lastV = v.lerp(lastX, lastY);
        
        x -= timestep*((2.0/9.0)*firstU + (3.0/9.0)*midU + (4.0/9.0)*lastU);
        y -= timestep*((2.0/9.0)*firstV + (3.0/9.0)*midV + (4.0/9.0)*lastV);
    }
    
    void flip() {
        swap(_src, _dst);
    }
    
    const std::vector<double>& src() const {
        return _src;
    }
    
    double at(int x, int y) const {
        return _src[x + y*_w];
    }
    
    double& at(int x, int y) {
        return _src[x + y*_w];
    }
    
    double lerp(double x, double y) const {
        x = std::min(std::max(x - _ox, 0.0), _w - 1.001);
        y = std::min(std::max(y - _oy, 0.0), _h - 1.001);
        int ix = (int)x;
        int iy = (int)y;
        x -= ix;
        y -= iy;
        
        double x00 = at(ix + 0, iy + 0), x10 = at(ix + 1, iy + 0);
        double x01 = at(ix + 0, iy + 1), x11 = at(ix + 1, iy + 1);
        
        return lerp(lerp(x00, x10, x), lerp(x01, x11, x), y);
    }
    
    /* Cubic intERPolate on grid at coordinates (x, y).
     * Coordinates will be clamped to lie in simulation domain
     */
    double cerp(double x, double y) const {
        x = std::min(std::max(x - _ox, 0.0), _w - 1.001);
        y = std::min(std::max(y - _oy, 0.0), _h - 1.001);
        int ix = (int)x;
        int iy = (int)y;
        x -= ix;
        y -= iy;
        
        int x0 = std::max(ix - 1, 0), x1 = ix, x2 = ix + 1, x3 = std::min(ix + 2, _w - 1);
        int y0 = std::max(iy - 1, 0), y1 = iy, y2 = iy + 1, y3 = std::min(iy + 2, _h - 1);
        
        double q0 = cerp(at(x0, y0), at(x1, y0), at(x2, y0), at(x3, y0), x);
        double q1 = cerp(at(x0, y1), at(x1, y1), at(x2, y1), at(x3, y1), x);
        double q2 = cerp(at(x0, y2), at(x1, y2), at(x2, y2), at(x3, y2), x);
        double q3 = cerp(at(x0, y3), at(x1, y3), at(x2, y3), at(x3, y3), x);
        
        return cerp(q0, q1, q2, q3, y);
    }
    
    /* Advect grid in velocity field u, v with given timestep */
    void advect(double timestep, const FluidQuantity &u, const FluidQuantity &v) {
        for (int iy = 0, idx = 0; iy < _h; iy++) {
            
            for (int ix = 0; ix < _w; ix++, idx++) {
                double x = ix + _ox;
                double y = iy + _oy;
                
                /* First component: Integrate in time */
                rungeKutta3(x, y, timestep, u, v);
                
                /* Second component: Interpolate from grid */
                _dst[idx] = cerp(x, y);
            }
        }
    }
    
    /* Set fluid quantity inside the given rect to the specified value, but use
     * a smooth falloff to avoid oscillations
     */
    void addInflow(double x0, double y0, double x1, double y1, double v) {
        int ix0 = (int)(x0/_hx - _ox);
        int iy0 = (int)(y0/_hx - _oy);
        int ix1 = (int)(x1/_hx - _ox);
        int iy1 = (int)(y1/_hx - _oy);
        
        for (int y = std::max(iy0, 0); y < std::min(iy1, _h); y++) {
            for (int x = std::max(ix0, 0); x < std::min(ix1, _h); x++) {
                double l = length(
                    (2.0*(x + 0.5)*_hx - (x0 + x1))/(x1 - x0),
                    (2.0*(y + 0.5)*_hx - (y0 + y1))/(y1 - y0)
                    );
                double vi = cubicPulse(l)*v;
                if (fabs(_src[x + y*_w]) < fabs(vi))
                    _src[x + y*_w] = vi;
            }
        }
    }
};

struct FluidSolver {
    double _hx;
    double _density;
    
    FluidQuantity _d;
    FluidQuantity _u;
    FluidQuantity _v;
    
    int _w;
    int _h;
    
    std::vector<double> _r;
    std::vector<double> _p;

    FluidSolver(int w, int h, double density)
        :_hx(1.0/std::min(w, h)),
         _density(density),
         _d(w, h, 0.5, 0.5, _hx),
         _u(w + 1, h, 0.0, 0.5, _hx),
         _v(w, h + 1, 0.5, 0.0, _hx),
         _w(w),
         _h(h),
         _r(w * h, 0),
         _p(w * h, 0)
    {}
    
    void buildRhs() {
        double scale = 1.0/_hx;
        
        for (int y = 0, idx = 0; y < _h; y++) {
            for (int x = 0; x < _w; x++, idx++) {
                _r[idx] = -scale*(_u.at(x + 1, y) - _u.at(x, y) +
                                  _v.at(x, y + 1) - _v.at(x, y));
            }
        }
    }
    
    void project(int limit, double timestep) {
        double scale = timestep/(_density*_hx*_hx);
        
        double maxDelta;
        for (int iter = 0; iter < limit; iter++) {
            maxDelta = 0.0;
            for (int y = 0, idx = 0; y < _h; y++) {
                for (int x = 0; x < _w; x++, idx++) {
                    int idx = x + y*_w;
                    
                    double diag = 0.0, offDiag = 0.0;
                    
                    if (x > 0) {
                        diag    += scale;
                        offDiag -= scale*_p[idx - 1];
                    }
                    if (y > 0) {
                        diag    += scale;
                        offDiag -= scale*_p[idx - _w];
                    }
                    if (x < _w - 1) {
                        diag    += scale;
                        offDiag -= scale*_p[idx + 1];
                    }
                    if (y < _h - 1) {
                        diag    += scale;
                        offDiag -= scale*_p[idx + _w];
                    }

                    double newP = (_r[idx] - offDiag)/diag;
                    
                    maxDelta = std::max(maxDelta, fabs(_p[idx] - newP));
                    
                    _p[idx] = newP;
                }
            }

            if (maxDelta < 1e-5) {
                return;
            }
        }
    }
    
    void applyPressure(double timestep) {
        double scale = timestep/(_density*_hx);
        
        for (int y = 0, idx = 0; y < _h; y++) {
            for (int x = 0; x < _w; x++, idx++) {
                _u.at(x,     y    ) -= scale*_p[idx];
                _u.at(x + 1, y    ) += scale*_p[idx];
                _v.at(x,     y    ) -= scale*_p[idx];
                _v.at(x,     y + 1) += scale*_p[idx];
            }
        }
        
        for (int y = 0; y < _h; y++)
            _u.at(0, y) = _u.at(_w, y) = 0.0;
        for (int x = 0; x < _w; x++)
            _v.at(x, 0) = _v.at(x, _h) = 0.0;
    }
    
    void addInflow(double x, double y,
                   double w, double h,
                   double d, double u, double v) {
        _d.addInflow(x, y, x + w, y + h, d);
        _u.addInflow(x, y, x + w, y + h, u);
        _v.addInflow(x, y, x + w, y + h, v);
    }

    std::pair<size_t, size_t>
    dimensions() const {
        return {_w, _h};
    }

    double operator[](size_t i) const {
        return _d.src()[i];
    }
};

bool is_equal(FluidSolver const& a,
              FluidSolver const& b,
              double epsilon)
{
    auto shape = a.dimensions();
    for (size_t i = 0; i < shape.first * shape.second; i++) {
        if(std::abs(a[i] - b[i]) > epsilon)
            return false;
    }
    return true;
}

struct fluid_solver : public pool_bench::suite
{
    size_t _grid_size;
    size_t _problem_size;
    FluidSolver _solver;
    FluidSolver _solver_blank;
    FluidSolver _solver_answer;
    double _epsilon;
    double _timestep;

    fluid_solver()
        :_grid_size(2048),
         _problem_size(_grid_size * 3 * 4),
        _solver_answer(_grid_size, _grid_size, 0.1),
        _solver_blank(_grid_size, _grid_size, 0.1), 
        _solver(_grid_size, _grid_size, 0.1), 
        _epsilon(1e-4),
        _timestep(0.05)
    {}

    size_t
    problem_size() override
    {
        return _problem_size;
    }

    char const*
    name() override
    {
        return "Fluid Solver";
    }

    bool check_result() override
    {
        return is_equal(_solver, _solver_answer, _epsilon);
    }
    
    void prepare() override
    {
        _solver_blank = _solver_answer;

        for(size_t i = 0; i < 4; ++i)
        {
            _solver_answer.buildRhs();
            _solver_answer.project(1000, _timestep);
            _solver_answer.applyPressure(_timestep);

            _solver_answer._d.advect(_timestep, _solver_answer._u, _solver_answer._v);
            _solver_answer._u.advect(_timestep, _solver_answer._u, _solver_answer._v);
            _solver_answer._v.advect(_timestep, _solver_answer._u, _solver_answer._v);

            _solver_answer._d.flip();
            _solver_answer._u.flip();
            _solver_answer._v.flip();
        }
    }

    void teardown() override {}

    void
    run(std::function<
        std::future<void>(std::function<void(void)>&&)>&& async) override
    {
        _solver = _solver_blank;

        for(size_t i = 0; i < 4; ++i)
        {
            auto tasks = std::vector<std::future<void>>();
            tasks.reserve(_grid_size);

            _solver.buildRhs();
            _solver.project(1000, _timestep);
            _solver.applyPressure(_timestep);

            for(size_t j = 0; j < _grid_size; ++j)
            {
                auto d_advect =
                    [this, j]
                    {
                        size_t idx = 0;
                        size_t iy = j;
                        for (int ix = 0; ix < _solver._w; ix++, idx++) {
                            double x = ix + _solver._d._ox;
                            double y = iy + _solver._d._oy;
                
                            _solver._d.rungeKutta3(x, y,
                                                   _timestep,
                                                   _solver._u,
                                                   _solver._v);
                
                            _solver._d._dst[idx] = _solver._d.cerp(x, y);
                        }
                    };
                
                tasks.emplace_back(async(d_advect));
            }

            for(size_t j = 0; j < _grid_size; ++j)
            {
                auto u_advect =
                    [this, j]
                    {
                        size_t idx = 0;
                        size_t iy = j;
                        for (int ix = 0; ix < _solver._w; ix++, idx++) {
                            double x = ix + _solver._u._ox;
                            double y = iy + _solver._u._oy;
                
                            _solver._u.rungeKutta3(x, y,
                                                   _timestep,
                                                   _solver._u,
                                                   _solver._v);
                
                            _solver._u._dst[idx] = _solver._u.cerp(x, y);
                        }
                    };

                tasks.emplace_back(async(u_advect));
            }

            for(size_t j = 0; j < _grid_size; ++j)
            {
                auto v_advect =
                    [this, j]
                    {
                        size_t idx = 0;
                        size_t iy = j;
                        for (int ix = 0; ix < _solver._w; ix++, idx++) {
                            double x = ix + _solver._v._ox;
                            double y = iy + _solver._v._oy;
                
                            _solver._v.rungeKutta3(x, y,
                                                   _timestep,
                                                   _solver._u,
                                                   _solver._v);
                
                            _solver._v._dst[idx] = _solver._v.cerp(x, y);
                        }
                    };

                tasks.emplace_back(async(v_advect));
            }

            for(auto& i : tasks)
                i.get();

            _solver._u.flip();
            _solver._d.flip();
            _solver._v.flip();
        }
    }
};

REGISTER_BENCHMARK(fluid_solver)
