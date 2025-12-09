//
// Created by Sull on 12/9/2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include "utils/input_reader.h"

using namespace std;

struct Point {
    long long x, y;
};

vector<Point> parseInput(const vector<string>& lines) {
    vector<Point> tiles;
    for (const string& line : lines) {
        if (line.empty()) continue;
        stringstream ss(line);
        string segment;
        long long x = 0, y = 0;
        if (getline(ss, segment, ',')) x = stoll(segment);
        if (getline(ss, segment, ',')) y = stoll(segment);
        tiles.push_back({x, y});
    }
    return tiles;
}

vector<long long> getUniqueCoords(const vector<Point>& points, bool isX) {
    set<long long> s;
    for (const auto& p : points) {
        s.insert(isX ? p.x : p.y);
    }
    return vector<long long>(s.begin(), s.end());
}

int getIdx(long long val, const vector<long long>& coords) {
    auto it = lower_bound(coords.begin(), coords.end(), val);
    int idx = distance(coords.begin(), it);
    return idx * 2 + 1;
}

int main() {
    vector<string> lines = readLines("data/day9_input.txt");

    vector<Point> redTiles = parseInput(lines);

    //PART 1
    /*
    long long maxArea = 0;
    int numTiles = redTiles.size();

    for (int i = 0; i < numTiles; ++i) {
        for (int j = i + 1; j < numTiles; ++j) {
            const Point& t1 = redTiles[i];
            const Point& t2 = redTiles[j];

            long long width_tiles = abs(t1.x - t2.x) + 1;
            long long height_tiles = abs(t1.y - t2.y) + 1;

            long long currentArea = width_tiles * height_tiles;

            if (currentArea > maxArea) {
                maxArea = currentArea;
            }
        }
    }

    cout << "Largest rectangle area: " << maxArea << endl;
    */

    //PART 2
    vector<long long> uniqueX = getUniqueCoords(redTiles, true);
    vector<long long> uniqueY = getUniqueCoords(redTiles, false);

    //compressed grid dimensions
    int width = uniqueX.size() * 2 + 1;
    int height = uniqueY.size() * 2 + 1;

    //compressed grid, 0 = empty, 1 = border/interior, 2 = exterior
    vector<vector<int>> grid(height, vector<int>(width, 0));

    //draw boundaries
    int N = redTiles.size();
    for (int i = 0; i < N; ++i) {
        Point p1 = redTiles[i];
        Point p2 = redTiles[(i + 1) % N];

        int x1 = getIdx(p1.x, uniqueX);
        int y1 = getIdx(p1.y, uniqueY);
        int x2 = getIdx(p2.x, uniqueX);
        int y2 = getIdx(p2.y, uniqueY);

        if (x1 == x2) { // Vertical Segment
            for (int y = min(y1, y2); y <= max(y1, y2); ++y) grid[y][x1] = 1;
        } else { // Horizontal Segment
            for (int x = min(x1, x2); x <= max(x1, x2); ++x) grid[y1][x] = 1;
        }
    }

    //mark outside
    queue<pair<int, int>> q;
    q.push({0, 0});
    grid[0][0] = 2;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (grid[ny][nx] == 0) {
                    grid[ny][nx] = 2;
                    q.push({nx, ny});
                }
            }
        }
    }

    //prefix sum
    vector<vector<int>> prefix(height, vector<int>(width, 0));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int isValid = (grid[y][x] != 2) ? 1 : 0;
            int top = (y > 0) ? prefix[y - 1][x] : 0;
            int left = (x > 0) ? prefix[y][x - 1] : 0;
            int diag = (y > 0 && x > 0) ? prefix[y - 1][x - 1] : 0;
            prefix[y][x] = isValid + top + left - diag;
        }
    }

    auto getValidCount = [&](int x1, int y1, int x2, int y2) {
        int res = prefix[y2][x2];
        if (y1 > 0) res -= prefix[y1 - 1][x2];
        if (x1 > 0) res -= prefix[y2][x1 - 1];
        if (x1 > 0 && y1 > 0) res += prefix[y1 - 1][x1 - 1];
        return res;
    };

    //check all pairs
    long long maxArea = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            Point p1 = redTiles[i];
            Point p2 = redTiles[j];

            int cx1 = getIdx(p1.x, uniqueX);
            int cy1 = getIdx(p1.y, uniqueY);
            int cx2 = getIdx(p2.x, uniqueX);
            int cy2 = getIdx(p2.y, uniqueY);

            int lx = min(cx1, cx2);
            int rx = max(cx1, cx2);
            int ly = min(cy1, cy2);
            int ry = max(cy1, cy2);

            //check validity
            int totalCells = (rx - lx + 1) * (ry - ly + 1);
            int validCells = getValidCount(lx, ly, rx, ry);

            if (totalCells == validCells) {
                long long w = abs(p1.x - p2.x) + 1;
                long long h = abs(p1.y - p2.y) + 1;
                long long area = w * h;
                if (area > maxArea) {
                    maxArea = area;
                }
            }
        }
    }

    cout << "Part 2 largest rectangle area: " << maxArea << endl;

    return 0;
}