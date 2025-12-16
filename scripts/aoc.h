#include <chrono>
#include <fstream>

#ifndef AOC_H_
#define AOC_H_

class AOC {
    public:
        AOC() = default;
        AOC(int, bool);
        ~AOC() = default;
        AOC(const AOC&) = delete;  // copy
        AOC(AOC&&) = delete;  // move
        AOC &operator=(const AOC&) = delete;  // copy assignment
        AOC &operator=(AOC&&) = delete;  // move assignment

    private:
        unsigned long int part_1(), part_2();
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end, get_time();
        void set_start(), set_end();
        std::ifstream file;
    public:
        void run(int), run(int, int);
        bool test;
};

#endif
