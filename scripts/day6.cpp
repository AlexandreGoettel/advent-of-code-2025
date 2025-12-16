#include <vector>
#include <regex>
#include <iterator>
#include <iostream>
#include <cctype>
#include <cmath>

#include "aoc.h"

using namespace std;

struct Ops {
    vector<unsigned int> nums;
    bool is_plus;
};

auto AOC::part_1() -> unsigned long int {
    string line;
    unsigned long int count = 0;
    // Initialise with the first line
    vector<Ops> data;
    getline(file, line);

    unsigned int num;
    regex intRegex(R"(\d+)");
    smatch matches;
    string::const_iterator searchStart(line.cbegin());
    while (regex_search(searchStart, line.cend(), matches, intRegex)) {
        Ops ops;
        ops.nums.push_back(stoi(matches[0]));
        data.push_back(ops);

        searchStart = matches[0].second;
    }

    // Read the rest of the data
    regex opsRegex(R"([+*])");
    while (getline(file, line)) {
        string::const_iterator searchStart(line.cbegin());
        int i = 0;
        while (regex_search(searchStart, line.cend(), matches, intRegex)) {
            data[i].nums.push_back(stoi(matches[0]));

            i++;
            searchStart = matches[0].second;
        }
        while (regex_search(searchStart, line.cend(), matches, opsRegex)) {
            data[i].is_plus = (matches[0] == "+");

            i++;
            searchStart = matches[0].second;
        }
    }

    for (auto el : data) {
        if (el.is_plus) {
            for (auto num : el.nums) count += num;
        } else {
            unsigned long int inner = 1;
            for (auto num : el.nums) inner *= num;
            count += inner;
        }
    }

    return count;
}

auto AOC::part_2() -> unsigned long int {
    string line;
    unsigned long int count = 0;

    // Initialise
    vector<string> lines;
    // vector<bool> is_plus;
    size_t max_cols = 0;
    regex opsRegex(R"([+*])");
    smatch matches;
    string::const_iterator searchStart(line.cbegin());
    while (std::getline(file, line)) {
        lines.push_back(line);
        if (line.length() > max_cols) max_cols = line.length();

        // while (regex_search(searchStart, line.cend(), matches, opsRegex)) {
        //     is_plus.push_back(matches[0] == "+");

        //     searchStart = matches[0].second;
        // }
    }

    // Implement gastropod maths
    int nums[4] = {};
    bool is_plus;
    int num_idx = 0;
    for (int j = 0; j <= max_cols; j++) {
        // If the whole row is empty, add to count and reset
        bool is_empty = true;
        for (int i = 0; i < lines.size(); i++) if (lines[i][j] != ' ') is_empty = false;
        if (is_empty || j == max_cols) {
            if (is_plus) for (auto num : nums) count += num;
            else {
                unsigned long int inner = 1;
                for (auto num : nums) if (num > 0) inner *= num;
                count += inner;
            }
            // cout << is_plus << ": " << count << " - ";
            // for (auto num : nums) cout << num << "-";
            // cout << endl;
            fill(nums, nums + 4, 0);
            num_idx = 0;
            continue;
        }

        // Add the numbers
        int pow_idx = 0;
        for (int i = lines.size() - 2; i >= 0; i--) {
            char pos = lines[i][j];
            if (!isdigit(pos)) continue;

            int n = pos - '0';
            nums[num_idx] += n * pow(10, pow_idx);
            pow_idx++;
        }
        num_idx++;

        // If the bottom char is an operation, modify control
        auto control = lines[lines.size() - 1][j];
        switch (control)
        {
            case '+':
                is_plus = true;
                break;
            case '*':
                is_plus = false;
                break;
            default:
                break;
        }
    }
    return count;
}

int main() {
    auto *aoc = new AOC(6, false);
    aoc->run(1);
    aoc->run(2);
}
