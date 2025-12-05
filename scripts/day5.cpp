#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

struct Pair {
    unsigned long int A;
    unsigned long int B;

    bool operator<( const Pair& rhs ) const
        { return A < rhs.A; }
};

int main() {
    ifstream file("input/day5.csv");
    string line;
    // vector<unsigned long int> left, right;
    vector<Pair> produce, produce_opt;
    unsigned int count = 0;
    auto start = chrono::high_resolution_clock::now();
    while (getline(file, line)) {
        unsigned long int one, two;
        if (sscanf(line.c_str(), "%lu-%lu", &one, &two) == 2) {
            Pair line;
            line.A = one;
            line.B = two;
            produce.push_back(line);
        } else if (sscanf(line.c_str(), "%lu", &one) == 1) {
            // Part 1
            for (auto el : produce_opt) {
                if (one >= el.A && one <= el.B) {
                    count++;
                    break;
                }
            }
        } else {
            sort(produce.begin(), produce.end());
            // Group overlapping bounds together
            for (int i = 0; i < produce.size() - 1; i++) {
                auto left = produce[i], right = produce[i + 1];

                if (left.B < right.A) {
                    produce_opt.push_back(left);
                }
                else if (left.B == right.A) {
                    produce[i + 1].A = left.A;
                }
                else {  // There is overlap
                    produce[i + 1].A = left.A;
                    if (right.B < left.B) produce[i + 1].B = left.B;
                }
            }
            produce_opt.push_back(produce[produce.size() - 1]);
        }
    }
    // Part 2
    unsigned long int count_2 = 0;
    for (auto el : produce_opt) {
        count_2 += el.B - el.A + 1;
    }
    auto end = chrono::high_resolution_clock::now();

    cout << "Part 1 answer: " << count << endl;  // 701
    cout << "Part 2 answer: " << count_2 << endl;  // 352340558684863
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "\tThis took: " << duration.count() << " mus" << endl;
}
