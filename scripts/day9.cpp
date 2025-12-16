#include <iostream>
#include <vector>
#include <complex>
#include <unordered_set>
#include <map>
#include "aoc.h"
#include <cassert>

using namespace std;


struct ComplexHash {
    std::size_t operator()(const std::complex<int>& c) const {
        return std::hash<int>()(c.real()) ^ (std::hash<int>()(c.imag()) << 1);
    }
};


auto AOC::part_1() -> unsigned long int {
    string line;
    int x, y;
    // vector<Tile> tiles;
    vector<complex<int>> edges;
    while (getline(file, line)) {
        sscanf(line.c_str(), "%d,%d", &x, &y);
        complex<int> num(x, y);
        edges.push_back(num);
    }

    // Check all areas
    unsigned long int largest_area = 0;
    for (int i = 0; i < edges.size(); i++) {
        for (int j = 0; j < i; j++) {
            auto diff = edges[j] - edges[i];
            unsigned long int area = (1UL + abs(diff.real())) * (1UL + abs(diff.imag()));
            if (area > largest_area) {
                // printf("(%d,%d)-(%d,%d)\n", edges[i].real(), edges[i].imag(), edges[j].real(), edges[j].imag());
                largest_area = area;
            }
        }
    }
    return largest_area;
}

// void setmax(map<int,int> &mymap) {
//     int _max = 0;
//     for (const auto& el : mymap) {
//         if (el.second < _max) {
//             bool higher_later = false;
//             for (const auto& inner : mymap) {
//                 if (inner.first <= el.first) continue;
//                 if (inner.second > el.second) {
//                     higher_later = true;
//                     break;
//                 }
//             }
//             if (higher_later) {
//                 mymap[el.first] = _max;
//             } else {
//                 _max = el.second;
//             }
//         } else {
//             _max = el.second;
//         }
//     }
// }

// void setmin(map<int,int> &mymap) {
//     int _min = 100000;
//     for (const auto& el : mymap) {
//         if (el.second > _min) {
//             bool lower_later = false;
//             for (const auto& inner : mymap) {
//                 if (inner.first <= el.first) continue;
//                 if (inner.second < el.second) {
//                     lower_later = true;
//                     break;
//                 }
//             }
//             if (lower_later) {
//                 mymap[el.first] = _min;
//             } else {
//                 _min = el.second;
//             }
//         } else {
//             _min = el.second;
//         }
//     }
// }

// // @brief Check whether the point is outside of the solid green loop
// bool is_outside(complex<int> pos, unordered_set<complex<int>, ComplexHash> *squares,
//                 int xmin, int xmax, int ymin, int ymax) {
//     int x = pos.real(), y = pos.imag();
//     bool is_in = false;
//     int crossings = 0;
//     for (int xi = xmin; xi <= x; xi++) {
//         complex<int> num(xi, y);
//         bool found = squares->find(num) != squares->end();
//         if (!is_in && found) {
//             is_in = true;
//         } else if (is_in && !found) {
//             is_in = false;
//             crossings++;
//         }
//         // previous = found;
//     }
//     if (!is_in && crossings & 2 == 0) return true;
//     // if (!is_in) return true;

//     is_in = false;
//     crossings = 0;
//     for (int yi = ymin; yi <= y; yi++) {
//         complex<int> num(x, yi);
//         bool found = squares->find(num) != squares->end();
//         if (!is_in && found) {
//             is_in = true;
//         } else if (is_in && !found) {
//             is_in = false;
//             crossings++;
//         }
//     }
//     if (is_in) return false;
//     return crossings % 2 == 0;
//     // return !is_in;
// }

// auto AOC::part_2() -> unsigned long int {
//     string line;
//     int x, y;
//     int xmax = 0, ymax = 0, xmin = 100000, ymin = 100000;
//     vector<complex<int>> edges;
//     while (getline(file, line)) {
//         sscanf(line.c_str(), "%d,%d", &x, &y);
//         complex<int> num(x, y);
//         edges.push_back(num);
//         xmax = max(x, xmax);
//         ymax = max(y, ymax);
//         ymin = min(y, ymin);
//         xmin = min(x, xmin);
//     }

//     cout << "Connecting the lines.." << endl;
//     unordered_set<complex<int>, ComplexHash> squares;
//     for (int i = 0; i < edges.size(); i++) {
//         complex<int> x1 = edges[i];
//         for (int j = 0; j < i; j++) {
//             complex<int> x2 = edges[j];
//             if (x1.real() == x2.real()) {
//                 for (int k = min(x1.imag(), x2.imag()); k <= max(x1.imag(), x2.imag()); k++) {
//                     complex<int> num(x1.real(), k);
//                     squares.insert(num);
//                 }
//             } else if (x1.imag() == x2.imag()) {
//                 for (int k = min(x1.real(), x2.real()); k <= max(x1.real(), x2.real()); k++) {
//                     complex<int> num(k, x1.imag());
//                     squares.insert(num);
//                 }
//             }
//         }
//     }

//     // Idea: for every bounding box, check every outside point
//     // Have rejected set and accepted set!
//     // If not in set:
//         // Check point
//         // If outside, add to set, reject point
//         // If inside, accept point
//     // Else, reject point
//     cout << "Calculating areas.." << endl;
//     unsigned long int largest_area = 0;
//     unordered_set<complex<int>, ComplexHash> accepted, rejected;
//     for (int i = 0; i < edges.size(); i++) {
//         printf("%d/%lu\n", i, edges.size());
//         for (int j = 0; j < i; j++) {
//             int x1 = edges[i].real(), x2 = edges[j].real();
//             int y1 = edges[i].imag(), y2 = edges[j].imag();
//             if (x1 == x2 || y1 == y2) continue;  // Not going to win anyway
//             auto diff = edges[j] - edges[i];
//             unsigned long int area = (1UL + abs(diff.real())) * (1UL + abs(diff.imag()));
//             if (area <= largest_area) continue;
//             if (area >= 4555485954) continue;

