
#include "sean_parent_naive.hpp"

namespace sparent_naive {
    namespace internal  {
        task_system sys;

        void push_queue(std::function<void()>&& f)
        {
            sys.push(std::move(f));
        }
    }
}
