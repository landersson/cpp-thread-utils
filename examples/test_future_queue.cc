//#include "async_queue.h"
//#include <cppformat/format.h>
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <vector>

#include "future_worker.h"

using namespace std::chrono_literals;

class DenetWorker : public toolbox::AsyncWorker<std::string, std::string>
{
public:
    DenetWorker()
        : AsyncWorker()
    {}

    std::string processJob(const std::string& job)
    {

        printf("Receiced job: '%s'\n", job.c_str());
        return job + ": Done";
    }
};

int main(int argc, char* argv[])
{
    DenetWorker worker;

    worker.start();

    std::vector<std::future<std::string>> jobs;

    jobs.push_back(worker.postJob("Hello"));
    std::this_thread::sleep_for(1s);
    worker.stop();

    std::string s = jobs[0].get();

    // jobs.push_back(worker.postJob("World"));
    std::cout << "Received: " << s << std::endl;
    // fmt::print("Received: {}\n", s);

    return 0;
}
