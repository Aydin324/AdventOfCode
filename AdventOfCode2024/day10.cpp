//
// Created by Sull on 11/29/2025.
//
#include <iostream>
#include "utils/input_reader.h"

using namespace std;


class Finder {
public:
    vector<vector<int>> matrix;
    int sizeX, sizeY;
    int sum_of_scores = 0;
    int sum_of_ratings = 0;

    Finder(string& path) {
        matrix = readIntMatrix(path);
        sizeX = matrix[0].size();
        sizeY = matrix.size();
    }

    void seekPaths() {
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                if (matrix[y][x] == 0) {
                    set<pair<int, int>> points;
                    sum_of_scores += pathScore(0, x, y, points);
                }
            }
        }
    }

    int pathScore(int height, int x, int y, set<pair<int, int>>& points) {
        if (height == 9 && !points.contains(make_pair(x, y))) {
            points.insert(make_pair(x, y));
            return 1;
        }
        if (height == 9) return 0;

        int next_height = height + 1;
        int score = 0;

        //check up
        if (checkY(y-1) && this->matrix[y-1][x] == next_height) score += pathScore(next_height, x, y-1, points);

        //check right
        if (checkX(x+1) && this->matrix[y][x+1] == next_height) score += pathScore(next_height, x+1, y, points);

        //check down
        if (checkY(y+1) && this->matrix[y+1][x] == next_height) score += pathScore(next_height, x, y+1, points);

        //check left
        if (checkX(x-1) && this->matrix[y][x-1] == next_height) score += pathScore(next_height, x-1, y, points);

        return score;
    }

    bool checkY(int y) const {
        if (y < 0 || y >= this->sizeY) return false;
        return true;
    }

    bool checkX(int x) const {
        if (x < 0 || x >= this->sizeX) return false;
        return true;
    }

    int getSumOfScores() {
        return sum_of_scores;
    }

    //PART 2
    void seekRatings() {
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                if (matrix[y][x] == 0) {
                    sum_of_ratings += pathRating(0, x, y);
                }
            }
        }
    }

    int pathRating(int height, int x, int y) {
        if (height == 9) {
            return 1;
        }

        int next_height = height + 1;
        int score = 0;

        //check up
        if (checkY(y-1) && this->matrix[y-1][x] == next_height) score += pathRating(next_height, x, y-1);

        //check right
        if (checkX(x+1) && this->matrix[y][x+1] == next_height) score += pathRating(next_height, x+1, y);

        //check down
        if (checkY(y+1) && this->matrix[y+1][x] == next_height) score += pathRating(next_height, x, y+1);

        //check left
        if (checkX(x-1) && this->matrix[y][x-1] == next_height) score += pathRating(next_height, x-1, y);

        return score;
    }

    int getSumOfRatings() {
        return sum_of_ratings;
    }
};

int main() {
    string input = "data/day10_input.txt";
    Finder finder(input);

    //PART 1
    /*
    finder.seekPaths();
    cout<<finder.getSumOfScores();
    */

    //PART 2
    finder.seekRatings();
    cout<<finder.getSumOfRatings()<<endl;

    return 0;
}