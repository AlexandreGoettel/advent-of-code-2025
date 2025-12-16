#include <iostream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include "aoc.h"

using namespace std;

int encode(vector<bool> *binary) {
    int result = 0;
    for (auto bit : *binary) result = (result << 1) | bit;
    return result;
}

long int encode_2(vector<int> *display) {
    long int result = 0;
    for (auto el : *display) result += result;
    return result;
}

vector<vector<int>> permute(vector<vector<int>> input, int n_buttons, int repetition) {
    if (repetition == 1) {
        for (auto i = 0; i < n_buttons; i++) {
            vector<int> inner;
            inner.push_back(i);
            input.push_back(inner);
        }
        return input;
    }

    vector<vector<int>> output;
    auto inner = permute(output, n_buttons, repetition - 1);
    for (auto el : inner) {
        for (int i = 0; i < n_buttons; i++) {
            vector<int> new_button;
            for (auto _el : el) new_button.push_back(_el);
            new_button.push_back(i);
            output.push_back(new_button);
        }
    }
    return output;
}

unsigned long int get_least_presses_bf(vector<bool> *positions, vector<vector<int>> *buttons) {
    int reference = encode(positions);
    int position = 0, n_presses = 1;  // FIXME
    int n_buttons[buttons->size()] = {};
    while (position != reference) {
        vector<vector<int>> presses;
        presses = permute(presses, buttons->size(), n_presses);

        vector<bool> state;
        for (int i = 0; i < positions->size(); i++) state.push_back(0);

        for (auto el : presses) {  // For each permutation of presses
            for (auto button_index : el) {  // For each button press
                for (auto pos : (*buttons)[button_index]) state[pos] = !state[pos];
            }

            // Check status and reset for the next sequence
            position = encode(&state);
            if (position == reference) break;
            for (int i = 0; i < positions->size(); i++) state[i] = 0;
        }
        n_presses++;
    }
    return n_presses - 1;
}


void print_matrix(vector<vector<double>> *matrix, string prefix) {
    cout << prefix << endl;
    for (auto row : *matrix) {
        for (int j = 0; j < row.size() - 1; j++) cout << row[j];
        cout << "|" << row[row.size() - 1] << endl;
    }
    cout << endl;
}

// @brief Return false if no swap will put a variable in the correct place
bool swap_if_no_value(vector<vector<double>> *matrix, int row, int col) {
    double max_row = row, max_val = (*matrix)[row][col];
    for (int i = row + 1; i < matrix->size(); i++) {
        auto val = (*matrix)[i][col];
        if (val == 0) continue;
        if (val > max_val || max_val == 0) {
            max_val = val;
            max_row = i;
        }
    }
    if (max_val == 0) return false;

    swap((*matrix)[max_row], (*matrix)[row]);
    return true;
}

bool subtract_if_lower_nonzero(vector<vector<double>> *matrix, int current_row, int current_var, int N_ROWS, int N_VARIABLES) {
    bool did_subtract = false;
    for (int row = current_row + 1; row < N_ROWS; row++) {
        double val = (*matrix)[row][current_var];
        if (val == 0) continue;

        double factor = val / (*matrix)[current_row][current_var];
        for (auto j = 0; j < N_VARIABLES; j++) {
            (*matrix)[row][j] -= factor * (*matrix)[current_row][j];
        }
        did_subtract = true;
    }
    return did_subtract;
}

bool subtract_if_upper_nonzero(vector<vector<double>> *matrix, int current_row, int j, int N_ROWS, int N_VARIABLES) {
    // Find a row below that can be subtracted
    for (int i = current_row; i < N_ROWS; i++) {
    // for (int i = j; i < N_ROWS; i++) {
        auto val = (*matrix)[i][j];
        if (val != 1) continue;

        // factor?
        for (int jj = j; jj < N_VARIABLES; jj++) (*matrix)[current_row - 1][jj] -= (*matrix)[i][jj];
        return true;
    }
    return false;
}

