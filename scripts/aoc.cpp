#include <iostream>
#include <string>
#include "aoc.h"


// Constructor
AOC::AOC(int day, bool test) {
    std::string filename;
    if (test) filename = "input/day" + std::to_string(day) + "_test.csv";
    else filename = "input/day" + std::to_string(day) + ".csv";
    file.open(filename);
}

auto AOC::get_time() -> std::chrono::time_point<std::chrono::high_resolution_clock> {
    auto output = std::chrono::high_resolution_clock::now();
    return output;
}

void AOC::set_start() {
    start = get_time();
}

void AOC::set_end() {
    end = get_time();
}

void AOC::run(int part) {
    unsigned long int count;
    set_start();
    if (part == 1) {
        count = part_1();
    } else if (part == 2) {
        count = part_2();
    } else return;
    set_end();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part " << part << " answer: " << count << std::endl;
    std::cout << "\tThis took: " << duration.count() << "mus" << std::endl;
}
