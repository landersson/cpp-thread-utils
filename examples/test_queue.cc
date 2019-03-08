#include <thread-utils/async_queue.h>

#include <thread>
#include <string>
#include <vector>
#include <future>
#include <chrono>
#include <iostream>
#include <iterator>

#include <fmt/printf.h>


using namespace std::chrono_literals;

typedef std::string QItemT;

void thread(toolbox::AsyncQueue<QItemT> & q)
{
    for (int i = 0; i < 1; i++)
    {
        QItemT j;
        if (!q.getItem(std::ref(j)))
            break;

        fmt::print("Got Job '{}' ({})\n", j,
                   reinterpret_cast<const void *>(j.data()));
    }
}


int main(int argc, char *argv[])
{
    toolbox::AsyncQueue<QItemT> job_q;

    std::thread t(thread, std::ref(job_q));

    for (int i = 0; i < 1; i++)
    {
        //job_q.postItem(i);
        //job_q.postItem(std::move(i));
        //std::string s = "short";
        std::string s = "0123456789ABCDEF";
        //job_q.postItem(std::string("apa"));
        //job_q.postItem(s);
        job_q.postItem(std::move(s));
        fmt::printf("s='{}'\n", s);
    }
    //job_q.stop();
    t.join();

    return 0;
}
