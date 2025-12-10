#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include "utils/input_reader.h"

using namespace std;

const long double e = 1e-8; //tolerance
const int SEARCH_RANGE = 200;

struct Machine {
    vector<vector<long long>> A;
    vector<long long> b;    //target
    int J;
    int B;
};

Machine parseMachinePartTwo(const string& line) {
    Machine machine;

    int joltage_start = line.find('{');
    int joltage_end = line.find('}');
    string joltage_str = line.substr(joltage_start + 1, joltage_end - joltage_start - 1);
    stringstream ss_joltage(joltage_str);
    string joltage_val;
    while (getline(ss_joltage, joltage_val, ',')) machine.b.push_back(stoll(joltage_val));
    machine.J = machine.b.size();

    int lights_end = line.find(']');
    int buttons_start = lights_end + 1;
    string buttons_str = line.substr(buttons_start, joltage_start - buttons_start);
    stringstream ss_buttons(buttons_str);
    string button_schematic;
    vector<vector<long long>> toggles;
    while (getline(ss_buttons, button_schematic, '(')) {
        int end = button_schematic.find(')');
        if (end == string::npos) continue;

        string indices_str = button_schematic.substr(0, end);
        vector<long long> current_toggles(machine.J, 0);

        stringstream ss_indices(indices_str);
        string index_str;
        while (getline(ss_indices, index_str, ',')) {
            int index = stoi(index_str);
            if (index >= 0 && index < machine.J) {
                current_toggles[index] = 1;
            }
        }
        toggles.push_back(current_toggles);
    }
    machine.B = toggles.size();

    //transpose
    machine.A.resize(machine.J, vector<long long>(machine.B, 0));
    for (int j = 0; j < machine.B; ++j) {
        for (int i = 0; i < machine.J; ++i) {
            machine.A[i][j] = toggles[j][i];
        }
    }

    return machine;
}

long long solveMachinePartTwo(Machine& m) {
    if (m.J == 0 || m.B == 0) return 0;

    //augmented matrix
    vector<vector<long double>> M(m.J, vector<long double>(m.B + 1));
    for (int i = 0; i < m.J; ++i) {
        for (int j = 0; j < m.B; ++j) M[i][j] = (long double)m.A[i][j];
        M[i][m.B] = (long double)m.b[i];
    }
    int B_aug = m.B + 1;

    //pivots and Gauss
    int current_row = 0;
    vector<int> pivot_cols;
    for (int lead_col = 0; lead_col < m.B && current_row < m.J; ++lead_col) {
        int pivot_row_idx = current_row;
        for(int i = current_row + 1; i < m.J; ++i) {
            if (abs(M[i][lead_col]) > abs(M[pivot_row_idx][lead_col])) pivot_row_idx = i;
        }

        //non-zero check with tolerance
        if (abs(M[pivot_row_idx][lead_col]) > e) {
            swap(M[pivot_row_idx], M[current_row]);
            pivot_cols.push_back(lead_col);

            //normalize
            long double pivot_val = M[current_row][lead_col];
            for (int k = lead_col; k < B_aug; ++k) {
                M[current_row][k] /= pivot_val;
            }

            //eliminate other rows
            for (int r = 0; r < m.J; ++r) {
                if (r != current_row) {
                    long double factor = M[r][lead_col];
                    for (int k = lead_col; k < B_aug; ++k) {
                        M[r][k] -= factor * M[current_row][k];
                    }
                }
            }

            current_row++;
        }
    }

    //check
    for (int r = current_row; r < m.J; ++r) {
        if (abs(M[r][m.B]) > e) return -1;
    }

    //free variables
    vector<int> free_cols;
    int current_pivot_row = 0;
    for (int j = 0; j < m.B; ++j) {
        if (current_pivot_row < pivot_cols.size() && j == pivot_cols[current_pivot_row]) {
            current_pivot_row++;
        } else {
            free_cols.push_back(j);
        }
    }
    int k = free_cols.size();

    long long min_total_presses = -1;
    vector<long long> current_free_vars(k);

    //recursive search
    function<void(int)> search =
        [&](int index) {
        if (index == k) {
            vector<long double> current_x(m.B, 0.0);
            bool non_negative = true;
            bool all_integer = true;

            //set free variables
            for (int i = 0; i < k; ++i) current_x[free_cols[i]] = (long double)current_free_vars[i];

            //calculate pivot variables
            for (int r = 0; r < pivot_cols.size(); ++r) {
                int pivot_col = pivot_cols[r];
                long double value = M[r][m.B];
                for (int i = 0; i < k; ++i) value -= M[r][free_cols[i]] * current_x[free_cols[i]];

                current_x[pivot_col] = value;

                if (value < -e) {
                    non_negative = false;
                    break;
                }
                if (abs(value - round(value)) > e) {
                    all_integer = false;
                    break;
                }
            }
            if (!non_negative || !all_integer) return;

            //total presses
            long long current_total = 0;
            for (int j = 0; j < m.B; ++j) {
                long long presses = (long long)round(current_x[j]);
                if (presses < 0) return;
                current_total += presses;
            }

            if (min_total_presses == -1 || current_total < min_total_presses) {
                min_total_presses = current_total;
            }

        } else {
            for (long long val = -SEARCH_RANGE; val <= SEARCH_RANGE; ++val) {
                current_free_vars[index] = val;
                search(index + 1);
            }
        }
    };

    search(0);

    return min_total_presses;
}

int main() {
    vector<string> lines = readLines("data/day10_input.txt");
    long long total_min_presses = 0;
    int machine_count = 0;

    for (const string& line : lines) {
        cout<<"Processing machine no. "<<machine_count+1<<endl;

        Machine machine = parseMachinePartTwo(line);
        long long min_presses = solveMachinePartTwo(machine);

        if (min_presses == -1) {
            cout << "\tERROR on machine " << machine_count + 1 << endl;
        } else {
            total_min_presses += min_presses;
        }

        machine_count++;
    }

    cout << "\nTotal min presses: " << total_min_presses << endl;

    return 0;
}