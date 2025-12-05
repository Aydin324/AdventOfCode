#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include "utils/input_reader.h"

using namespace std;

vector<pair<long long, long long>> fresh;

void addToFresh(string& s) {
    int separator = s.find('-');
    long long a = stoll(s.substr(0, separator));
    long long b = stoll(s.substr(separator+1));
    fresh.emplace_back(make_pair(a, b));
}

bool isFresh(string& s) {
    long long candidate = stoll(s);
    for (pair<long long, long long> p : fresh) {
        if (candidate >= p.first && candidate <= p.second) return true;
    }
    return false;
}

int main() {
    vector<string> input = readLines("data/day5_input.txt");

    int empty = 0;
    for (string s: input) {
        if (s.empty()) {break;}
        addToFresh(s);
        empty++;
    }

    /* PART 1
    long long count = 0;
    for (int i = empty+1; i < input.size(); i++) {
        if (isFresh(input[i])) count++;
    }
    */

    //PART 2
    sort(fresh.begin(), fresh.end());
    long long count = 0;
    long long lower = fresh[0].first;
    long long upper = fresh[0].second;
    count += upper - lower + 1;
    for (int i = 1; i < fresh.size(); i++) {
        if (fresh[i].first >= lower && fresh[i].second <= upper) continue;
        if (fresh[i].first >= lower && fresh[i].first <= upper) {
            count += fresh[i].second - upper;
            upper = fresh[i].second;
            continue;
        }
        if (fresh[i].first > upper) {
            lower = fresh[i].first;
            upper = fresh[i].second;
            count += upper - lower + 1;
        }
    }

    cout<<count<<endl;

    return 0;
}