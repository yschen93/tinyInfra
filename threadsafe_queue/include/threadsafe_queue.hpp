#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace tinyInfra {

template<typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() = default;

    threadsafe_queue(const threadsafe_queue& rhs)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        data_queue_ = rhs.data_queue_;
    }

    void push(const T& new_value)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        data_queue_.emplace(new_value);
        cond_.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        cond_.wait(lk, [this](){ return !data_queue_.empty(); });
        value = data_queue_.front();
        data_queue_.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        cond_.wait(lk, [this](){ return !data_queue_.empty(); });
        auto rtn = std::shared_ptr<T>(std::make_shared<T>(data_queue_.front()));
        data_queue_.pop();
        return rtn;
    }

    bool try_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mtx_);
        if (!data_queue_.empty())
        {
            value = data_queue_.front();
            data_queue_.pop();
            return true;
        }
        return false;
    }

    std::shared_ptr<T> try_pop()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        if (!data_queue_.empty())
        {
            auto rtn = std::shared_ptr<T>(std::make_shared<T>(data_queue_.front()));
            data_queue_.pop();
            return rtn;
        }
        return std::shared_ptr<T>();
    }

    bool empty() const
    {
        std::unique_lock<std::mutex> lk(mtx_);
        return data_queue_.empty();
    }

    bool size() const
    {
        std::unique_lock<std::mutex> lk(mtx_);
        return data_queue_.size();
    }
    
private:
    mutable std::mutex mtx_;
    std::condition_variable cond_;
    std::queue<T> data_queue_;
};

}
