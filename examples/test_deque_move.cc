
#include <deque>
#include <fmt/printf.h>
#include <string>

int main(int argc, char* argv[])
{
    std::deque<std::string> q;

    std::string s = "APA KOLA HITTAR INTA KARTAN";
    fmt::printf("R2: %p\n", s.data());
    // q.emplace_back(s);
    // fmt::printf("R1: %p\n", q.front().data());
    // std::string s2(std::move(q.front()));
    std::string s2(std::move(s));
    fmt::print("S: {}\n", s);
    fmt::print("S2: {}\n", s2);
    // std::string s2(std::move(s));

    fmt::printf("R2: %p\n", s2.data());
    fmt::print("S: {}\n", q.size());

    return 0;
}
