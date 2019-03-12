#include <thread-utils/async_worker.h>

#include <chrono>
#include <fmt/printf.h>
#include <future>
#include <iostream>
#include <string>
#include <vector>

using namespace std::chrono_literals;

class DenetWorker : public toolbox::AsyncWorker<std::string, std::string>
{
public:
    DenetWorker()
        : AsyncWorker()
    {}

    std::string processJob(const std::string& job)
    {
        fmt::printf("Worker: receiced job '%s' (%p)\n", job, job.data());
        std::this_thread::sleep_for(200ms);
        return job + ": Done";
    }
};

void client(int id, DenetWorker& worker)
{
    for (int i = 0; i < 4; i++)
    {
        std::string s = fmt::format("Job {} from thread {}", i, id);
        fmt::printf("Addr0: %p\n", s.data());
        auto fut = worker.postJob(std::move(s));
        fmt::print("Received result: '{}'\n", fut.get());
    }
}

int main(int argc, char* argv[])
{
    DenetWorker worker;
    worker.start();

    std::vector<std::thread> threads;
    for (int t = 0; t < 4; t++)
    {
        threads.emplace_back(std::thread(client, t, std::ref(worker)));
        std::this_thread::sleep_for(300ms);
    }

    for (auto&& th : threads)
    {
        th.join();
    }
    worker.stop();
    return 0;
}
