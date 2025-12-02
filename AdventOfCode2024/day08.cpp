//
// Created by Sull on 11/23/2025.
//
#include <iostream>
#include "utils/input_reader.h"

using namespace std;

struct Point {
    int x,y;

    pair<Point,Point> find_antinodes_with_point(Point point2){
        pair<Point, Point> antinodes;
        int _x = point2.x - x;
        int _y = point2.y - y;

        antinodes.first = Point(x-_x, y-_y);
        antinodes.second = Point(point2.x+_x, point2.y+_y);

        return antinodes;
    }

    static pair<Point,Point> find_antinodes(pair<Point,Point> combination_pair){
        pair<Point, Point> antinodes;
        int _x = combination_pair.second.x - combination_pair.first.x;
        int _y = combination_pair.second.y - combination_pair.first.y;

        antinodes.first = Point(combination_pair.first.x-_x, combination_pair.first.y-_y);
        antinodes.second = Point(combination_pair.second.x+_x, combination_pair.second.y+_y);

        return antinodes;
    }

    static vector<Point> find_linear_antinodes(pair<Point,Point> combination_pair, int sizeX, int sizeY) {
        vector<Point> antinodes;
        int _x = combination_pair.second.x - combination_pair.first.x;
        int _y = combination_pair.second.y - combination_pair.first.y;

        int temp_x = combination_pair.first.x, temp_y = combination_pair.first.y;
        while (temp_x > -1 && temp_y > -1 && temp_x < sizeX && temp_y < sizeY) {
            antinodes.emplace_back(Point(temp_x, temp_y));
            temp_x = temp_x - _x;
            temp_y = temp_y - _y;
        }

        temp_x = combination_pair.second.x, temp_y = combination_pair.second.y;
        while (temp_x < sizeX && temp_y < sizeY && temp_x > -1 && temp_y > -1) {
            antinodes.emplace_back(Point(temp_x, temp_y));
            temp_x = temp_x + _x;
            temp_y = temp_y + _y;
        }

        return antinodes;
    }

    bool isNull(Point point) {
        if (point.x < 0 || point.y < 0) return true;
        return false;
    }

    bool isValid(int sizex, int sizey) const {
        if (x < 0 || y < 0 || x > sizex-1 || y > sizey-1) return false;
        return true;
    }

    bool operator<(const Point& point) const {
        if (y<point.y) return true;
        if (y == point.y && x < point.x) return true;
        return false;
    }
};

class VectorCombinator {
    private:
    vector<Point> points;
    int a=0, b=1;

    public:
    VectorCombinator(vector<Point> points) : points(points) {}

    pair<Point, Point> operator()() {
        if (a>=points.size()-1) throw out_of_range("Ended combinations");

        int _a = a;
        int _b = b;

        if (b==points.size()-1) {
            ++a;
            b = a+1;
        } else
            ++b;

        return pair<Point, Point>(points[_a], points[_b]);
    }

    bool ended() const {
        if (a>=points.size()-1) return true;
        return false;
    }
};

namespace AntinodeDetection {
    static set<Point> antinodes;

    void findAntinodes(const vector<Point>& points, int sizeX, int sizeY) {
        //iterate over pairs
        //check each pair
        VectorCombinator combinator(points);
        while (!combinator.ended()) {
            pair<Point, Point> combination_pair = combinator();
            combination_pair = Point::find_antinodes(combination_pair);
            if (combination_pair.first.isValid(sizeX, sizeY))
                antinodes.insert(combination_pair.first);
            if (combination_pair.second.isValid(sizeX, sizeY))
                antinodes.insert(combination_pair.second);
        }
    }

    void findLinearAntinodes(const vector<Point>& points, int sizeX, int sizeY) {
        VectorCombinator combinator(points);
        while (!combinator.ended()) {
            pair<Point, Point> combination_pair = combinator();
            vector<Point> pair_antinodes = Point::find_linear_antinodes(combination_pair, sizeX, sizeY);
            for (const Point& p : pair_antinodes) {
               antinodes.insert(p);
            }
        }
    }
}



int main() {
    vector<vector<char>> matrix = readCharMatrix("data/day8_input.txt");
    int sizeX = matrix[0].size();
    int sizeY = matrix.size();

    map<char, vector<Point>> map;

    //map all antennae
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (matrix[y][x] != '.') {
                map[matrix[y][x]].push_back({x, y});
            }
        }
    }

    for (const auto& [key, points] : map) {
        //AntinodeDetection::findAntinodes(points, sizeX, sizeY);
        AntinodeDetection::findLinearAntinodes(points, sizeX, sizeY);
    }



    cout<<"Number of antinodes: "<<AntinodeDetection::antinodes.size()<<endl;

    return 0;
}
