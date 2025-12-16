#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include "aoc.h"


// Constructor
AOC::AOC(int day, bool _test) {
    std::string filename;
    if (_test) filename = "input/day" + std::to_string(day) + "_test.csv";
    else filename = "input/day" + std::to_string(day) + ".csv";
    file.open(filename);
    test = _test;
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

/// @brief Run one of the day's part solutions
/// @param part
void AOC::run(int part) {
    unsigned long int count;
    set_start();
    if (part == 1) {
        count = part_1();
    } else if (part == 2) {
        count = part_2();
    } else return;
    set_end();

    file.clear();
    file.seekg(0);

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part " << part << " answer: " << count << std::endl;
    std::cout << "\tThis took: " << duration.count() << "mus" << std::endl;
}

/// @brief Run the part several times to get a time mean & uncertainty
/// @param part, repeat
void AOC::run(int part, int repeat) {
    std::chrono::microseconds v[repeat];
    unsigned long int count;
    for (int i = 0; i < repeat; i++) {
        set_start();
        if (part == 1) {
            count = part_1();
        } else if (part == 2) {
            count = part_2();
        } else return;
        set_end();
        v[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        file.clear();
        file.seekg(0);
    }
    double mean = 0, var = 0;
    for (auto el : v) mean += el.count() / repeat;
    for (auto el : v) var += pow(el.count() - mean, 2);
    var /= (repeat - 1);
    std::cout << "Part " << part << " answer: " << count << std::endl;
    std::cout << "\tThis took: " << std::setprecision(3) << mean;
    std::cout << " +- " << std::setprecision(3) << sqrt(var) << " mus" << std::endl;
}
