
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
#include "grand_central_dispatch.hpp"

namespace dispatch
{
    struct grand_central_dispatch : pool_bench::runner
    {
        char const*
        name() override
        { return "Grand Central Dispatch"; }

        void prepare() override
        {}

        void teardown() override
        {}

        std::function<std::future<void>(std::function<void(void)>&&)>
        operator()() override
        {
            return [](std::function<void(void)>&& f)
                   { return dispatch::async(std::move(f)); };
        }
    };
    REGISTER_RUNNER(grand_central_dispatch)
}
