

/* 
 * thread-pool-benchmark, a C++ Thread Pool Colosseum
 * Copyright (C) 2018  Red-Portal
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

#include <cstdlib>

#include <pool_bench.hpp>
#include "ThreadPool/ThreadPool.h"

namespace progschj
{
    struct progschj_ThreadPool : pool_bench::runner
    {
        std::unique_ptr<ThreadPool> _pool;

        char const*
        name() override
        { return "progschj's ThreadPool"; }

        void prepare() override
        {
            _pool = std::make_unique<ThreadPool>(4);
        }

        void teardown() override
        {
            _pool = nullptr;
        }

        std::function<std::future<void>(std::function<void(void)>&&)>
        operator()() override
        {
            return [this](std::function<void(void)>&& f)
                   { return _pool->enqueue(std::move(f)); };
        }
    };
    REGISTER_RUNNER(progschj_ThreadPool)
}
