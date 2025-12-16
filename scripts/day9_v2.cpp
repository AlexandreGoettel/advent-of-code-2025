#include <iostream>
#include <vector>
#include <complex>
#include <unordered_set>
#include <set>
#include <map>
#include <algorithm>
#include "aoc.h"

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

// bool is_valid(complex<int> x1, complex<int> x2, set<int> crossings[]) {
//     int ymin = min(x1.imag(), x2.imag()), ymax = max(x1.imag(), x2.imag());
//     int xs[2] {x1.real(), x2.real()};
//     for (auto x : xs) {
//         bool start = false;
//         if (crossings[x].find(ymin) == crossings[x].end()) {
//             // Go from ymin to ymax. If crossing, INVALID
//             for (auto el : crossings[x]) {
//                 if (el > ymin && el <= ymax) return false;
//                 else if (el > ymax) break;
//             }

//         } else {
//             // Reasons to break
//             // ymax > max(crossings[x])
//             // ymin < min(crossings[x])
//             // Any dy > 1 between ymin and ymax
//             // if (*crossings[x].begin() > ymin) return false;
//             // if (*(--crossings[x].end()) < ymax) return false;

//             auto previous = ymin;
//             for (auto el : crossings[x]) {
//                 if (el > ymax) break;
//                 if (el < ymin) continue;
//                 if (el > previous + 1) return false;
//                 previous = el;
//             }
//         }
//     }
//     return true;
// }

bool is_valid(complex<int> x1, complex<int> x2, set<int> crossings[]) {
    // New idea: start from middle. If you cross a boundary, INVALID
    int xmid = (x1.real() + x2.real()) / 2;
    // Go along perimeter
    bool is_inside = false, is_inside_2 = false;
    for (auto x = xmid; x < max(x1.real(), x2.real()); x++) {
        auto cross = crossings[x];
        if (cross.find(x1.imag()) != cross.end()) is_inside = true;
        else if (is_inside) return false;

        if (cross.find(x2.imag()) != cross.end()) is_inside_2 = true;
        else if (is_inside_2) return false;
    }
    is_inside = is_inside_2 = false;
    for (auto x = xmid; x > min(x1.real(), x2.real()); x--) {
        auto cross = crossings[x];
        if (cross.find(x1.imag()) != cross.end()) is_inside = true;
        else if (is_inside) return false;

        if (cross.find(x2.imag()) != cross.end()) is_inside_2 = true;
        else if (is_inside_2) return false;
    }

    int ymid = (x1.imag() + x2.imag()) / 2;
    int ymax = max(x1.imag(), x2.imag());
    int ymin = min(x1.imag(), x2.imag());
    int xs[2] {x1.real(), x2.real()};
    for (auto x : xs) {
        auto cross = crossings[x];
        for (auto el : cross) {
            if (el < ymid) continue;
            if (el >= ymax) break;
            for (int y = el + 1; y < ymax; y++) {
                if (cross.find(y) == cross.end()) return false;
            }
        }
        for (auto iter = cross.rbegin(); iter != cross.rend(); iter--) {
            if (*iter > ymid) continue;
            if (*iter <= ymin) break;
            for (int y = *iter - 1; y > ymin; y--) {
                if (cross.find(*iter) == cross.end()) return false;
            }
        }
    }

    return true;
}

