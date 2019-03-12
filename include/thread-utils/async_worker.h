
#pragma once

#include "async_queue.h"

#include <cassert>
#include <condition_variable>
#include <future>
#include <thread>

namespace toolbox {

template<typename J, typename R>
class AsyncWorker
{
    typedef std::pair<std::promise<R>, J> JobT;

public:
    typedef R Result;
    typedef J Job;

    AsyncWorker(int id = 0)
        : _id(id)
        , _done(false)
    {}

    virtual ~AsyncWorker()
    {
        if (_thread.joinable())
            stop();
    }

    void start() { _thread = std::thread(&AsyncWorker::run, this); }

    void run()
    {
        initThread();
        while (!_done)
        {
            JobT job;
            if (_job_queue.getItem(job))
            {
                auto& p = std::get<0>(job);
                // fmt::printf("WORKER: GOT JOB: %s (%p)\n",
                // std::get<1>(job), std::get<1>(job).data());

                p.set_value(std::move(this->processJob(std::get<1>(job))));
                //_job_valid = false;
            }
            if (this->_done)
                break;
        }
        //        printf("Leaving thread...\n");
        this->stopThread();
    }

    std::future<R> postJob(const J& job)
    {
        std::promise<R> p;
        auto fut = p.get_future();
        _job_queue.postItem(std::make_pair(std::move(p), job));
        return fut;
    }

    std::future<R> postJob(J&& job)
    {
        std::promise<R> p;
        auto fut = p.get_future();
        _job_queue.postItem(std::make_pair(std::move(p), std::move(job)));
        return fut;
    }

    void stop()
    {
        // printf("Stopping worker...\n");
        _done = true;
        _job_queue.stop();
        _thread.join();
    }

    // called from thead before starting job processing loop
    virtual void initThread() {}

    // called from thead before leaving job processing loop
    virtual void stopThread() {}

    // reimplement in subclass to process jobs in thread
    virtual R processJob(const J& job) = 0;

    int id() const { return _id; }

private:
    AsyncQueue<JobT> _job_queue;
    std::thread _thread;

    int _id;
    bool _done;
};

} // namespace toolbox
