#ifndef _THREADSAFE_QUEUE_H
#define _THREADSAFE_QUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <optional>

namespace threadsafe
{
    template <typename T>
    class queue
    {
        std::queue<T> queue_;
        mutable std::mutex mutex_;

    public:
        queue() = default;
        queue(const queue<T> &) = delete;
        queue &operator=(const queue<T> &) = delete;

        queue(queue<T> &&other) noexcept(false)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!empty())
                // TODO: Exception handler
                throw "Error";

            queue_ = std::move(other.queue_);
        }

        virtual ~queue() noexcept(true) {}

        size_t size() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size();
        }

        inline bool empty() const { queue_.empty(); }

        void push(const T &item)
        {
            std::lock_guard<std::mutex>(mutex_);
            queue_.push(item);
        }

        std::optional<T> pop()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (queue_.empty())
                return {};

            T tmp = queue_.front();
            queue_.pop();

            return tmp;
        }
    };
} // namespace threadsafe

#endif // _THREADSAFE_QUEUE_H