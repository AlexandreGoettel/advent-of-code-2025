#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

#include "aoc.h"

using namespace std;

struct Distance {
    int A;
    int B;
    // double distance;
    unsigned long int distance;

    bool operator<( const Distance& rhs ) const
        { return distance < rhs.distance; }
};

struct Junction {
    int x;
    int y;
    int z;
};

auto AOC::part_1() -> unsigned long int {
    string line;
    int x, y, z;
    int n_lines = 0;
    while (getline(file, line)) n_lines++;
    file.clear();
    file.seekg(0);
    Junction junctions[n_lines];
    int i = 0;
    while (getline(file, line)) {
        sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
        Junction *junction = new Junction;
        junction->x = x;
        junction->y = y;
        junction->z = z;
        junctions[i] = *junction;
        i++;
    }

    // Make array of distances
    int n_distances = n_lines / 2 * (n_lines - 1);
    Distance distances[n_distances];
    i = 0;
    for (int j = 0; j < n_lines; j++) {
        for (int k = 0; k < j; k++) {
            if (j == k) continue;
            Distance *dist = new Distance;
            dist->A = j;
            dist->B = k;
            auto a = junctions[j], b = junctions[k];
            dist->distance = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
            distances[i] = *dist;
            i++;
        }
    }

    // Each distance has two ids for the boxes
    sort(distances, distances + n_distances);

    // Both ids go into a set?
    vector<set<int>> circuits;
    set<int> init{distances[0].A, distances[0].B};
    circuits.push_back(init);

    // For each distance, either add to set or create a new set
    for (int j = 1; j < 1000; j++) {
        // If ids in a circuit set, add
        auto dist = distances[j];
        // cout << "Distance: " << dist.A << "-" << dist.B << ": " << dist.distance << endl;
        vector<int> found;

        for (int k = 0; k < circuits.size(); k++) {
            auto circuit = &circuits[k];
            if (circuit->find(dist.A) != circuit->end()
                    && circuit->find(dist.B) != circuit->end()) {
                found.push_back(-1);
                break;
            }
            else if (circuit->find(dist.A) != circuit->end()
                    || circuit->find(dist.B) != circuit->end()) {
                found.push_back(k);
            }
        }
        if (found.size() == 0) {  // Create a new circuit
            set<int> circuit{dist.A, dist.B};
            circuits.push_back(circuit);
        // } else if (found.size() == 1 && found[0] >= 0) {
        } else if (found.size() >= 1 && found[0] >= 0) {
            auto circuit = &circuits[found[0]];
            // cout << "Existing circuit: ";
            // for (auto el : *circuit) cout << el << ",";
            // cout << ": " << dist.A << "-" << dist.B << endl;
            circuit->insert(dist.A);
            circuit->insert(dist.B);
        }
        // else if (found.size() == 2) {
        //     auto circuit = &circuits[found[0]];
        //     for (auto el : circuits[found[1]]) circuit->insert(el);
        //     cout << "Removing circuit: ";
        //     for (auto el : circuits[found[1]]) cout << el << ",";
        //     cout << endl << "Replace: ";
        //     for (auto el : circuits[found[0]]) cout << el << ",";
        //     cout << endl;
        //     circuits.erase(circuits.begin() + found[1]);
        // }
    }

    // Merge overlapping circuits
    vector<set<int>> overlapped;
    // set<int> start;
    // for (auto el : circuits[0]) start.insert(el);
    // overlapped.push_back(start);

    int changed = true;
    while (changed) {
        changed = false;
        for (auto circuit : circuits) {
            // If a circuit has an element in common, add to overlapped set
            bool has_overlap = false;
            for (int j = 0; j < overlapped.size(); j++) {
                auto inner_circuit = &overlapped[j];
                for (auto el : circuit) if (inner_circuit->find(el) != inner_circuit->end()) {
                    has_overlap = true;
                    changed = true;
                    for (auto _el : circuit) inner_circuit->insert(_el);
                    break;
                }
            }
            if (!has_overlap) overlapped.push_back(circuit);
        }
        circuits.clear();
        for (auto el : overlapped) circuits.push_back(el);
        overlapped.clear();
    }

    vector<size_t> sizes;
    for (auto el : circuits) {
        cout << el.size() << ", ";
        sizes.push_back(el.size());
    }
    cout << endl;

    sort(sizes.begin(), sizes.end());
    unsigned long int count = 1;
    for (int j = sizes.size() - 1; j > sizes.size() - 4; j--) {
        cout << sizes[j] << ",";
        count *= sizes[j];
    }
    cout << endl;
    return count;
}

auto AOC::part_2() -> unsigned long int {
    unsigned long int count;
    return count;
}

int main() {
    auto *aoc = new AOC(8, false);
    aoc->run(1);  // > 18750
    // aoc->run(2);
}