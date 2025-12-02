//
// Created by Sull on 12/1/2025.
//
#include <chrono>
#include <iostream>
#include "utils/input_reader.h"

using namespace std;

class Safe {
    public:
    int dial;

    Safe() {
        dial = 50;
    }

    //PART 1
    bool turnDial(const string& instruction) {
        bool isLeft = (instruction[0] == 'L');
        int number = stoi(instruction.substr(1));

        if (isLeft) {
            dial = (dial - number) % 100;
            if (dial < 0) dial += 100;
        } else {
            dial = (dial + number) % 100;
        }

        return (dial == 0);
    }

    //PART 2
    int turnClickableDial(const string& instruction) {
        bool isLeft = (instruction[0] == 'L');
        int number = stoi(instruction.substr(1));
        int zero_counter = 0;

        for (int i = 0; i < number; i++) {
            if (isLeft) {
                dial--;
                if (dial < 0) dial = 99;
            } else {
                dial++;
                if (dial == 100) dial = 0;
            }

            if (dial == 0) zero_counter++;
        }

        return zero_counter;
    }

    //PART 2 - OPTIMIZED
    int turnClickableDialOptimized(const string& instruction) {
        const bool isRight = (instruction[0] == 'R');
        const int number = stoi(instruction.substr(1));
        int counter = 0;

        if (isRight) {
            //number of whole rotations
            counter = number / 100;
            int incomplete_rotation = number - (counter*100);
            dial += incomplete_rotation;
            if (dial > 99) {
                ++counter;
                dial = dial % 100;
            }
        } else {
            counter = number / 100;
            const int incomplete_rotation = number - (counter*100);
            dial -= incomplete_rotation;
            if (dial < 0) {
                if (dial+incomplete_rotation != 0) ++counter;
                dial = 100 + dial;
            }
        }

        if (dial == 0 && !isRight) ++counter;

        return counter;
    }


};

int main() {
    vector<string> turns = readLines("data/day1_input.txt");
    Safe safe{};
    int count = 0;

    /* PART 1
    for (string& turn : turns) {
        if (safe.turnDial(turn)) count++;
    }
    */

    /* PART 2
    for (string& turn : turns) {
        count += safe.turnClickableDial(turn);
    }
    */

    //PART 2 - OPTIMIZED
    for (string& turn : turns) {
        count += safe.turnClickableDialOptimized(turn);
    }

    cout << count << endl;

    return 0;
}