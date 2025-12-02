// day01.cpp
#include <algorithm>
#include <iostream>
#include <sstream>

#include "utils/input_reader.h"
#include <string>

using namespace std;

int main() {
    vector<string> lines = readLines("data/day1_input.txt");
    vector<long long> left, right;

    for (const string& line : lines) {
        if (line.empty()) continue;
        istringstream iss(line);
        long long a, b;
        iss >> a >> b;
        left.push_back(a);
        right.push_back(b);
    }

    // Sort both lists
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    /* PART 1
    long long totalDistance = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        totalDistance += abs(left[i] - right[i]);
    }

    cout << totalDistance << endl;
    */

    // PART 2 - CORRECT RESULT BUT LOGIC ERROR
    /*
    size_t cached_size = 0;
    int similarityScore = 0;
    for (size_t i = 0; i < left.size(); i++) {
        int count = 0;
        for (size_t j = cached_size; j < right.size(); j++, cached_size++) {
            if (right[j] == left[i]) {
                count++;
            } else if (right[j] > left[i]) {
                break;
            }
        }
        similarityScore += count * left[i];
    }

    cout<<similarityScore<<endl;
    */

    return 0;
}
