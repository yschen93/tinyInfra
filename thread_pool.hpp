#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <type_traits>
#include <chrono>

class ThreadPool
{
public:
    using task_type = std::function<void()>;
    ThreadPool(size_t thread_num);
    ~ThreadPool();

    //submit task
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args)
        ->std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto wrapper = std::bind(std::forward<F>(f), std::forward<Args>(args)...);  //return_type wrapper();
        auto wrapper_1 = std::make_shared<std::packaged_task<return_type()>>(wrapper);
        auto wrapper_2 = [wrapper_1](){
            (*wrapper_1)();
        };

        {
            std::unique_lock<std::mutex> lk(tasks_mtx_);
            tasks_.emplace(std::move(wrapper_2));
            cv_.notify_one();
        }

        return wrapper_1->get_future();
    }

    //delete Copy constructor && Copy assignment
    ThreadPool(const ThreadPool& rhs) = delete;
    ThreadPool& operator= (const ThreadPool& rhs) = delete;
private:
    std::queue<task_type> tasks_;
    std::vector<std::thread> threads_;
    std::mutex tasks_mtx_;
    std::atomic_bool is_running_;
    std::condition_variable cv_;
};

ThreadPool::ThreadPool(size_t thread_num)
: is_running_(true)
{
    for (size_t index = 0; index < thread_num; ++index)
    {
        threads_.emplace_back([this, index](){
            while (is_running_)
            {
                std::unique_lock<std::mutex> lk(tasks_mtx_);
                cv_.wait(lk, [this](){
                    return (!is_running_ || !tasks_.empty());
                });

                if (!tasks_.empty())
                {
                    auto task = tasks_.front();
                    std::cout << "Starting running task in thread [" << index << "]" << std::endl;
                    task();
                    std::cout << "Ending running task in thread [" << index << "]" << std::endl;
                    tasks_.pop();
                }
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    is_running_.store(false);
    cv_.notify_all();

    for (auto& thread : threads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
