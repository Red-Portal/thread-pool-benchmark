
#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "pool_bench.hpp"

namespace pool_bench
{
    template<typename F, typename Ft>
    inline std::tuple<chrono::nanoseconds, chrono::nanoseconds>
    execute_benchmark(F&& f, Ft&& task, size_t problem_size)
    {
        auto tasks = std::vector<std::future<void>>();
        tasks.reserve(problem_size);

        auto insert_start = chrono::steady_clock::now();
        for(size_t i = 0; i < problem_size; ++i) {
            tasks.emplace_back(f([=]{ task(i); }));
        }
        auto insert_stop = chrono::steady_clock::now();

        auto retrieve_start = chrono::steady_clock::now();
        for(auto& i : tasks)
            i.get();
        auto retrieve_stop = chrono::steady_clock::now();

        auto insert_duration = insert_stop - insert_start;
        auto retrieve_duration = retrieve_stop - retrieve_start;
        return {insert_duration, retrieve_duration};
    }

    void run_benchmarks(std::vector<pool_bench::suite*>& suites,
                        std::vector<pool_bench::runner*>& runners)
    {
        for(auto& i : suites)
        {
            std::cout << "-- preparing " << i->name() << std::endl;
            i->prepare();
            std::cout << "-- preparing " << i->name() << " - done\n"
                      << std::endl;

            size_t problem_size = i->problem_size();
            auto task = i->task();

            for(auto& j : runners){
                j->prepare();

                auto result = pool_bench::execute_benchmark((*j)(), task, problem_size);
                auto first = chrono::duration_cast<chrono::milliseconds>(std::get<0>(result));
                auto second = chrono::duration_cast<chrono::milliseconds>(std::get<1>(result));
                int insertion = first.count();
                int processing = second.count();
                std::cout << j->name()
                          << " insertion: " << insertion << "ms, "
                          << " processing: " << processing << "ms, "
                          << " total: " << insertion + processing << "ms\n";

                if(!i->check_result())
                {
                    using namespace std::string_literals;
                    std::string err = "Error: Incorrect computation result while running \""s
                        + std::string(i->name()) + "\" on \""s + std::string(j->name()) + "\"\n"s;
                    throw std::runtime_error(err);
                }

                j->teardown();
            }
            std::cout << "-- tearing down " << i->name() << std::endl;
            std::cout << "-- tearing down " << i->name() << " - done\n"<< std::endl;
            i->teardown();
            std::cout << std::endl;
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
}
