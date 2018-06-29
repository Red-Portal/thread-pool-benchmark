
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

#include <pool_bench.hpp>

#include "sean_parent_naive.hpp"
#include "sean_parent_multiqueue.hpp"
#include "sean_parent_worksteal.hpp"

namespace sparent_naive
{
    namespace internal 
    { 
        std::unique_ptr<task_system> sys;

        void push_queue(std::function<void()>&& f)
        {
            sys->push(std::move(f));
        }
    }

    struct sparent_naive_runner : pool_bench::runner
    {
        sparent_naive_runner()
            : pool_bench::runner()
        {}

        char const*
        name() override
        { return "Sean Parent Naive"; }

        void prepare() override
        { internal::sys = std::make_unique<internal::task_system>(); }

        void teardown() override
        { internal::sys = nullptr; }

        std::function<std::future<void>(std::function<void(void)>)>
        operator()() override
        {
            return [](std::function<void(void)>&& f)
                   { return sparent_naive::async(f); };
        }
    };
    REGISTER_RUNNER(sparent_naive_runner)
}


namespace sparent_multiqueue
{
    namespace internal 
    { 
        std::unique_ptr<task_system> sys;

        void push_queue(std::function<void()>&& f)
        {
            sys->push(std::move(f));
        }
    }

    struct sean_parent_multiqueue : pool_bench::runner
    {
        char const*
        name() override
        { return "Sean Parent Multiqueue"; }

        void prepare() override
        { internal::sys = std::make_unique<internal::task_system>(); }

        void teardown() override
        { internal::sys = nullptr; }

        std::function<std::future<void>(std::function<void(void)>)>
        operator()() override
        {
            return [](std::function<void(void)>&& f)
                   { return sparent_multiqueue::async(f); };
        }
    };
    REGISTER_RUNNER(sean_parent_multiqueue)
}

namespace sparent_worksteal
{
    namespace internal 
    { 
        std::unique_ptr<task_system> sys;

        void push_queue(std::function<void()>&& f)
        {
            sys->push(std::move(f));
        }
    }

    struct sean_parent_worksteal : pool_bench::runner
    {
        char const*
        name() override
        { return "Sean Parent Work Stealing Multiqueue"; }

        void prepare() override
        { internal::sys = std::make_unique<internal::task_system>(); }

        void teardown() override
        { internal::sys = nullptr; }


        std::function<std::future<void>(std::function<void(void)>)>
        operator()() override
        {
            return [](std::function<void(void)>&& f)
                   { return sparent_worksteal::async(f); };
        }
    };
    REGISTER_RUNNER(sean_parent_worksteal)
}
