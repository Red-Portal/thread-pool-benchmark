
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <vector>
#include <thread>

namespace sparent_multiqueue
{
    namespace internal{
        using lock_t = std::unique_lock<std::mutex>; 

        class notification_queue {
            std::deque<std::function<void()>> _q;
            bool _done = false;
            std::mutex _mutex;
            std::condition_variable _ready;

        public:
            inline void
            done()
            {
                {
                    std::unique_lock<std::mutex> lock{_mutex};
                    _done = true;
                }
                _ready.notify_all();
            }

            inline bool
            pop(std::function<void()>& x)
            {
                lock_t lock{_mutex};
                while(_q.empty())
                {
                    _ready.wait(lock);
                }
                if(_q.empty())
                    return false;
                x = move(_q.front());
                _q.pop_front();
                return true;
            }

            inline void
            push(std::function<void()>&& f)
            {
                {
                    lock_t lock{_mutex};
                    _q.emplace_back(std::move(f));
                }
                _ready.notify_one();
            }
        };

        class task_system
        {
            unsigned const _count;
            std::vector<std::thread> _threads;
            std::vector<notification_queue> _q;
            std::atomic<unsigned> _index;

            inline void
            run(unsigned i)
            {
                while(true)
                {
                    std::function<void()> f;
                    if(!_q[i].pop(f))
                        break;
                    f();
                }
            }

        public:
            inline task_system()
                : _count(std::thread::hardware_concurrency()),
                  _threads(),
                  _q(),
                  _index(0)
            {
                for(unsigned n = 0; n != _count; ++n)
                    _threads.emplace_back([&, n]{ run(n); });
            }

            inline ~task_system()
            {
                for(auto& e: _q)
                    e.done();
                for(auto& e : _threads)
                    e.join();
            }

            inline void
            push(std::function<void()>&& f){
                auto i = _index++;
                _q[i % _count].push(std::move(f));
            }
        };

        void
        push_queue(std::function<void()>&& f);
    }
    
    template<typename F, typename... Args>
    decltype(auto)
    async(F&& f, Args&&... args)
    {
        using result_type = std::result_of_t<std::decay_t<F>(std::decay_t<Args>...)>;
        using packaged_type = std::packaged_task<result_type()>;

        auto ptr = new packaged_type(std::bind(std::forward<F>(f),
                                               std::forward<Args>(args)...));
        auto result = ptr->get_future();

        internal::push_queue([ptr]
                             {
                                 (*ptr)();
                                 delete ptr;
                             });
        return result;
    }
}

