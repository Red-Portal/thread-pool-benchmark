
#include "sean_parent_multiqueue.hpp"

namespace sparent_multiqueue {
    namespace internal  {
        task_system sys;

        void push_queue(std::function<void()>&& f)
        {
            sys.push(std::move(f));
        }
    }
}
