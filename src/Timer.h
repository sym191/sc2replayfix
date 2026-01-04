

#ifndef TIMER_H
#define TIMER_H

#include <queue>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <atomic>
#include <future>
#include <memory>

class Timer {
public:
    static Timer& instance()
    {
        static Timer timer;
        return timer;
    }

    Timer(): running_(true), worker_(&Timer::run, this){}

    template <typename F>
    auto schedule(const std::chrono::milliseconds delay, F&& f) -> std::future<std::invoke_result_t<F>>
    {
        using Ret = std::invoke_result_t<F>;
        auto task = std::make_shared<std::packaged_task<Ret()>>(std::forward<F>(f));
        auto future = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!running_) throw std::runtime_error("timer stopped");
            task_.push(TimerTask(delay + std::chrono::steady_clock::now(), [task]{(*task)();}));
        }

        cv_.notify_one();

        return future;
    }

    void stop()
    {
        running_ = false;
        cv_.notify_all();
        if (worker_.joinable())
        {
            worker_.join();
        }
    }

    ~Timer()
    {
        stop();
    }

private:
    void run()
    {
        while (running_ || !task_.empty())
        {
            std::unique_lock lock(mutex_);
            if (task_.empty())
            {
                cv_.wait(lock, [&]
                {
                    return !running_ || !task_.empty();
                });
                continue; // 防止虚假唤醒
            }

            auto next_time = task_.top().when;
            if (cv_.wait_until(lock, next_time, [&]{return !running_ || task_.top().when < next_time;}))
            {
                continue;
            }

            if (!running_) break;
            if (std::chrono::steady_clock::now() >= next_time)
            {
                auto task = task_.top().task; // task.top(): const T&,只能copy,move无意义。
                task_.pop();
                lock.unlock();
                task();
            }
        }
    }


private:
    struct TimerTask
    {
        std::chrono::steady_clock::time_point when;
        std::function<void()> task;

        bool operator > (const TimerTask& other) const
        {
            return when > other.when;
        }
    };

    std::priority_queue<
        TimerTask,
        std::vector<TimerTask>,
        std::greater<>
    >task_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_;
    std::thread worker_; // 任务比较少，暂时只用单线程。

};



#endif //TIMER_H
