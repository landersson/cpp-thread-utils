
#pragma once

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <limits>
#include <chrono>
#include <type_traits>

namespace toolbox
{

template<typename T>
class AsyncQueue
{
public:
    AsyncQueue(unsigned max_queue_size = std::numeric_limits<unsigned>::max()) :
        _max_queue_size(max_queue_size),
        _stop(false)
    { }

    ~AsyncQueue()
    {
    }

    // return true if new item was placed in "item".
    // return false if timeout, or if queue was stopped.
    bool getItem(T &item, int timeout_ms = 0)
    {
        std::unique_lock<std::mutex> lock(_queue_mutex);

        if (timeout_ms > 0)
        {
            auto now = std::chrono::system_clock::now();
            if (!_item_posted.wait_until(lock, now + std::chrono::milliseconds(timeout_ms),
                                         [&](){ return !this->_item_queue.empty() || this->_stop; }))
            {
                return false;
            }
        }
        else
        {
            _item_posted.wait(lock,
                              [&](){ return !this->_item_queue.empty() || this->_stop; });
        }

        if (_stop) return false;

        //printf("R1: %p\n", _item_queue.front().data());
        item = std::move(_item_queue.front());
        //item = _move(std::move(_item_queue.front()));
        //T kalle(_move(std::move(_item_queue.front())));
        //printf("R2: %p\n", kalle.data());
        //printf("R2: %p\n", item.data());

        _item_queue.pop_front();
        _item_consumed.notify_all();

        return true;
    }

    //T&& _move(T && item)
    //{
        ////printf("_move: %p\n", item.data());
        //return std::move(item);
        ////printf("M0: %p\n", _item_queue.back().data());
    //}

    template<typename U>
    void postItem(U && item)
    {
        // statically assert that U has the same base type as T, and provide a
        // more readable error message if they differ.
        //static_assert(std::is_same<std::decay_t<U>,std::decay_t<T>>::value,
                      //"U must be the same as T");
        //static_assert(std::is_same<std::decay<U>::type, std::decay<T>::type>::value,
                      //"U must be the same as T");
        std::unique_lock<std::mutex> lock(_queue_mutex);

        _item_consumed.wait(lock,
                            [&](){ return this->_item_queue.size() < this->_max_queue_size; });

        this->_post(std::forward<U>(item));

        if (_item_queue.size() > 1)
            _item_posted.notify_all();
        else
            _item_posted.notify_one();
    }

    void flush()
    {
        T t;
        while (!this->empty())
            this->getItem(t);
    }

    void stop()
    {
        _stop = true;
        _item_posted.notify_all();
    }

    int queueSize()
    {
        _queue_mutex.lock();
        unsigned queue_size = _item_queue.size();
        _queue_mutex.unlock();
        return queue_size;
    }

    bool empty()
    {
        return this->queueSize() == 0;
    }

private:
    void _post(const T & item)
    {
        //printf("Copying...\n");
        _item_queue.push_back(item);
    }
    void _post(T && item)
    {
        //printf("Moving...\n");
        _item_queue.emplace_back(std::move(item));
        //printf("M0: %p\n", _item_queue.back().data());
    }

    unsigned                 _max_queue_size;
    bool                     _stop;
    std::mutex               _queue_mutex;
    std::condition_variable  _item_posted;
    std::condition_variable  _item_consumed;
    std::deque<T>            _item_queue;
};

} // namespace toolbox
