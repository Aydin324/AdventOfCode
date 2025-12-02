//
// Created by Sull on 11/13/2025.
//
#include <iostream>
#include "utils/input_reader.h"

using namespace std;

//PART 1
int checkSurroundings(const vector<vector<char>>& m, int row, int column) {
    int result = 0;
    string word = "XMAS";
    int max_row = m.size()-1;
    int max_column = m[0].size()-1;

    //define direction offset - starting clockwise from left-up
    int dr[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dc[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int direction=0; direction < 8; direction++) {
        bool found = true;
        int cr = row;
        int cy = column;

        for (int i = 1; i < 4; i++) {
            if (cr+dr[direction]<0 || cy+dc[direction]<0 || cr+dr[direction]>max_row || cy+dc[direction]>max_column) {
                found = false;
                break;
            }

            cr = cr+dr[direction];
            cy = cy+dc[direction];

            if (m[cr][cy]!=word[i]) {
                found = false;
                break;
            }
        }

        if (found) result++;
    }

    return result;
}

//PART 2
int checkXSurroundings(const vector<vector<char>>& m, int row, int column, char letter) {
    string word = (letter=='M') ? "MAS" : "SAM";
    bool found = true;

    int last_row = m.size()-1;
    int last_column = m[0].size()-1;

    //check bounds
    if (row+2>last_row || column+2>last_column) return 0;

    for (int i=1; i<3; i++) {
        row++;
        column++;
        if (m[row][column]!=word[i]) return 0;
    }
    if ((m[row-2][column]=='M' && m[row][column-2]=='S')
        || (m[row-2][column]=='S' && m[row][column-2]=='M')) return 1;

    return 0;
}

int main() {
    vector<vector<char>> matrix = readCharMatrix("data/day4_input.txt");

    int count = 0;
    for (int r=0; r<matrix.size(); r++) {
        for (int c = 0; c < matrix[0].size(); c++) {
            //if (matrix[r][c]=='X') count += checkSurroundings(matrix, r, c);
            if (matrix[r][c]=='M' || matrix[r][c]=='S') count += checkXSurroundings(matrix, r, c, matrix[r][c]);
        }
    }

    cout<<"Count: "<<count<<endl;

    return 0;
}