auto AOC::part_2() -> unsigned long int {
    // New idea
    // Get x and y crossings
    // Starting from center, get inner most crossing
    // by looking for jumps while looping sets
    string line;
    int x, y;
    set<int> xcrossings[100000];
    set<int> ycrossings[100000];
    return 0;
    vector<complex<int>> edges;
    while (getline(file, line)) {
        sscanf(line.c_str(), "%d,%d", &x, &y);
        xcrossings[x].insert(y);
        ycrossings[y].insert(x);

        complex<int> num(x, y);
        edges.push_back(num);
    }
    return 0;

    // Fill lines
    cout << "yay" << endl;
    for (int i = 0; i < edges.size(); i++) {
        complex<int> x1 = edges[i];
        for (int j = 0; j < i; j++) {
            complex<int> x2 = edges[j];
            if (x1.real() == x2.real()) {
                for (int k = min(x1.imag(), x2.imag()); k <= max(x1.imag(), x2.imag()); k++) {
                    xcrossings[x1.real()].insert(k);
                    ycrossings[k].insert(x1.real());
                }
            } else if (x1.imag() == x2.imag()) {
                for (int k = min(x1.real(), x2.real()); k <= max(x1.real(), x2.real()); k++) {
                    xcrossings[k].insert(x1.imag());
                    ycrossings[x1.imag()].insert(k);
                }
            }
        }
    }
    cout << "yay" << endl;

    // Sorting areas
    // map<unsigned long int, tuple<int, int>> areas;
    vector<pair<unsigned long int, tuple<int, int>>> areas;
    for (int i = 0; i < edges.size(); i++) {
        complex<int> x1 = edges[i];
        for (int j = 0; j < i; j++) {
            auto diff = edges[j] - x1;
            unsigned long int area = (1UL + abs(diff.real())) * (1UL + abs(diff.imag()));
            tuple<int, int> ids (i, j);
            // areas[area] = ids;
            areas.push_back({area, make_tuple(i, j)});
            // If an area is duplicated, then the problem is ill-defined anyway
        }
    }
    std::sort(areas.begin(), areas.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

    // Print the sorted vector
    int count = 0;
    for (const auto& entry : areas) {
        cout << entry.first << endl;
        if (count > 10) break;
        count++;
    }


    return 0;
}

// auto AOC::part_2() -> unsigned long int {
//     string line;
//     int x, y;
//     set<int> crossings[100000];
//     vector<complex<int>> edges;
//     while (getline(file, line)) {
//         sscanf(line.c_str(), "%d,%d", &x, &y);
//         // crossings[x].push_back(y);
//         crossings[x].insert(y);

//         complex<int> num(x, y);
//         edges.push_back(num);
//     }

//     // Fill lines
//     for (int i = 0; i < edges.size(); i++) {
//         complex<int> x1 = edges[i];
//         for (int j = 0; j < i; j++) {
//             complex<int> x2 = edges[j];
//             if (x1.real() == x2.real()) {
//                 for (int k = min(x1.imag(), x2.imag()); k <= max(x1.imag(), x2.imag()); k++) {
//                     crossings[x1.real()].insert(k);
//                     // complex<int> num(x1.real(), k);
//                     // squares.insert(num);
//                 }
//             } else if (x1.imag() == x2.imag()) {
//                 for (int k = min(x1.real(), x2.real()); k <= max(x1.real(), x2.real()); k++) {
//                     crossings[k].insert(x1.imag());
//                     // complex<int> num(k, x1.imag());
//                     // squares.insert(num);
//                 }
//             }
//         }
//     }

//     // printf("Creating and sorting areas..\n");
//     map<unsigned long int, tuple<int, int>> areas;
//     for (int i = 0; i < edges.size(); i++) {
//         complex<int> x1 = edges[i];
//         for (int j = 0; j < i; j++) {
//             auto diff = edges[j] - x1;
//             unsigned long int area = (1UL + abs(diff.real())) * (1UL + abs(diff.imag()));
//             tuple<int, int> ids (i, j);
//             areas[area] = ids;
//             // If an area is duplicated, then the problem is ill-defined anyway
//         }
//     }

//     // printf("Finding largest compatible area..\n");
//     int count = 0;
//     for (auto iter = areas.rbegin(); iter != areas.rend(); iter++) {
//         printf("%d/%lu\n", count, areas.size());
//         count++;

//         // line of shame
//         if (iter->first == 123688499 || iter->first >= 3054069120) continue;

//         auto i = get<0>(iter->second), j = get<1>(iter->second);
//         auto x1 = edges[i], x2 = edges[j];

//         if (is_valid(x1, x2, crossings)) {
//             printf("(%d,%d)-(%d,%d)", x1.real(), x1.imag(), x2.real(), x2.imag());
//             // for (auto el : crossings[x1.real()]) cout << el << endl;
//             return iter->first;
//         }

//     }
//     return 0;
// }

int main() {
    AOC *aoc = new AOC(9, false);
    aoc->run(1);  // 4749672288
    aoc->run(2);  // < 4555485954
}
