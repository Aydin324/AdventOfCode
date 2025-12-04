//
// Created by Sull on 12/3/2025.
//
#include <iostream>

#include "utils/input_reader.h"

using namespace std;

class Farm {
    public:
    vector<vector<char>> farm;
    set<pair<int, int>> explored;
    int sizeX, sizeY;

    Farm(vector<vector<char>>& farm) {
        this->farm = farm;
        this->sizeX = farm[0].size();
        this->sizeY = farm.size();

    }

    //PART 1
    pair<int, int> findPrice(pair<int, int> p, char c, int x, int y) {
        int area = p.first, perimeter = p.second;
        explored.insert(make_pair(x, y));

        //check up
        if (y-1>=0 && farm[y-1][x] == c && !explored.contains(make_pair(x, y-1))) {
            pair<int, int> temp = findPrice(make_pair(1,0), c, x, y-1);
            area += temp.first;
            perimeter += temp.second;
        } else if (y-1<0 || farm[y-1][x] != c ) perimeter++;

        //check right
        if (x+1<sizeX && farm[y][x+1] == c && !explored.contains(make_pair(x+1, y))) {
            pair<int, int> temp = findPrice(make_pair(1,0), c, x+1, y);
            area += temp.first;
            perimeter += temp.second;
        } else if (x+1>=sizeX || farm[y][x+1] != c) perimeter++;

        //check down
        if (y+1<sizeY && farm[y+1][x] == c && !explored.contains(make_pair(x, y+1))) {
            pair<int, int> temp = findPrice(make_pair(1,0), c, x, y+1);
            area += temp.first;
            perimeter += temp.second;
        } else if (y+1>=sizeY || farm[y+1][x] != c) perimeter++;

        //check left
        if (x-1>=0 && farm[y][x-1] == c && !explored.contains(make_pair(x-1, y))) {
            pair<int, int> temp = findPrice(make_pair(1,0), c, x-1, y);
            area += temp.first;
            perimeter += temp.second;
        } else if (x-1<0 || farm[y][x-1] != c) perimeter++;

        return make_pair(area, perimeter);
    }

    //PART 2

    vector<pair<int, int>> findRegion(char c, int x, int y) {
        vector<pair<int, int>> region = {make_pair(x, y)};
        explored.insert(make_pair(x, y));

        //check up
        if (y-1>=0 && farm[y-1][x] == c && !explored.contains(make_pair(x, y-1))) {
            vector<pair<int, int>> temp = findRegion(c, x, y-1);
            region.insert(region.end(), temp.begin(), temp.end());
        }

        //check right
        if (x+1<sizeX && farm[y][x+1] == c && !explored.contains(make_pair(x+1, y))) {
            vector<pair<int, int>> temp = findRegion(c, x+1, y);
            region.insert(region.end(), temp.begin(), temp.end());
        }

        //check down
        if (y+1<sizeY && farm[y+1][x] == c && !explored.contains(make_pair(x, y+1))) {
            vector<pair<int, int>> temp = findRegion(c, x, y+1);
            region.insert(region.end(), temp.begin(), temp.end());
        }

        //check left
        if (x-1>=0 && farm[y][x-1] == c && !explored.contains(make_pair(x-1, y))) {
            vector<pair<int, int>> temp = findRegion(c, x-1, y);
            region.insert(region.end(), temp.begin(), temp.end());
        }

        return region;
    }

    int findRegionSides(vector<pair<int, int>>& region) {
        set<int> top;
        set<int> bottom;
        set<int> left;
        set<int> right;
        char c = farm[region[0].second][region[0].first];

        //for each cell check borders
        for (pair<int, int> p : region) {
            int x = p.first, y = p.second;

            //check up
            if (y-1<0 || farm[y-1][x] != c ) top.insert(y);
            //check right
            if (x+1>=sizeX || farm[y][x+1] != c) right.insert(x);
            //bottom
            if (y+1>=sizeY || farm[y+1][x] != c) bottom.insert(y);
            //left
            if (x-1<0 || farm[y][x-1] != c) left.insert(x);
        }

        return top.size() + bottom.size() + left.size() + right.size();
    }
};

int main() {
    vector<vector<char>> input = readCharMatrix("data/day12_test_input.txt");
    Farm farm(input);


    long long count = 0;

    /*
    for (int y = 0; y < farm.sizeY; y++) {
        for (int x = 0; x < farm.sizeX; x++) {
            if (!farm.explored.contains(make_pair(x, y))) {
                cout<<"Currently exploring "<<input[y][x]<<" from point(x,y): "<<x<<" "<<y<<endl;
                pair<int, int> temp = farm.findPrice(make_pair(1, 0), input[y][x], x, y);
                count += temp.first * temp.second;
            }
        }
    } */

    for (int y = 0; y < farm.sizeY; y++) {
        for (int x = 0; x < farm.sizeX; x++) {
            if (!farm.explored.contains(make_pair(x, y))) {
                vector<pair<int, int>> temp = farm.findRegion(input[y][x], x, y);
                int sides = farm.findRegionSides(temp);
                count += sides * temp.size();
            }
        }
    }

    cout<<"\nCOUNT: "<<count<<endl;

}