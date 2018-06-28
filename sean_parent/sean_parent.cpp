
#include "../pool_bench.hpp"

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
        {
            return "Sean Parent Naive Implementation";
        
        }

        void prepare() override
        {
            internal::sys = std::make_unique<internal::task_system>();
        }

        void teardown() override
        {
            internal::sys = nullptr;
        }

        std::function<std::future<void>(std::function<void(void)>)>
        operator()() override
        {
            return [](std::function<void(void)>&& f)
                   { return sparent_naive::async(f); };
        }
    };

    REGISTER_RUNNER(sparent_naive_runner)
}


struct sean_parent_multiqueue : pool_bench::runner
{
    char const*
    name() override
    {
        return "Sean Parent with Multiqueue";
    }

    std::function<std::future<void>(std::function<void(void)>)>
    operator()() override
    {
        return [](std::function<void(void)>&& f)
               { return sparent_multiqueue::async(f); };
    }
};
//REGISTER_RUNNER(sean_parent_multiqueue)

struct sean_parent_worksteal : pool_bench::runner
{
    char const*
    name() override
    {
        return "Sean Parent With Multiqueue and Work Stealing";
        
    }

    std::function<std::future<void>(std::function<void(void)>)>
    operator()() override
    {
        return [](std::function<void(void)>&& f)
               { return sparent_worksteal::async(f); };
    }
};
//REGISTER_RUNNER(sean_parent_worksteal)
