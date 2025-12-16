#include <iostream>
#include <complex>
#include <set>
#include <vector>
#include <regex>
#include <iterator>
#include <cassert>

#include "aoc.h"

using namespace std;

struct GridInfo {
    std::vector<std::complex<double>> points;
    int area;
};

vector<GridInfo> testGrid = {
    // 0: ###, ##., ##.
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
            {0, 1}, {1, 1},           // Second row: ##.
            {0, 2}, {1, 2}},          // Third row: ##.
        7,  // Area
    },
    // 1: ###, ##., .##
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
            {0, 1}, {1, 1},           // Second row: ##.
                    {1, 2}, {2, 2}},  // Third row: .##
        7,  // Area
    },
    // 2: .##, ###, ##.
    {
        {        {1, 0}, {2, 0},           // First row: .##
            {0, 1}, {1, 1}, {2, 1},   // Second row: ###
            {0, 2}, {1, 2}},          // Third row: ##.
        7,  // Area
    },
    // 3: ##., ###, ##.
    {
        {{0, 0}, {1, 0},           // First row: ##.
            {0, 1}, {1, 1}, {2, 1},   // Second row: ###
            {0, 2}, {1, 2}},          // Third row: ##.
        7,  // Area
    },
    // 4: ###, #.., ###
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
            {0, 1},                   // Second row: #..
            {0, 2}, {1, 2}, {2, 2}},  // Third row: ###
        7,  // Area
    },
    // 5: ###, .#., ###
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
                    {1, 1},                   // Second row: .#.
            {0, 2}, {1, 2}, {2, 2}},  // Third row: ###
        7,  // Area
    }
};

vector<GridInfo> mainGrid = {
    // 0: ###, ###, ..#
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
         {0, 1}, {1, 1}, {2, 1},  // Second row: ###
                         {2, 2}},     // Third row: ..#
        7,  // Area
    },
    // 1: ###, .#., ###
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
                 {1, 1},      // Second row: .#.
         {0, 2}, {1, 2}, {2, 2}}, // Third row: ###
        7,  // Area
    },
    // 2: ..#, .##, ###
    {
        {                {2, 0},      // First row: ..#
                 {1, 1}, {2, 1}, // Second row: .##
         {0, 2}, {1, 2}, {2, 2}}, // Third row: ###
        6,  // Area
    },
    // 3: ###, ..#, ###
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
                         {2, 1},      // Second row: ..#
         {0, 2}, {1, 2}, {2, 2}}, // Third row: ###
        7,  // Area
    },
    // 4: ..#, .##, ##.
    {
        {                {2, 0},      // First row: ..#
                 {1, 1}, {2, 1}, // Second row: .##
         {0, 2}, {1, 2}},         // Third row: ##.
        5,  // Area
    },
    // 5: ###, .##, ##.
    {
        {{0, 0}, {1, 0}, {2, 0},  // First row: ###
                 {1, 1}, {2, 1}, // Second row: .##
         {0, 2}, {1, 2}},         // Third row: ##.
        7,  // Area
    }
};

auto AOC::part_1() -> unsigned long int {
    vector<GridInfo> grid;
    if (test) grid = testGrid;
    else grid = mainGrid;

    string line;
    regex xpattern(R"(\d+x\d+)");
    regex pattern(R"(\d+)");
    smatch matches;
    int count = 0;
    while (getline(file, line)) {
        int x, y, n = 0;
        vector<int> nboxes;
        string::const_iterator searchStart(line.cbegin());
        // Skip line if no x is found
        regex_search(searchStart, line.cend(), matches, xpattern);
        if (matches[0].second == line.cend()) continue;

        // Read in instructions
        while (regex_search(searchStart, line.cend(), matches, pattern)) {
            int num = stoi(matches[0]);
            if (n == 0) x = num;
            else if (n == 1) y = num;
            else nboxes.push_back(num);

            n++;
            searchStart = matches[0].second;
        }
        // cout << x << "," << y << ": ";
        // for (auto el : nboxes) cout << el << " ";
        // cout << endl;

        // Max area check
        int area = 0;
        for (int i = 0; i < nboxes.size(); i++) area += nboxes[i] * grid[i].area;
        // cout << area << " > " << x * y << "?" << endl;
        if (area > x * y) continue;

        // 3 by 3 check
        int n_x = x / 3, n_y = y / 3;
        int sum_nboxes = 0;
        for (auto el : nboxes) sum_nboxes += el;
        cout << n_x << "x" << n_y << "=" << n_x*n_y << " < " << sum_nboxes << "?" << endl;
        if (n_x * n_y < sum_nboxes) continue;

        // Panic
        cout << line << endl;
        count++;
        // assert(false);
    }
    return count;
}

auto AOC::part_2() -> unsigned long int {
    return 0;
}

int main() {
    AOC *aoc = new AOC(12, false);
    aoc->run(1);
    aoc->run(2);
}
