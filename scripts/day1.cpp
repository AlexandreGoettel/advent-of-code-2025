#include <iostream>
#include <vector>
#include "aoc.h"

using namespace std;

unsigned long int AOC::part_1() {
    std::vector<int> directions;
    std::vector<int> numbers;
    std::string line;

    unsigned int position = 50;
    unsigned long int count_1 = 0, count_2 = 0;

    position = 50;
    // Yay brute force!
    auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        char dir;
        unsigned int raw_num;
        if (sscanf(line.c_str(), "%c%d", &dir, &raw_num) == 2) {
            for (unsigned int i = 0; i < raw_num; i++) {
                if (dir == 'R') {
                    position += 1;
                    if (position == 100) {
                        position = 0;
                        count_2 += 1;
                    }
                } else {
                    if (position == 0) {
                        position = 99;
                    } else if (position == 1) {
                        position = 0;
                        count_2 += 1;
                    } else position -= 1;
                }
            }
            if (position == 0) count_1 += 1;
        }
    }
    // cout << "Part 2 answer: " << count_2 << endl;
    return count_1;
}

// Empty implementation because I'm doing both parts at once
unsigned long int AOC::part_2() {
    return 0;
}

int main() {
    AOC *aoc = new AOC(1, false);
    aoc->run(1, 10);
}
