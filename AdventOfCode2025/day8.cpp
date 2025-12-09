//
// Created by Sull on 12/8/2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <map>
#include "utils/input_reader.h"

using namespace std;

struct Junction {
    long long x, y, z;
};

struct Connection {
    int u, v;
    long long distanceSq;

    int u_x, v_x;

    bool operator<(const Connection& other) const {
        return distanceSq < other.distanceSq;
    }
};

//union-find
vector<int> parent;
vector<int> circuitSize;

int findSet(int i) {
    if (parent[i] == i)
        return i;
    // Path compression: set parent[i] directly to the root
    return parent[i] = findSet(parent[i]);
}

bool unionSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);

    if (a != b) {
        if (circuitSize[a] < circuitSize[b]) swap(a, b);
        parent[b] = a;
        circuitSize[a] += circuitSize[b];
        return true;
    }
    return false;
}

long long getDistanceSq(const Junction& a, const Junction& b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    long long dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

int main() {
    vector<string> lines = readLines("data/day8_input.txt");
    vector<Junction> junctions;

    for (const string& line : lines) {
        stringstream ss(line);
        string segment;
        Junction j;
        if (getline(ss, segment, ',')) j.x = stoll(segment);
        if (getline(ss, segment, ',')) j.y = stoll(segment);
        if (getline(ss, segment, ',')) j.z = stoll(segment);

        junctions.push_back(j);
    }

    int numJunctions = junctions.size();

    //make connections and sort
    vector<Connection> connections;
    for (int i = 0; i < numJunctions; i++) {
        for (int j = i + 1; j < numJunctions; j++) {
            connections.emplace_back( i, j, getDistanceSq(junctions[i], junctions[j]),
            junctions[i].x, junctions[j].x);
        }
    }
    sort(connections.begin(), connections.end());

    //init union-find
    parent.resize(numJunctions);
    circuitSize.resize(numJunctions, 1);
    for (int i = 0; i < numJunctions; i++) {
        parent[i] = i;
    }

    //PART 1
    /*
    for (int i = 0; i < 1000 && i < connections.size(); ++i) {
        const Connection& conn = connections[i];
        unionSets(conn.u, conn.v);
    }

    map<int, long long> finalCircuitSizes;
    for (int i = 0; i < numJunctions; i++) {
        int root = findSet(i);
        finalCircuitSizes[root] = circuitSize[root];
    }

    vector<long long> sizes;
    for (const auto& pair : finalCircuitSizes) {
        sizes.push_back(pair.second);
    }
    sort(sizes.rbegin(), sizes.rend());

    long long product = sizes[0] * sizes[1] * sizes[2];
    cout<<"Product: "<<product<<endl;
    */

    //PART 2
    int circuitCount = numJunctions;
    long long first_x = 1, second_x = -1;

    for (const Connection& conn : connections) {
        if (unionSets(conn.u, conn.v)) {
            circuitCount--;

            if (circuitCount == 1) {
                first_x = conn.u_x;
                second_x = conn.v_x;
                break;
            }
        }
    }

    long long finalAnswer = first_x * second_x;
    cout << "Last x coordinates: " << finalAnswer << endl;

    return 0;
}