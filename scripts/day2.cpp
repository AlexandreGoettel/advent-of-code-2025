#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;


std::string repeat(const std::string &str, int n) {
    std::string result;
    for (int i = 0; i < n; ++i) {
        result += str;
    }
    return result;
}


int get_n_repetitions(unsigned long int num) {
    if (num < 10) return 0;
    if (num < 100) {
        if (num % 11 == 0) return 2;
        return 0;
    }

    string strnum = to_string(num);
    auto first = strnum[0];
    size_t size = strnum.size();

    for (int i = size / 2; i > 0; i--) {
        if (size % i != 0) continue;
        if (strnum[i] != first) continue;
        // There can be more repetitions
        auto sub = strnum.substr(0, i);
        int n_repetitions = size / i;
        if (strnum == repeat(sub, n_repetitions)) return n_repetitions;
    }
    return 0;
}


int main() {
    ifstream file("input/day2.csv");
    string line, cell;
    getline(file, line);
    stringstream lineStream(line);

    vector<unsigned long int> start, end;
    while (getline(lineStream, cell, ',')) {
        auto hyphenPos = cell.find("-");
        unsigned long int _start = stoul(cell.substr(0, hyphenPos));
        unsigned long int _end = stoul(cell.substr(hyphenPos + 1));
        start.push_back(_start);
        end.push_back(_end);
    }

    auto t_start = chrono::high_resolution_clock::now();
    unsigned long int count = 0, count_2 = 0;
    for (int i = 0; i < start.size(); i++) {
        auto _start = start[i];
        auto _end = end[i];

        for (unsigned long int num = _start; num <= _end; num++) {
            auto n_repetitions = get_n_repetitions(num);
            if (!n_repetitions) continue;
            if (n_repetitions == 2) count += num;
            count_2 += num;
        }
    }
    auto t_end = chrono::high_resolution_clock::now();
    cout << "Part 1 answer: " << count << endl;
    cout << "Part 2 answer: " << count_2 << endl;
    auto duration = chrono::duration_cast<chrono::milliseconds>(t_end - t_start);
    cout << "Time taken: " << duration.count() << " ms" << endl;
}
