//
// Created by Sull on 12/11/2025.
//
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "utils/input_reader.h"

using namespace std;

struct Device {
    string name;
    vector<string> connected_to;
};

unordered_map<string, Device> device_map;
unordered_map<string, long long> memo;

long long findPaths(const string& node, bool dac, bool fft, unordered_set<string>& visited) {
    if (visited.contains(node)) return 0;

    //memo
    int visit_key = (dac ? 2 : 0) + (fft ? 1 : 0);
    string key = node + "$" + to_string(visit_key);
    if (memo.contains(key)) {
        return memo[key];
    }

    visited.insert(node);
    bool new_dac = dac || (node == "dac");
    bool new_fft = fft || (node == "fft");

    long long count = 0;
    const Device& d = device_map[node];
    for (const string& s : d.connected_to) {
        if (s == "out") {
            if (new_dac && new_fft) {
                count++;
            }
        } else {
            unordered_set<string> new_visited = visited;
            count += findPaths(s, new_dac, new_fft, new_visited);
        }
    }

    memo[key] = count;

    visited.erase(node);
    return count;
}

int main() {
    vector<string> input = readLines("data/day11_input.txt");

    for (string &s: input) {
        Device d;
        int index = s.find(':');
        d.name = s.substr(0, index);
        stringstream ss(s.substr(index + 2));
        string temp;
        while (ss >> temp) {
            d.connected_to.push_back(temp);
        }
        device_map[d.name] = d;
    }

    unordered_set<string> visited;
    long long result = findPaths("svr", false, false, visited);

    cout << result << endl;

    return 0;
}