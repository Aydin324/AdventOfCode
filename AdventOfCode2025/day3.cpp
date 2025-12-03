//
// Created by Sull on 12/3/2025.
//
#include <iostream>
#include "utils/input_reader.h"

using namespace std;

//PART 1
int findJoltage(string& row) {
    int len = row.length();
    int max = 0;

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            int number = (row[i] - '0') * 10 + (row[j] - '0');
            if (number > max) {
                max = number;
            }
        }
    }

    return max;
}

//PART 2
long long findBigJoltage(const string& row) {
    int len = row.length();
    string result = "";

    int start = 0;
    for (int i = 0; i < 12; i++) {
        //index of last possible element
        int end = len - (12 - i);

        char maxDigit = '0';
        int maxPos = start;
        for (int j = start; j <= end; j++) {    //find max digit within given window
            if (row[j] > maxDigit) {
                maxDigit = row[j];
                maxPos = j;
                if (row[j]=='9') break;
            }
        }

        result.push_back(maxDigit);
        start = maxPos + 1; //set next window's start
    }

    return stoll(result);
}

int main() {
    vector<string> input = readLines("data/day3_input.txt");

    long long count = 0;
    for (string& s : input) {
        //count += findJoltage(s);
        count += findBigJoltage(s);
    }

    cout << count << endl;

    return 0;
}