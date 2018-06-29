
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

#ifndef _POOL_BENCH_TBB_HPP_
#define _POOL_BENCH_TBB_HPP_

#include <functional>
#include <future>
#include <type_traits>
#include <iostream>

#include <tbb/task.h>

namespace pool_bench_tbb
{
    template <typename TASK>
    using operator_return_t = typename std::result_of<TASK()>::type;

    template<typename F, typename... Args>
    decltype(auto)
    async(F&& f, Args&&... args)
    {
        struct LocalTBBTask : public tbb::task
        {
            F func;

            LocalTBBTask(F&& f) : func(std::forward<F>(f))
            {}

            tbb::task* execute() override
            {
                func();
                return nullptr;
            }
        };

        using result_type = std::result_of_t<std::decay_t<F>(std::decay_t<Args>...)>;
        using packaged_type = std::packaged_task<result_type()>;

        auto ptr = new packaged_type(std::bind(std::forward<F>(f),
                                               std::forward<Args>(args)...));
        auto result = ptr->get_future();
        auto* tbbNode = new (tbb::task::allocate_root()) LocalTBBTask(
            [=]()
            {
                (*ptr)();
                delete ptr;
            });

        tbb::task::enqueue(*tbbNode);
        return result;
    }
}

#endif
