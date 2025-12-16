#include <iostream>
#include <unordered_set>
#include <cstring>
#include <cmath>
#include <vector>

#include "aoc.h"

using namespace std;

int traverse(unordered_set<size_t> *beams, string line) {
    int output = 0;
    size_t pos = 0;
    while ((pos = line.find('^', pos)) != line.npos) {
        if (beams->find(pos) != beams->end()) {
            beams->erase(pos);
            beams->insert(pos - 1);
            beams->insert(pos + 1);
            output++;
        }
        ++pos;
    }
    return output;
}

auto AOC::part_1() -> unsigned long int {
    // Initialise & find starting position
    unsigned long int count = 0;
    string line;
    getline(file, line);
    auto pos = line.find('S');
    unordered_set<size_t> beams{pos};

    // Iteratively split the beam
    while (getline(file, line)) count += traverse(&beams, line);
    return count;
}

void traverse_multiverse(unsigned long int beams[], string line) {
    size_t pos = 0;
    while ((pos = line.find('^', pos)) != line.npos) {
        unsigned long int ref = beams[pos];
        beams[pos - 1] += ref;
        beams[pos + 1] += ref;
        beams[pos] = 0;
        ++pos;
    }
}

auto AOC::part_2() -> unsigned long int {
    string line;
    getline(file, line);
    auto pos = line.find('S');
    unsigned long int beams[line.size()] = {};
    beams[pos] = 1;

    while (getline(file, line)) traverse_multiverse(beams, line);

    unsigned long int count = 0;
    for (auto el : beams) count += el;
    return count;
}

int main() {
    auto *aoc = new AOC(7, false);
    aoc->run(1, 100);  // 1630
    aoc->run(2, 100);  // 47857642990160
}