//             bool skip = false;
//             int ys[2] = {y1, y2};
//             for (int xi = min(x1, x2); xi < max(x1, x2); xi++) {
//                 // Check y1, y2
//                 for (auto yi : ys) {
//                     complex<int> num(xi, yi);
//                     if (accepted.find(num) != accepted.end()) {
//                         continue;
//                     }
//                     if (rejected.find(num) != rejected.end()) {
//                         skip = true;
//                         break;
//                     }
//                     if (is_outside(num, &squares, xmin, xmax, ymin, ymax)) {
//                         rejected.insert(num);
//                         skip = true;
//                     } else {
//                         accepted.insert(num);
//                     }
//                     if (skip) break;
//                 }
//             }
//             if (skip) continue;

//             int xs[2] = {x1, x2};
//             for (int yi = min(y1, y2); yi < max(y1, y2); yi++) {
//                 // Check x1, x2
//                 for (auto xi : xs) {
//                     complex<int> num(xi, yi);
//                     if (accepted.find(num) != accepted.end()) {
//                         continue;
//                     }
//                     if (rejected.find(num) != rejected.end()) {
//                         skip = true;
//                         break;
//                     }
//                     if (is_outside(num, &squares, xmin, xmax, ymin, ymax)) {
//                         rejected.insert(num);
//                         skip = true;
//                     } else {
//                         accepted.insert(num);
//                     }
//                     if (skip) break;
//                 }
//             }
//             if (skip) continue;
//             if (area > largest_area) {
//                 largest_area = area;
//                 // printf("%lu: (%d,%d)-(%d,%d)\n", area, x1, y1, x2, y2);
//             }
//         }
//     }
//     // cout << "Accepted" << endl;
//     // for (auto el : accepted) printf("(%d,%d)-", el.real(), el.imag());
//     // cout << endl << "Rejected" << endl;
//     // for (auto el : rejected) printf("(%d,%d)-", el.real(), el.imag());
//     return largest_area;
// }

auto AOC::part_2() -> unsigned long int {
    // Fill lines
    // For each xpos, get vector of crossings
    // Sort pairs by area (part 1)
    // Starting from the top, iterate down using the vectors
    string line;
    int x, y;
    int xmin = 100000, xmax = 0;
    int ymin = 100000, ymax = 0;
    vector<complex<int>> edges;
    // unordered_set<complex<int>, ComplexHash> edges;
    while (getline(file, line)) {
        sscanf(line.c_str(), "%d,%d", &x, &y);
        complex<int> num(x, y);
        edges.push_back(num);
        xmin = min(xmin, x);
        xmax = max(xmax, x);
        ymin = min(ymin, y);
        ymax = max(ymax, y);
    }

    // Fill lines
    unordered_set<complex<int>, ComplexHash> squares;
    for (int i = 0; i < edges.size(); i++) {
        complex<int> x1 = edges[i];
        for (int j = 0; j < i; j++) {
            complex<int> x2 = edges[j];
            if (x1.real() == x2.real()) {
                for (int k = min(x1.imag(), x2.imag()); k <= max(x1.imag(), x2.imag()); k++) {
                    complex<int> num(x1.real(), k);
                    squares.insert(num);
                }
            } else if (x1.imag() == x2.imag()) {
                for (int k = min(x1.real(), x2.real()); k <= max(x1.real(), x2.real()); k++) {
                    complex<int> num(k, x1.imag());
                    squares.insert(num);
                }
            }
        }
    }

    // Get y-crossings for each x
    // Maybe I can read data is this way directly, get it for "free"?
    printf("Generating y-crossing map..\n");
    vector<int> crossings[xmax - xmin + 1];
    for (int x = xmin; x <= xmax; x++) {
        printf("%d/%d\n", x, xmax);
        bool is_inside = false;
        for (int y = ymin; y <= ymax; y++) {
            complex<int> num(x, y);
            bool is_found = squares.find(num) != squares.end();
            if (is_inside && !is_found) {
                is_inside = false;
                crossings[x - xmin].push_back(y - 1);
            }
            else if (!is_inside && is_found) {
                is_inside = true;
                crossings[x - xmin].push_back(y);
            }
        }
    }

    printf("Creating and sorting areas..\n");
    map<unsigned long int, tuple<int, int>> areas;
    for (int i = 0; i < edges.size(); i++) {
        complex<int> x1 = edges[i];
        for (int j = 0; j < i; j++) {
            auto diff = edges[j] - x1;
            unsigned long int area = (1UL + abs(diff.real())) * (1UL + abs(diff.imag()));
            tuple<int, int> ids (i, j);
            areas[area] = ids;
            // If an area is duplicated, then the problem is ill-defined anyway
        }
    }

    int count = 0;
    for (auto el : areas) {
        if (count >= 10) break;
        count++;
        cout << el.first << endl;
    }

    return squares.size();
}

int main() {
    AOC *aoc = new AOC(9, false);
    aoc->run(1);  // 4749672288
    aoc->run(2);  // < 3054069120,
    //              !=  119283293, 123688499
}


// 