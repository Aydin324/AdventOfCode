//
// Created by Sull on 11/7/2025.
//
#include <forward_list>
#include <iostream>
#include <sstream>
#include <vector>
#include "utils/input_reader.h"

using namespace std;

bool checkSafety(forward_list<int> numbers, int index_ignored = -1) {
    //deletion logic
    if (index_ignored != -1) {
        if (index_ignored == 0) numbers.pop_front();
        else {
            auto it = numbers.begin();
            advance(it, index_ignored-1);
            numbers.erase_after(it);
        }
    }

    //safety check logic
    auto current = numbers.begin();
    auto past = current++;
    bool increasing = (*current > *past);
    bool valid = true;
    while (current != numbers.end()) {
        if ((*current > *past && !increasing) || (*current < *past && increasing)) {
            valid = false;
            break;
        } else if (abs(*current - *past) < 1 || abs(*current - *past) > 3) {
            valid = false;
            break;
        }
        current++;
        past++;
    }
    return valid;
}

int main() {
    vector<string> lines = readLines("data/day2_input.txt");

    //extract
    /* PART 1
    int counter = 0;
    for (string& line : lines) {
        if (line.empty()) continue;
        istringstream iss(line);
        vector<int> numbers;
        int value;
        while (iss >> value) {
            numbers.push_back(value);
        }
        bool increasing = (numbers[1] > numbers[0]);
        bool valid = true;
        for (int i = 1; i<numbers.size(); i++) {
            if (abs(numbers[i]-numbers[i-1]) > 0 && abs(numbers[i]-numbers[i-1]) <= 3) {
                if ((numbers[i]>numbers[i-1] && !increasing) || (numbers[i]<numbers[i-1] && increasing)) {
                    valid = false;
                    break;
                }
            } else {
                valid = false;
            }
        }
        if (valid) {counter++;}
    }

    cout<<counter;
    */

    int counter = 0;
    for (string& line : lines) {
        //read a line and put it in numbers vector
        if (line.empty()) continue;
        istringstream iss(line);
        forward_list<int> numbers;
        auto it = numbers.before_begin();
        int value;
        while (iss >> value) {
            numbers.insert_after(it, value);
            it++;
        }
        if (checkSafety(numbers)) counter++;
        else {
            int list_size = distance(numbers.begin(), numbers.end());
            for (int i=0; i<list_size; i++) {
                if (checkSafety(numbers, i)) {
                    counter++;
                    break;
                }
            }
        }
    }

    cout<<counter;


    return 0;
}
