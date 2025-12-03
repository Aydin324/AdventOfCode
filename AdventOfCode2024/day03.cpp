//
// Created by Sull on 11/9/2025.
//
#include <iostream>
#include <vector>
#include <regex>
#include "utils/input_reader.h"

using namespace std;

int main(){
    vector<string> lines = readLines("data/day3_input.txt");

    //part 1 regex
    //regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

    //part 2 regex
    const regex _pattern(R"((mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\)))");


    long long sum = 0;
    bool enabled = true;

    for (string& line : lines) {
        sregex_iterator it(line.begin(), line.end(), _pattern);
        sregex_iterator end;
        for (; it != end; ++it) {
            smatch match = *it;
            if (match[0] == "do()") {
                enabled = true;
            } else if (match[0] == "don't()") {
                enabled = false;
            } else if (match[1].matched && match[2].matched && enabled) {
                cout<<"match.str(2): "<<match.str(2)<<" match.str(3): "<<match.str(3)<<endl;
                sum += stoi(match.str(2)) * stoi(match.str(3));
            }
        }
    }

    cout<<sum;

    return 0;
}