//
// Created by Sull on 12/12/2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <stdexcept>
#include "utils/input_reader.h"

using namespace std;

struct Point {
    int r, c;
    bool operator<(const Point& other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }
};
using Shape = vector<Point>;

struct Region {
    int w, h;
    vector<int> pieceCounts;
};

vector<Shape> base_shapes;

Shape absolute(const Shape& shape) {
    if (shape.empty()) return shape;
    int minR = shape[0].r;
    int minC = shape[0].c;

    for (const auto& p : shape) {
        minR = min(minR, p.r);
        minC = min(minC, p.c);
    }

    Shape absolute_shape;
    for (const auto& p : shape) absolute_shape.push_back({p.r - minR, p.c - minC});
    sort(absolute_shape.begin(), absolute_shape.end());
    return absolute_shape;
}

vector<Shape> findUniqueVariations(const Shape& shape) {
    set<Shape> uniqueVariations;
    Shape current = shape;

    for (int i = 0; i < 4; ++i) {
        uniqueVariations.insert(absolute(current));

        Shape flipped;
        for (const auto& p : current) flipped.push_back({p.r, -p.c});
        uniqueVariations.insert(absolute(flipped));
        Shape next;
        for (const auto& p : current) next.push_back({p.c, -p.r});
        current = next;
    }
    return vector<Shape>(uniqueVariations.begin(), uniqueVariations.end());
}

void placeShape(vector<vector<bool>>& grid, const Shape& shape, int start_r, int start_c, bool presence) {
    for (const auto& p : shape) grid[start_r + p.r][start_c + p.c] = presence;
}

bool doesFit(const vector<vector<bool>>& grid, const Shape& shape, int start_r, int start_c, int H, int W) {
    for (const auto& p : shape) {
        int nr = start_r + p.r;
        int nc = start_c + p.c;
        if (nr < 0 || nr >= H || nc < 0 || nc >= W || grid[nr][nc]) return false;
    }
    return true;
}

bool fits(vector<vector<bool>>& grid, const vector<int>& yet_to_fit, int pieceIdx, const vector<vector<Shape>>& shapes_variations, int h, int w) {
    if (pieceIdx == yet_to_fit.size()) return true;

    int current_id = yet_to_fit[pieceIdx];
    const vector<Shape>& variations = shapes_variations[current_id];
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            for (const auto& var : variations) {
                if (doesFit(grid, var, r, c, h, w)) {
                    placeShape(grid, var, r, c, true);
                    if (fits(grid, yet_to_fit, pieceIdx + 1, shapes_variations, h, w)) return true;
                    placeShape(grid, var, r, c, false);
                }
            }
        }
    }
    return false;
}

void parseInput(const vector<string>& lines, vector<Region>& regions) {
    vector<string> currentShapeLines;

    for (string line : lines) {
        if (line.find('x') != string::npos && line.find(':') != string::npos) {
            stringstream ss(line);
            string segment;
            Region reg;
            getline(ss, segment, 'x'); reg.w = stoi(segment);
            getline(ss, segment, ':'); reg.h = stoi(segment);

            int count;
            while (ss >> count) {
                reg.pieceCounts.push_back(count);
            }
            regions.push_back(reg);

            if (!currentShapeLines.empty()) {
                Shape newShape;
                for (int r = 0; r < currentShapeLines.size(); ++r) {
                    for (int c = 0; c < currentShapeLines[r].length(); ++c) {
                        if (currentShapeLines[r][c] == '#') newShape.push_back({r, c});
                    }
                }
                if (!newShape.empty()) base_shapes.push_back(newShape);
                currentShapeLines.clear();
            }
        }
        else if (line.empty() || (line.find(':') != string::npos)) {
            if (!currentShapeLines.empty()) {
                Shape newShape;
                for (int r = 0; r < currentShapeLines.size(); ++r) {
                    for (int c = 0; c < currentShapeLines[r].length(); ++c) {
                        if (currentShapeLines[r][c] == '#') newShape.push_back({r, c});
                    }
                }
                if (!newShape.empty()) base_shapes.push_back(newShape);
                currentShapeLines.clear();
            }
        }
        else currentShapeLines.push_back(line);
    }

    if (!currentShapeLines.empty()) {
        Shape newShape;
        for (int r = 0; r < currentShapeLines.size(); ++r) {
            for (int c = 0; c < currentShapeLines[r].length(); ++c) {
                if (currentShapeLines[r][c] == '#') newShape.push_back({r, c});
            }
        }
        if (!newShape.empty()) base_shapes.push_back(newShape);
    }
}

int main() {
    vector<string> lines = readLines("data/day12_input.txt");
    vector<Region> regions;
    parseInput(lines, regions);

    vector<vector<Shape>> shapes_variations;
    for (const auto& shape : base_shapes) shapes_variations.push_back(findUniqueVariations(shape));

    int counter = 0;
    int tracker = 0;
    for (const auto& region : regions) {
        cout << tracker++ << endl;

        vector<int> yet_to_fit;
        long long total_area = 0;
        for (int i = 0; i < region.pieceCounts.size(); ++i) {
            int count = region.pieceCounts[i];
            if (i < base_shapes.size()) {
                for (int k = 0; k < count; ++k) yet_to_fit.push_back(i);
                total_area += (long long)count * base_shapes[i].size();
            }
        }

        //early exit
        long long gridArea = (long long)region.h * region.w;
        if (total_area > gridArea) continue;

        sort(yet_to_fit.begin(), yet_to_fit.end(), [&](int a, int b) {
            return base_shapes[a].size() > base_shapes[b].size();
        });
        vector<vector<bool>> grid(region.h, vector<bool>(region.w, false));
        if (fits(grid, yet_to_fit, 0, shapes_variations, region.h, region.w)) counter++;
    }

    cout << "Counter: " << counter << endl;

    return 0;
}