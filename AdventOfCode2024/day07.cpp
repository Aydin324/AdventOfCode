//
// Created by Sull on 11/20/2025.
//
#include <bitset>
#include <iostream>
#include <sstream>
#include <valarray>
#include <vector>
#include "utils/input_reader.h"

using namespace std;

class operationMask {
    public:
    int* mask = new int[16];

    operationMask() {
        for (int i = 0; i < 16; i++) {
            mask[i] = 0;
        }
    }

    void incrementMask() {
        for (int i=0; i < 16; i++) {
            if (mask[i]==2) mask[i] = 0;
            else {
                mask[i]++;
                break;
            }
        }
    }

    ~operationMask() {
        delete[] mask;
    }
};

bool checkValidity(const long long result, const vector<int>& operands) {
    bitset<16> bits;
    int iterations = pow(2, operands.size()-1);
    long long test;

    for (int i=0; i<iterations; i++) {
        test = operands[0];
        for (int o=1; o<operands.size(); o++) {
            if (bits[o-1]==0) test += operands[o];
            else test *= operands[o];
        }
        if (test==result) return true;
        bits = i+1;
    }

    return false;
}

bool checkComplexValidity(const long long result, const vector<int>& operands) {
    operationMask mask;
    int iterations = static_cast<int>(pow(3, operands.size() - 1));
    long long test;

    for (int i=0; i<iterations; i++) {
        test = operands[0];
        for (int op = 1; op<operands.size(); op++) {
            switch (mask.mask[op-1]) {
                case 0:
                    test += operands[op];
                    break;
                case 1:
                    test *= operands[op];
                    break;
                default:
                    test = stoll(to_string(test)+to_string(operands[op]));
                    break;
            }
        }
        if (test==result) return true;
        mask.incrementMask();
    }

    return false;
}

int main() {
    vector<string> lines = readLines("data/day7_input.txt");

    vector<long long> results;
    vector<vector<int>> operands;

    //load
    for (int i=0; i<lines.size(); i++) {
        istringstream iss(lines[i]);
        string token;

        getline(iss, token, ':');
        results.push_back(stoll(token));
        getline(iss, token, ' ');

        vector<int> operandLine;
        while (getline(iss, token, ' ')) {
            operandLine.push_back(stoi(token));
        }
        operands.push_back(operandLine);
    }

    /* PART 1
    long long result = 0;
    for (int i=0; i<results.size(); i++) {
        if (checkValidity(results[i], operands[i])) result += results[i];
    }
    */

    //PART 2
    long long result = 0;

    for (int i=0; i<results.size(); i++) {
        if (checkComplexValidity(results[i], operands[i])) result += results[i];
    }

    cout<<result<<endl;

    return 0;
}