#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>

using namespace std;


unsigned long int solve(int numbers[], size_t nmax, int n_digits) {
    unsigned long int output = 0;
    int left_index = -1, right_index;
    for (int n = n_digits; n >= 1; n--) {
        int largest_digit;
        right_index = -1;
        for (largest_digit = 9; largest_digit >= 1; largest_digit--) {
            for (int i = left_index + 1; i < nmax - (n - 1); i++) {
                if (numbers[i] == largest_digit) {
                    right_index = i;
                    break;
                }
            }
            if (right_index > -1) break;
        }
        output += pow(10, n - 1) * largest_digit;
        left_index = right_index;
    }
    return output;
}


int main() {
    ifstream file("input/day3.csv");
    string line;
    unsigned long int count = 0, count_2 = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while (getline(file, line)) {
        int numbers[line.size()];
        for (int i = 0; i < line.size(); i++) {
            numbers[i] = line[i]  - '0';
        }

        count += solve(numbers, line.size(), 2);
        count_2 += solve(numbers, line.size(), 12);
    }
    auto end = std::chrono::high_resolution_clock::now();
    cout << "Part 1 answer: " << count << endl;
    cout << "Part 2 answer: " << count_2 << endl;

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "This took: " << duration.count() << " mus" << std::endl;
}
