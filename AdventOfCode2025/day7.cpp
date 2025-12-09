//
// Created by Sull on 12/7/2025.
//
#include <iostream>
#include "utils/input_reader.h"

using namespace std;

vector<vector<char>> input;
int sizeX, sizeY;

bool isValid(int x, int y) {
    if (x < 0 || x >= sizeX || y < 0 || y >= sizeY) return false;
    return true;
}

//PART 1
long long travelDown(int x, int y) {
    long long count = 0;
    input[y][x] = '|';

    while (true) {
        y++;
        if (!isValid(x, y)) return 0;
        if (input[y][x] == '^') {
            if (input[y][x-1] == '|' && input[y][x+1] == '|') break;
            count++;
            if (input[y][x-1] != '|') count += travelDown(x - 1, y);
            if (input[y][x+1] != '|') count += travelDown(x + 1, y);
            break;
        }
        input[y][x] = '|';
    }

    return count;
}

//PART 2
vector<vector<long long>> memo;

long long countTimelines(int x, int y) {
    if (memo[y][x] != -1) return memo[y][x];

    long long count = 0;
    int initial_y = y;

    while (true) {
        y++;
        if (!isValid(x, y)) return 1;
        if (input[y][x] == '^') {
            count += countTimelines(x - 1, y) + countTimelines(x + 1, y);
            break;
        }
    }

    memo[initial_y][x] = count;
    return count;
}

int main() {

    input = readCharMatrix("data/day7_input.txt");
    sizeX = input[0].size();
    sizeY = input.size();

    long long count = 0;
    //PART 1
    /*
    for (int i = 0; i < input[0].size(); i++) {
        if (input[0][i] == 'S') count = travelDown(i, 0);
    }
    */

    //PART 2
    memo.assign(sizeY, vector<long long>(sizeX, -1));
    for (int i = 0; i < input[0].size(); i++) {
        if (input[0][i] == 'S') count = countTimelines(i, 0);
    }

    cout<<count<<endl;

    return 0;

}