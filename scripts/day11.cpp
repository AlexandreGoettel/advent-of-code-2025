#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <set>

#include "aoc.h"

using namespace std;

unsigned long int recurse(unordered_map<string, vector<string>> *server, string key,
                          unsigned long int output, bool first) {
    if (!first && key == "you") return output;
    if (key == "out") return output + 1;

    for (auto el : (*server)[key]) {
        output = recurse(server, el, output, false);
    }
    return output;
}

auto AOC::part_1() -> unsigned long int {
    string line;
    regex pattern(R"(\w+)");
    smatch matches;
    unordered_map<string, vector<string>> server;

    while (getline(file, line)) {
        string::const_iterator searchStart(line.cbegin());
        bool first = true;
        string key;
        vector<string> values;
        while (regex_search(searchStart, line.cend(), matches, pattern)) {
            if (first) {
                key = matches[0];
                first = false;
            } else {
                values.push_back(matches[0]);
            }
            searchStart = matches[0].second;
        }
        server[key] = values;
    }

    // Just follow all options from "you" to "out"
    unsigned long int count = 0;
    count = recurse(&server, "you", count, true);
    return count;
}

unsigned long int recurse_dacfft(unordered_map<string, vector<string>> *server, string key,
                                 unsigned long int output, bool first, bool hasfft, bool hasdac) {
    if (!first && key == "svr") return output;
    if (key == "out" && hasdac && hasfft) return output + 1;
    if (key == "out") return output;

    if (key == "dac") hasdac = true;
    else if (key == "fft") hasfft = true;

    for (auto el : (*server)[key]) {
        output = recurse_dacfft(server, el, output, false, hasfft, hasdac);
    }
    return output;
}

unsigned long int recurse_partial(unordered_map<string, vector<string>> *server, string key,
                                  string startkey, string outkey, bool first, unsigned long int output) {
    if (!first && key == startkey) return output;
    if (key == outkey) return output + 1;

    for (auto el : (*server)[key]) {
        output = recurse_partial(server, el, startkey, outkey, false, output);
    }
    return output;
}

auto AOC::part_2() -> unsigned long int {
    string line;
    regex pattern(R"(\w+)");
    smatch matches;
    unordered_map<string, vector<string>> server;

    while (getline(file, line)) {
        string::const_iterator searchStart(line.cbegin());
        bool first = true;
        string key;
        vector<string> values;
        while (regex_search(searchStart, line.cend(), matches, pattern)) {
            if (first) {
                key = matches[0];
                first = false;
            } else {
                values.push_back(matches[0]);
            }
            searchStart = matches[0].second;
        }
        server[key] = values;
    }

    // Test
    string start = "svr", end = "fft";
    // string start = "fft", end = "dac";
    // string start = "dac", end = "out";
    unordered_map<string, unsigned long int> active, tmp;
    for (auto el : server[start]) active[el] = 1;
    unsigned long int num_fft = 0;
    while (active.size() > 0)
    {
        bool do_break = false;
        for (auto kv : active) {
            for (auto key : server[kv.first]) {
                if (key == end) num_fft += kv.second;
                else if (key != "out") {
                    if (tmp.find(key) != tmp.end()) tmp[key] += kv.second;
                    else tmp[key] = kv.second;
                }
            }
        }
        active = move(tmp);
    }
    return num_fft;
}


int main() {
    AOC *aoc = new AOC(11, false);
    aoc->run(1);
    aoc->run(2);
}
