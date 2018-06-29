
/* 
 * thread-pool-benchmark, a C++ Thread Pool Colosseum
 * Copyright (C) 2018 Red-Portal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _POOL_BENCH_POOL_BENCH_
#define _POOL_BENCH_POOL_BENCH_

#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <memory>
#include <vector>

namespace pool_bench
{
    namespace chrono = std::chrono;

    struct runner;
    struct suite;

    inline std::vector<pool_bench::suite*>&
    get_suites()
    {
        static std::vector<pool_bench::suite*> _suites;
        return _suites;
    }

    inline std::vector<pool_bench::runner*>&
    get_runners()
    {
        static std::vector<pool_bench::runner*> runners;
        return runners;
    }

    struct runner
    {
        inline runner()
        {
            get_runners().emplace_back(this);
        }

        virtual char const* name() = 0;
        virtual void prepare() = 0;
        virtual void teardown() = 0;

        virtual std::function<std::future<void>(std::function<void(void)>)>
        operator()() = 0;
    };

    struct suite
    {
        inline suite()
        {
            get_suites().emplace_back(this);
        }

        virtual size_t problem_size() = 0;
        virtual std::function<void(size_t)> task() = 0;
        virtual char const* name () = 0;
        virtual bool check_result () = 0;
        virtual void prepare () = 0;
        virtual void teardown() = 0;
        virtual ~suite() = default;
    };
}

#define REGISTER_BENCHMARK(NAME) static NAME _##NAME{};
#define REGISTER_RUNNER(NAME) static NAME _##NAME{};

#endif
