//
// Created by Sull on 12/2/2025.
//
#include <cmath>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>

#include "utils/input_reader.h"

using namespace std;

int evenNUmOfDigits(long long number) {
    int count = 0;
    while (number > 0) {
        ++count;
        number /= 10;
    }
    return count%2==0;
}

void blinkRotation(list<long long>& stones) {
    auto it = stones.begin();
    for (; it != stones.end(); it++) {
        long long stone = *it;
        if (stone == 0) *it = 1;
        else if (evenNUmOfDigits(stone)) {
            string stoneString = to_string(stone);
            int len = stoneString.length();
            stones.emplace(it, stoll(stoneString.substr(0, len/2)));
            *it = stoll(stoneString.substr(len/2));
        } else {
            *it *= 2024;
        }
    }
}

//PART 2
static unordered_map<string, long long> results;

inline int digitCount(long long number) {
    int digits = number == 0 ? 1 : floor(log10(number)) + 1;
    return digits;
}

long long numOfChildren(long long number, int blink) {
    if (blink == 0) return 1;

    string key = to_string(number) + ',' + to_string(blink);
    if (results.contains(key)) return results[key];

    long long result;

    if (number == 0) result = numOfChildren(1, blink-1);
    else if (digitCount(number) % 2 == 0) {
        string sn = to_string(number);
        int half = sn.length()/2;
        result = numOfChildren(stoll(sn.substr(0, half)), blink-1)
            + numOfChildren(stoll(sn.substr(half)), blink-1);
    }
    else result = numOfChildren(number*2024, blink-1);

    results[key] = result;
    return result;
}



int main() {
    string input = readString("data/day11_input.txt");
    list<long long> stones;

    stringstream ss(input);
    string temp;
    while (getline(ss, temp, ' ')) {
        stones.emplace_back(stoll(temp));
    }

    //PART 1
    /*for (int i=0; i< 25; i++) {
        blinkRotation(stones);
    }*/

    //PART 2
    long long count = 0;
    for (long long number : stones) {
        count += numOfChildren(number, 75);
    }

    cout<<count<<endl;

    return 0;
}