//
// Created by Sull on 12/10/2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <map>
#include "utils/input_reader.h"

using namespace std;

struct Machine {
    vector<vector<int>> A;
    vector<int> b;  //target
    int L;
    int B;
};

int charToBinary(char c) {
    return (c == '#') ? 1 : 0;
}

Machine parseMachine(const string& line) {
    Machine machine;

    int lights_end = line.find(']');
    int buttons_start = lights_end + 1;
    int buttons_end = line.find('{');

    string light_str = line.substr(1, lights_end - 1);
    machine.L = light_str.length();
    machine.b.resize(machine.L);
    for (int i = 0; i < machine.L; ++i) machine.b[i] = charToBinary(light_str[i]);

    string all_buttons = line.substr(buttons_start, buttons_end - buttons_start);
    stringstream ss_buttons(all_buttons);
    string button_schematic;
    vector<vector<int>> toggles;
    while (getline(ss_buttons, button_schematic, '(')) {
        int end = button_schematic.find(')');
        if (end == string::npos) continue;

        string indices_str = button_schematic.substr(0, end);
        vector<int> current_toggles(machine.L, 0);

        stringstream ss_indices(indices_str);
        string index_str;
        while (getline(ss_indices, index_str, ',')) {
            int index = stoi(index_str);
            if (index >= 0 && index < machine.L) current_toggles[index] = 1;
        }
        toggles.push_back(current_toggles);
    }
    machine.B = toggles.size();

    //transpose toggles into matrix
    machine.A.resize(machine.L, vector<int>(machine.B, 0));
    for (int j = 0; j < machine.B; ++j) {
        for (int i = 0; i < machine.L; ++i) {
            machine.A[i][j] = toggles[j][i];
        }
    }

    return machine;
}

long long solveMachine(Machine& m) {
    if (m.L == 0 || m.B == 0) return 0;

    //augmented matrix
    vector  <vector<int>> M = m.A;
    for (int i = 0; i < m.L; ++i) {
        M[i].push_back(m.b[i]);
    }
    int B_aug = m.B + 1;

    //pivots and Gaussian elimination
    vector<int> pivot_rows(m.B, -1);
    int current_row = 0;
    for (int lead_col = 0; lead_col < m.B && current_row < m.L; ++lead_col) {
        int pivot_row_idx = current_row;
        while (pivot_row_idx < m.L && M[pivot_row_idx][lead_col] == 0) {
            pivot_row_idx++;
        }
        //found pivot
        if (pivot_row_idx < m.L) {
            swap(M[pivot_row_idx], M[current_row]);
            pivot_rows[lead_col] = current_row;

            //xor rows
            for (int r = 0; r < m.L; ++r) {
                if (r != current_row && M[r][lead_col] == 1) {
                    // M[r] = M[r] XOR M[current_row]
                    for (int k = lead_col; k < B_aug; ++k) {
                        M[r][k] ^= M[current_row][k];
                    }
                }
            }

            current_row++;
        }
    }
    //M = RREF

    //check for row 0=1
    for (int r = current_row; r < m.L; ++r) {
        if (M[r][m.B] == 1) return -1;
    }

    //particular solution
    vector<int> pivot_cols;
    for(int j = 0; j < m.B; ++j) {
        if (pivot_rows[j] != -1) pivot_cols.push_back(j);
    }
    vector<int> free_cols;
    for (int j = 0; j < m.B; ++j) {
        if (pivot_rows[j] == -1) free_cols.push_back(j);
    }
    int k = free_cols.size(); // Number of free variables

    vector<int> xp(m.B, 0);
    for (int c : pivot_cols) {
        int r = pivot_rows[c];
        xp[c] = M[r][m.B];
    }

    //nullspace basis vectors
    vector<vector<int>> null_basis(k, vector<int>(m.B, 0));
    for (int i = 0; i < k; ++i) {
        int free_col = free_cols[i];

        null_basis[i][free_col] = 1;

        for (int c : pivot_cols) {
            int r = pivot_rows[c];
            null_basis[i][c] = M[r][free_col];
        }
    }

    //minimum Hamming weight
    long long min_presses = m.B + 1;
    for (int i = 0; i < (1 << k); ++i) {
        vector<int> current_x = xp;

        //apply combination
        for (int j = 0; j < k; ++j) {
            if ((i >> j) & 1) {
                for (int l = 0; l < m.B; ++l) current_x[l] ^= null_basis[j][l];
            }
        }
        long long current_weight = accumulate(current_x.begin(), current_x.end(), 0LL);
        min_presses = min(min_presses, current_weight);
    }

    return min_presses;
}

int main() {
    vector<string> lines = readLines("data/day10_input.txt");
    long long total_min_presses = 0;
    int machine_count = 0;

    for (const string& line : lines) {
        Machine machine = parseMachine(line);

        long long min_presses = solveMachine(machine);

        if (min_presses == -1) {
            cout << "\tERROR on machine " << machine_count + 1 << endl;
        } else {
            total_min_presses += min_presses;
        }

        machine_count++;
    }

    cout << "Total machines processed: " << machine_count << endl;
    cout << "Total min presses: " << total_min_presses << endl;

    return 0;
}