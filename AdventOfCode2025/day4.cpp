//
// Created by Sull on 12/4/2025.
//
#include <iostream>
#include "utils/input_reader.h"

using namespace std;

//initial solution
bool canBeAccessed(vector<vector<char>>& matrix, int x, int y) {
    int count = 0;
    int sizeX = matrix[0].size();
    int sizeY = matrix.size();

    if (y-1 >= 0 && matrix[y-1][x] == '@') count++;
    if (y+1 < sizeY && matrix[y+1][x] == '@') count++;
    if (x-1 >= 0 && matrix[y][x-1] == '@') count++;
    if (x+1 < sizeX && matrix[y][x+1] == '@') count++;
    if (y-1 >= 0 && x-1 >= 0 && matrix[y-1][x-1] == '@') count++;
    if (y-1 >= 0 && x+1 < sizeX && matrix[y-1][x+1] == '@') count++;
    if (y+1 < sizeY && x-1 >= 0 && matrix[y+1][x-1] == '@') count++;
    if (y+1 < sizeY && x+1 < sizeX && matrix[y+1][x+1] == '@') count++;

    return count < 4;
}

//OPTIMIZED
class Storage {
private:
    vector<vector<char>> matrix;
    vector<vector<int>> neighbourCount;
    int sizeX, sizeY;

public:
    Storage(vector<vector<char>>& matrix) {
        this->matrix = matrix;
        this->sizeX = matrix[0].size();
        this->sizeY = matrix.size();
        neighbourCount.assign(sizeY, vector<int>(sizeX));
    }

    //PART 1
    int getNumOfAccessable() {
        //update neighbour counts
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                if (matrix[y][x] == '@') increaseNeighbourCount(x, y);
            }
        }

        int count = 0;
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                if (neighbourCount[y][x] < 4 && matrix[y][x] == '@') count++;
            }
        }

        return count;
    }

    //PART 2
    int getNumOfAccessableAdvanced() {
        //update neighbour counts
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                if (matrix[y][x] == '@') increaseNeighbourCount(x, y);
            }
        }

        int count = 0;
        bool changed = true;
        while (changed) {
            changed = false;
            for (int y = 0; y < sizeY; y++) {
                for (int x = 0; x < sizeX; x++) {
                    if (neighbourCount[y][x] < 4 && matrix[y][x] == '@') {
                        changed = true;
                        matrix[y][x] = '.';
                        decreaseNeighbourCount(x, y);
                        count++;
                    }
                }
            }
        }

        return count;
    }

    void increaseNeighbourCount(int x, int y) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dy == 0 && dx == 0) continue;
                if (x+dx<sizeX && y+dy<sizeY && x+dx>=0 && y+dy>=0) neighbourCount[y+dy][x+dx]++;
            }
        }
    }

    void decreaseNeighbourCount(int x, int y) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dy == 0 && dx == 0) continue;
                if (x+dx<sizeX && y+dy<sizeY && x+dx>=0 && y+dy>=0) neighbourCount[y+dy][x+dx]--;
            }
        }
    }
};

int main() {
    vector<vector<char>> input = readCharMatrix("data/day4_input.txt");
    vector<vector<char>> input2 = input; //for optimized version

    long long count = 0;

    //PART 1
    /*
    for (int y=0; y<input.size(); y++) {
        for (int x=0; x<input[0].size(); x++) {
            if (input[y][x] == '@' && canBeAccessed(input, x, y)) count++;
        }
    }
    */

    //PART 2
    bool removed = true;
    while (removed) {
        removed = false;
        for (int y=0; y<input.size(); y++) {
            for (int x=0; x<input[0].size(); x++) {
                if (input[y][x] == '@' && canBeAccessed(input, x, y)) {
                    count++;
                    input[y][x] = '.';
                    removed = true;
                }
            }
        }
    }

    cout<<count<<endl;

    //OPTIMIZED

    Storage storage(input2);
    //cout<<"\nOptimized part 1: "<<storage.getNumOfAccessable()<<endl;
    cout<<"\nOpzimized part 2: "<<storage.getNumOfAccessableAdvanced()<<endl;

    return 0;
}