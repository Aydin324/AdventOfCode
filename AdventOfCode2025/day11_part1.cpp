//
// Created by Sull on 12/11/2025.
//
#include <iostream>
#include <sstream>
#include "utils/input_reader.h"

using namespace std;

struct Device {
    string name;
    vector<string> connected_to;
};

vector<Device> devices;

Device& findDevice(string s) {
    for (Device& device : devices) {
        if (device.name == s) return device;
    }
}

long long findPaths(Device d) {
    long long count = 0;

    for (string& s : d.connected_to) {
        if (s == "out") return 1;
        count += findPaths(findDevice(s));
    }

    return count;
}

int main() {
    vector<string> input = readLines("data/day11_input.txt");

    for (string& s : input) {
        Device d;
        int index = s.find(':');
        d.name = s.substr(0, index);
        stringstream ss(s);
        string temp;
        getline(ss, temp, ' ');
        while (getline(ss, temp, ' ')) {
            d.connected_to.push_back(temp);
        }
        devices.push_back(d);
    }

    //find you
    long long count = 0;
    for (Device& d : devices) {
        if (d.name == "you") count = findPaths(d);
    }

    cout<<count<<endl;

    return 0;

}