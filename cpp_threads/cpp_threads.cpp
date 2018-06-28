
#include <cstdlib>
#include <future>
#include <functional>

#include "../pool_bench.hpp"

struct cpp_threads : pool_bench::runner
{
    cpp_threads()
        : pool_bench::runner()
    {}

    char const*
    name() override
    {
        return "C++ thread";
    }

    void prepare() override {}
    void teardown() override {}

    std::function<std::future<void>(std::function<void(void)>)>
    operator()() override
    {
        return [](std::function<void(void)>&& f)
               { return std::async(std::launch::async, f); };
    }
};

REGISTER_RUNNER(cpp_threads)
