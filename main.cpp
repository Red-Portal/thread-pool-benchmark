
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

#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <tuple>

#include "pool_bench.hpp"

#include <boost/spirit/include/karma.hpp>

namespace pool_bench
{
    using namespace std::string_literals;

    template<class...Durations, class DurationIn>
    std::tuple<Durations...>
    break_down_durations( DurationIn d )
    /* 
     * taken from https://stackoverflow.com/questions/42138599/how-to-format-stdchrono-durations
     * copyright: Adam Nevraumont
     */
    {
        std::tuple<Durations...> retval;
        using discard=int[];
        (void)discard{0,(
                void(((std::get<Durations>(retval) =
                       std::chrono::duration_cast<Durations>(d)),
                      (d -= std::chrono::duration_cast<DurationIn>(
                          std::get<Durations>(retval))))),0)...};
        return retval;
    }

    template<typename F>
    inline std::tuple<chrono::nanoseconds, chrono::nanoseconds>
    execute_benchmark(F&& f, pool_bench::suite& task)
    {
        auto insertion = std::vector<chrono::nanoseconds>();
        insertion.reserve(task.problem_size());

        auto async_call = [&f, &insertion](std::function<void(void)>&& task)
                          {
                              auto insert_start = chrono::steady_clock::now();
                              auto future = f(std::move(task));
                              auto insert_stop = chrono::steady_clock::now();
                              insertion.emplace_back(insert_stop - insert_start);
                              return future;
                          };

        auto span_start = chrono::steady_clock::now();
        task.run(std::move(async_call));
        auto span_stop = chrono::steady_clock::now();

        auto insert_duration = std::accumulate(insertion.begin(),
                                               insertion.end(),
                                               chrono::nanoseconds());
        auto span_duration = span_stop - span_start;
        return {insert_duration, span_duration - insert_duration};
    }

    template<typename Duration>
    inline double
    format_time(Duration&& duration)
    {
        using float_millisec = chrono::duration<double, std::milli>;
        return chrono::duration_cast<float_millisec>(duration).count();
    }

    inline std::pair<std::string, std::string>
    make_output_format(std::vector<pool_bench::runner*> const& runners)
    {
        size_t max_len = 0;
        for(auto& i : runners)
            max_len = std::max(max_len, std::string(i->name()).size());
        auto report_format = "%-"s + std::to_string(max_len) + "s  %10fms  %10fms  %10fms\n";
        auto header_format = "%-"s + std::to_string(max_len) + "s  %10s  %10s  %10s\n"s;
        return {header_format, report_format};
    }

    void run_benchmarks(std::vector<pool_bench::suite*>& suites,
                        std::vector<pool_bench::runner*>& runners)
    {
        auto output = make_output_format(runners);
        auto header_format = std::get<0>(output);
        auto report_format = std::get<1>(output);

        for(auto& i : suites)
        {
            std::cout << "-- preparing " << i->name() << std::endl;
            i->prepare();
            std::cout << "-- preparing " << i->name() << " - done\n"
                      << std::endl;

            printf(header_format.c_str(),
                   "< gladiatior >",
                   " < forking >",
                   " < joining >",
                   " < total >");

            for(auto& j : runners){
                j->prepare();

                auto result = pool_bench::execute_benchmark((*j)(), *i);
                auto fork_duration = std::get<0>(result);
                auto join_duration = std::get<1>(result);
                auto total_duration = fork_duration + join_duration;

                //printf("%s %10fms %10fms %10fms\n",
                printf(report_format.c_str(),
                       j->name(),
                       format_time(fork_duration),
                       format_time(join_duration),
                       format_time(total_duration));

                if(!i->check_result())
                {
                    std::string err = "Error: Incorrect computation result while running \""s
                        + std::string(i->name()) + "\" on \""s + std::string(j->name()) + "\"\n"s;
                    throw std::runtime_error(err);
                }
                j->teardown();
            }
            std::cout << "\n-- tearing down " << i->name() << std::endl;
            i->teardown();
            std::cout << "-- tearing down " << i->name() << " - done" << std::endl;
        }
    }
}

int main()
{
    std::cout << "***  thread pool benchmark  ***" << std::endl;
    std::cout << "-- Found " << pool_bench::get_runners().size() 
              << " registered subjects"
              << std::endl;
    std::cout << "-- Found " << pool_bench::get_suites().size()
              << " registered benchmark problems"
              << std::endl;
    pool_bench::run_benchmarks(pool_bench::get_suites(), pool_bench::get_runners());
    return 0;
}
