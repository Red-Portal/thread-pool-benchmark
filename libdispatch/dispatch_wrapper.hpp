
#include <functional>
#include <future>
#include <type_traits>

#include <dispatch/dispatch.h>

namespace dispatch
{
    template<typename F, typename... Args>
    decltype(auto)
    async(F&& f, Args&&... args)
    {
        using result_type = std::result_of_t<std::decay_t<F>(std::decay_t<Args>...)>;
        using packaged_type = std::packaged_task<result_type()>;

        auto ptr = new packaged_type(std::bind(std::forward<F>(f),
                                               std::forward<Args>(args)...));
        auto result = ptr->get_future();

        dispatch_async_f(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                         ptr,
                         [](void* f){
                             packaged_type* f_ = static_cast<packaged_type*>(f);
                             (*f_)();
                             delete f_;
                         });
        return result;
    }
}
