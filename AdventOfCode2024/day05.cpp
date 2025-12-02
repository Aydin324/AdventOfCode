//
// Created by Sull on 11/15/2025.
//
#include <algorithm>
#include <iostream>
#include <sstream>
#include <bits/ranges_algo.h>

#include "utils/input_reader.h"

using namespace std;

//helper method
void swapElements(int a, int b, vector<int>& v) {
    int temp = v[a];
    v[a] = v[b];
    v[b] = temp;
}

//PART 1
int checkUpdate(string update, map<int, set<int>>& map) {
    set<int> previous_updates;
    vector<int> updates;

    //feed update into a set
    stringstream ss(update);
    string temp;
    while (getline(ss, temp, ',')) {
        updates.push_back(stoi(temp));
    }

    //check if update is valid
    previous_updates.insert(updates[0]);
    set<int> intersection;
    for (int i=1; i<updates.size(); i++) {
        set_intersection(previous_updates.begin(), previous_updates.end(),
                                 map[updates[i]].begin(), map[updates[i]].end(),
                                 inserter(intersection, intersection.begin()));
        if (intersection.size() > 0) return 0;
        previous_updates.insert(updates[i]);
    }

    //return middle element
    return updates[updates.size()/2];
};

//PART 2
int sortUpdate(string update, map<int, set<int>>& map) {
    set<int> previous_updates;
    vector<int> updates;

    //feed update into a set
    stringstream ss(update);
    string temp;
    while (getline(ss, temp, ',')) {
        updates.push_back(stoi(temp));
    }

    //check if update is valid
    previous_updates.insert(updates[0]);
    set<int> intersection;
    bool valid = true;
    for (int i=1; i<updates.size(); i++) {
        intersection.clear();
        set_intersection(previous_updates.begin(), previous_updates.end(),
                                 map[updates[i]].begin(), map[updates[i]].end(),
                                 inserter(intersection, intersection.begin()));
        if (intersection.size() > 0) {
            valid = false;
            break;
        }
        previous_updates.insert(updates[i]);
    }

    if (valid) return 0;

    for (int i=updates.size()-1; i>0; i--) {
        for (int j=0; j<i; j++) {
            if (map[updates[i]].contains(updates[j])) {
                swapElements(i, j, updates);
                i++;
                break;
            }
        }
    }

    cout<<"Sorted incorrect update: ";
    for (int u : updates) cout<<u<<" ";
    cout<<endl;

    return updates[updates.size()/2];
}


int main() {
    vector<string> updates;
    map<int, set<int>> map = readMap("data/day5_input.txt", updates);

    int sum = 0;
    for (const string& s: updates) {
        //sum += checkUpdate(s, map);
        sum += sortUpdate(s, map);
    }

    cout<< sum;


    return 0;
}