bool normalise(vector<vector<double>> *matrix, int current_row, int current_var, int N_VARIABLES) {
    double val = (*matrix)[current_row][current_var];
    if (val == 1 or val == 0) return false;

    double factor = 1 / val;
    for (int j = 0; j < N_VARIABLES; j++) {
        double el = (*matrix)[current_row][j];
        if (el == 0) continue;
        (*matrix)[current_row][j] = factor * el;
    }
    return true;
}

void gaussJordanElimination(vector<vector<double>> *matrix, bool verbose) {
    int N_ROWS = matrix->size(), N_VARIABLES = (*matrix)[0].size() - 1;
    int current_row = 0, current_var = 0;
    // for (int var = 0; var < N_VARIABLES; var++) {
    while (current_var < N_VARIABLES && current_row < N_ROWS) {
        cout << "CURRENT ROW/VAR: " << current_row << ", " << current_var << endl;
        // Check need to swap to get a value at [current_row][current_var]
        bool success = swap_if_no_value(matrix, current_row, current_var);
        if (!success) {
            current_var++;  // don't increment current_row
            continue;
        }
        else print_matrix(matrix, "SWAP");

        // Check if you need to subtract
        success = subtract_if_lower_nonzero(matrix, current_row, current_var, N_ROWS, N_VARIABLES + 1);
        if (success) print_matrix(matrix, "SUBTRACT");

        // Now normalise diagonal
        success = normalise(matrix, current_row, current_var, N_VARIABLES + 1);
        if (success) print_matrix(matrix, "NORMALISE");

        current_var++;
        current_row++;
    }

    // Phase 2 - Remove elements above the diagonal points
    for (int i = 0; i < N_ROWS - 1; i++) {
        // Find first element
        bool found = false;
        for (int j = 0; j < N_VARIABLES; j++) {
            if (!found && (*matrix)[i][j] == 1) {
                found = true;
                continue;
            }
            if (found && (*matrix)[i][j] != 0) {
                // cout << i << "," << j << endl;
                // Try and subtract!
                for (int ii = i + 1; ii < N_ROWS; ii++) {
                    if ((*matrix)[ii][j] == 0) continue;
                    // Only subtract if all elements to the left are 0
                    bool can_subtract = true;
                    for (int jj = 0; jj < j; jj++) {
                        if ((*matrix)[ii][jj] != 0) {
                            can_subtract = false;
                            break;
                        }
                    }
                    cout << "Can subtract?: " << can_subtract << endl;
                    if (!can_subtract) continue;

                    double val = (*matrix)[i][j];
                    double factor = val / (*matrix)[ii][j];
                    if (factor == 1/4) assert(false);
                    for (int jj = 0; jj < N_VARIABLES + 1; jj++) (*matrix)[i][jj] -= factor * (*matrix)[ii][jj];

                    break;  // Only one subtraction
                }
            }
        }
        print_matrix(matrix, "UPPER");
    }
}

