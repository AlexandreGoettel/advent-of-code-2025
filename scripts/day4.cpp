#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <chrono>

using namespace std;

int main() {
    ifstream file("input/day4.csv");

    // Alternative ideas:
    //  - use unordered map with complex numbers
    //  - use wave function collapse

    // First pass, get dimensions
    string line;
    size_t width = -1, height = 0;
    while (getline(file, line)) {
        if (width == -1) width = line.size();
        height++;
    }
    file.clear();
    file.seekg(0);

    // Second pass, read data
    int data[height][width];
    int hpos = 0;
    while (getline(file, line)) {
        for (int i = 0; i < width; i++) {
            auto el = line[i];
            if (el == '.') data[hpos][i] = 0;
            else data[hpos][i] = 1;
        }
        hpos++;
    }

    // Part 1
    auto start = chrono::high_resolution_clock::now();
    int n_rolls = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!data[i][j]) continue;
            int count = 0;
            for (int delta_i = -1; delta_i <= 1; delta_i++) {
                if (i + delta_i < 0 || i + delta_i >= height) continue;
                for (int delta_j = -1; delta_j <= 1; delta_j++) {
                    if (j + delta_j < 0 || j + delta_j >= width) continue;
                    if (delta_i == 0 && delta_j == 0) continue;

                    if (data[i + delta_i][j + delta_j]) count++;
                    if (count == 4) break;
                }
                if (count == 4) break;
            }
            if (count < 4) n_rolls++;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Part 1 answer: " << n_rolls << endl;
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "This took: " << duration.count() << " ms" << endl;

    // Part 2
    start = chrono::high_resolution_clock::now();
    int n_removed = 0;
    int n_can_remove = n_rolls;
    while (n_can_remove > 0) {
        n_can_remove = 0;
        vector<tuple<int, int>> to_remove;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {

                if (!data[i][j]) continue;
                int count = 0;
                for (int delta_i = -1; delta_i <= 1; delta_i++) {
                    if (i + delta_i < 0 || i + delta_i >= height) continue;
                    for (int delta_j = -1; delta_j <= 1; delta_j++) {
                        if (j + delta_j < 0 || j + delta_j >= width) continue;
                        if (delta_i == 0 && delta_j == 0) continue;

                        if (data[i + delta_i][j + delta_j]) count++;
                        if (count == 4) break;
                    }
                    if (count == 4) break;
                }
                if (count < 4) {
                    n_can_remove++;
                    to_remove.push_back(tuple<int, int>(i, j));
                }
            }
        }
        // Prepare next iteration
        for (auto el : to_remove) {
            data[get<0>(el)][get<1>(el)] = 0;
        }
        n_removed += n_can_remove;
    }

    end = chrono::high_resolution_clock::now();
    cout << "Part 2 answer: " << n_removed << endl;
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "This took: " << duration.count() << " ms" << endl;
}
