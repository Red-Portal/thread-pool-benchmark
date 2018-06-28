
#include "sean_parent_worksteal.hpp"

namespace sparent_worksteal {
    namespace internal  {
        task_system sys;

        void push_queue(std::function<void()>&& f)
        {
            sys.push(std::move(f));
        }
    }
}