unsigned long int get_least_presses_gj(vector<vector<double>> *buttons, bool verbose) {
    // vector<double> solution = (*buttons)[buttons->size() - 1];
    int j = buttons->size() - 1;
    vector<vector<double>> matrix((*buttons)[j].size(), vector<double>(buttons->size(), 0));
    for (int i = 0; i < j; i++) {
        for (auto el : (*buttons)[i]) matrix[el][i] = 1;
    }

    for (int i = 0; i < matrix.size(); i++) matrix[i][j] = (*buttons)[j][i];
    gaussJordanElimination(&matrix, verbose);

    // Encode linalg information
    vector<int> free, fixed;
    for (int jj = 0; jj < j; jj++) {
        int number_of_ones = 0;
        for (auto row : matrix) {
            if (row[jj] == 1) number_of_ones++;
            else if (row[jj] == 0) continue;
            else number_of_ones = 2;

            if (number_of_ones > 1) break;
        }
        if (number_of_ones > 1) free.push_back(jj);
        else fixed.push_back(jj);
    }
    // If the solution is unique, return the solution sum
    if (free.size() == 0) {
        unsigned long int sum = 0;
        for (auto row : matrix) sum += row[j];
        return sum;
    }

    // See what happens when you set the free variables to 0
    // Calculate individual coefficients
    // vector<vector<int>> ranges;
    // ranges = permute(ranges, 10, 2);
    // for (auto range : ranges) {
    //     for (auto el : range) cout << el << " ";
    //     cout << endl;
    // }

    // TODO - hardcode
    int MAX_COEFF = 30;
    vector<vector<int>> coefficients;
    coefficients = permute(coefficients, MAX_COEFF, free.size());
    int coef_index = 0;

    unsigned long int lowest_sum = 1000000;  // Just a large number
    for (auto coeffs : coefficients) {
        int one_hot_coeffs[j] = {};
        for (int i = 0; i < free.size(); i++) one_hot_coeffs[free[i]] = coeffs[i];

        double x = 0;  // some non-negative value
        int sum = 0;
        for (auto row : matrix) {
            // Find first 1
            int j_first_1 = -1;
            for (auto jj = 0; jj < j - 1; jj++) if (row[jj] == 1) {
                j_first_1 = jj;
                break;
            }
            if (j_first_1 == -1) continue;  // Skip empty row

            x = row[j];
            if (j_first_1 == j - 1) {sum += x; continue;}  // Trivial row

            // Add free variables
            // If this row's total is < 0, break and reject coeffs
            // Also, track row total for sum
            for (auto jj = j_first_1 + 1; jj < j; jj++) {
                auto val = row[jj];
                if (val == 0) continue;
                // if (coeffs[0] == 2) cout << val << one_hot_coeffs[jj] << endl;
                x -= val * one_hot_coeffs[jj];
            }
            if (x < 0) break;
            sum += x;
        }
        if (x < 0) continue;
        // Don't forget the coefficients!
        for (auto coeff : coeffs) sum += coeff;
        // for (auto coeff : coeffs) cout << coeff << ",";
        // cout << ": " << sum << endl;
        if (sum < lowest_sum) lowest_sum = sum;
    }
    return lowest_sum;
}

// n3 = 20 - n7
// n4 = 19 - n7
// n6 = -2 + n7

// sum = 60 - n7 = 58?

auto AOC::part_1() -> unsigned long int {
    string line;
    regex intRegex(R"([\d,]+)");
    smatch matches;

    unsigned long int count = 0;
    int i = 0;
    while (getline(file, line)) {
        // First get desired indicator light positions
        vector<bool> positions;
        for (auto el : line) {
            if (el == ']') break;
            else if (el == '.') positions.push_back(0);
            else if (el == '#') positions.push_back(1);
        }

        // Then get the button and joltage information
        string::const_iterator searcher(line.cbegin());
        vector<vector<int>> buttons;
        while (regex_search(searcher, line.cend(), matches, intRegex)) {
            vector<int> button;
            for (auto el : matches[0].str()) {
                if (el != ',') button.push_back(el - '0');
            }
            searcher = matches[0].second;
            buttons.push_back(button);
        }
        buttons.pop_back();

        // Find lowest number of button presses
        auto tmp = get_least_presses_bf(&positions, &buttons);
        cout << i << "/187: " << tmp << endl;
        count += tmp;
        i++;
    }
    return count;
}

auto AOC::part_2() -> unsigned long int {
    string line;
    regex intRegex(R"([\d+,]+)");
    smatch matches;

    unsigned long int count = 0;
    int i = 0;
    while (getline(file, line)) {
        string::const_iterator searcher(line.cbegin());
        vector<vector<double>> buttons;
        while (regex_search(searcher, line.cend(), matches, intRegex)) {
            vector<double> button;
            istringstream iss(matches[0].str());
            string token;
            while (getline(iss, token, ',')) {
                button.push_back(stoi(token));
            }

            searcher = matches[0].second;
            buttons.push_back(button);
        }

        i++;
        // if (i != 8) continue;
        count += get_least_presses_gj(&buttons, true);
    }
    return count;
}

int main() {
    AOC *aoc = new AOC(10, false);
    // aoc->run(1);
    aoc->run(2);  // < 3019548
    //              != 3019463
}

// 0: 138
// 1: 38
// 2: 52
// 3: 64
// 4: 62
// 5: 107 (++)
// 6: 199
// 7: 135@max=10, 126@max=20
