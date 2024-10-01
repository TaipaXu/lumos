#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
public:
    void push(T value);

    void close()
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            isClosed = true;
        }
        cv.notify_all();
    }

    std::optional<T> pop();

private:
    std::queue<T> queue;
    mutable std::mutex mtx;
    std::condition_variable cv;
    bool isClosed = false;
};

template <typename T>
void ThreadSafeQueue<T>::push(T value)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (isClosed)
        {
            throw std::runtime_error("Queue is closed");
        }
        queue.push(std::move(value));
    }
    cv.notify_one();
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::pop()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return isClosed || !queue.empty(); });

    if (queue.empty())
    {
        return std::nullopt;
    }

    T value = std::move(queue.front());
    queue.pop();
    return value;
}
