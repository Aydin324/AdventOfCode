//
// Created by Sull on 11/18/2025.
//
#include <iostream>
#include <unordered_set>
#include <vector>
#include "utils/input_reader.h"
using namespace std;

int guardX = 0, guardY = 0;
int sizeX, sizeY;
int orientation = 0; //{0,1,2,3} -> {up, right, down, left}
int stepsX[] = {0,1,0,-1};
int stepsY[] = {-1,0,1,0};

void changeOrientation() {
    if (orientation == 3) orientation = 0;
    else orientation++;
}

//part 1
char peekNext(vector<vector<char>>& map) {
    if ((guardX < sizeX - 1 && guardY < sizeY - 1) && (guardX > 0 && guardY > 0))
        return map[guardY+stepsY[orientation]][guardX+stepsX[orientation]];
    return '!';
}

void moveAhead(vector<vector<char>>& map) {
    guardX = guardX + stepsX[orientation];
    guardY = guardY + stepsY[orientation];
}

int followGuard(vector<vector<char>>& map) {
    int counter = 0;
    while (guardX < sizeX && guardY < sizeY && guardX > -1 && guardY > -1) {
        map[guardY][guardX] = 'X';
        char ahead = peekNext(map);
        if (ahead == '#') changeOrientation();
        else if (ahead == 'X') moveAhead(map);
        else {
            counter++;
            moveAhead(map);
        }
    }
    return counter;
}

//PART 2
inline void hash_combine(size_t& a, size_t b) {
    a ^= b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2);
}

struct Triple {
    int x, y, o;

    bool operator==(const Triple& rhs) const {
        return rhs.x == x && rhs.y == y && rhs.o == o;
    }
};

struct TripleHash {
    size_t operator()(const Triple& t) const noexcept{
        size_t a = hash<int>()(t.x);
        hash_combine(a, hash<int>()(t.y));
        hash_combine(a, hash<int>()(t.o));
        return a;
    }
};

struct Guard {
    int x, y;
    int startingX, startingY;
};

char peekAheadOfGuard(vector<vector<char>>& map, Guard& guard) {
    if ((guard.x < sizeX - 1 && guard.y < sizeY - 1) && (guard.x > 0 && guard.y > 0))
        return map[guard.y+stepsY[orientation]][guard.x+stepsX[orientation]];
    return '!';
}

void moveGuard(vector<vector<char>>& map, Guard& guard) {
    guard.x += stepsX[orientation];
    guard.y += stepsY[orientation];
}

void markGuardPath(vector<vector<char>>& map, Guard& guard) {
    guard.x = guard.startingX;
    guard.y = guard.startingY;
    while (guard.x < sizeX && guard.y < sizeY && guard.x > -1 && guard.y > -1) {
        map[guard.y][guard.x] = 'X';
        char ahead = peekAheadOfGuard(map, guard);
        if (ahead == '#') changeOrientation();
        else {
            moveGuard(map, guard);
        }
    }
}

bool checkLoop(vector<vector<char>>& map, Guard& guard, int x, int y) {
    map[y][x] = '#';
    guard.x = guard.startingX;
    guard.y = guard.startingY;
    orientation = 0;

    unordered_set<Triple, TripleHash> set;

    while (guard.x < sizeX && guard.y < sizeY && guard.x > -1 && guard.y > -1) {
        auto r = set.emplace(Triple{guard.x, guard.y, orientation});
        if (!r.second) {
            map[y][x] = '.';
            return true;
        };
        char ahead = peekAheadOfGuard(map, guard);
        if (ahead == '#') changeOrientation();
        else {
            moveGuard(map, guard);
        }
    }

    map[y][x] = '.';
    return false;

}

int main() {
    vector<vector<char> > map = readCharMatrix("data/day6_input.txt");

    //initialize map size
    sizeY = map.size();
    sizeX = map[0].size();

    //find the guard
    /*
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (map[y][x] == '^') {
                guardX = x;
                guardY = y;
            }
        }
    }

    int counter = followGuard(map);
    */

    Guard guard;

    //find the guard
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (map[y][x] == '^') {
                guard.startingX = x;
                guard.startingY = y;
            }
        }
    }

    //mark guard's path
    markGuardPath(map, guard);

    int loopCounter = 0;
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (map[y][x] == 'X' && checkLoop(map, guard, x, y)) loopCounter++;
        }
    }

    cout<<"loopCounter: "<<loopCounter<<endl;

    return 0;
}
