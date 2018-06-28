
#include <cstdlib>
#include <cmath>
#include <functional>
#include <future>
#include <random>

#include "../pool_bench.hpp"

template<typename Dist, typename Rng, typename It>
inline void
generate_random(Dist& dist, Rng& rng, It begin, It end)
{
    for(It it = begin; it != end; ++it) {
        *it = dist(rng);
    }
}

inline size_t
index(size_t i, size_t j, size_t m)
{
    return i + j * m;
}

struct matrix_multiplication : public pool_bench::suite
{
    size_t _problem_size;
    std::vector<float> _A; 
    std::vector<float> _B; 
    std::vector<float> _C; 
    std::vector<float> _C_answer; 
    float _epsilon;

    matrix_multiplication()
        :_problem_size(1024),
         _A(_problem_size * _problem_size),
         _B(_problem_size * _problem_size),
         _C(_problem_size * _problem_size),
         _C_answer(_problem_size * _problem_size),
         _epsilon(1e-5)
    {}

    size_t
    problem_size() override
    {
        return _problem_size;
    }

    char const*
    name() override
    {
        return "matrix multiplication";
    }

    bool check_result() override
    {
        for(size_t i = 0; i < _C.size(); ++i)
        {
            if(std::abs(_C_answer[i] - _C[i]) >= _epsilon)
                return false;
        }
        return true;
    }
    
    void prepare() override
    {
        size_t m = _problem_size;
        size_t k = _problem_size;
        size_t n = _problem_size;
        for(size_t i = 0; i < m; ++i)
        {
            for(size_t j = 0; j < n; ++j) {
                float sum = 0;
                for(size_t l = 0; l < k; ++l)
                    sum += _A[index(i, l, k)] * _B[index(l, j, n)];
                _C_answer[index(i, j, n)] = sum;
            }
        }

        std::random_device seed;
        std::mt19937 rng;
        std::normal_distribution<float> dist(0, 1);

        generate_random(dist, rng, _A.begin(), _A.end());
        generate_random(dist, rng, _B.begin(), _B.end());
    }

    void teardown() override {}

    std::function<void(size_t)>
    task() override
    {
        return [this](size_t i)
               {
                   size_t k = _problem_size;
                   size_t n = _problem_size;
                   for(size_t j = 0; j < n; ++j) {
                       float sum = 0;
                       for(size_t l = 0; l < k; ++l)
                           sum += _A[index(i, l, k)] * _B[index(l, j, n)];
                       _C[index(i, j, n)] = sum;
                   }
               };
    }
};

REGISTER_BENCHMARK(matrix_multiplication)
