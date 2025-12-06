//
// Created by Sull on 12/6/2025.
//
#include <iostream>
#include <sstream>

#include "utils/input_reader.h"

using namespace std;

class Reader {
    public:
    vector<vector<char>> _matrix;
    int opIndex;

    Reader(string path) {
        _matrix = readCharMatrix(path);
        opIndex = _matrix[4].size()-1;
    }

    //return {operation, index_where_found}
    pair<char, int> nextOpcode() {
        while (_matrix[4][opIndex] == ' ') opIndex--;
        char op = _matrix[4][opIndex];
        opIndex--;
        return make_pair(op, opIndex+1);
    }

    long long calculateColumn(pair<char, int> operation) {
        int index = operation.second;
        bool isAddittion;

        if (operation.first == '+') isAddittion = true;
        else isAddittion = false;

        //construct numbers
        bool detected = true;
        vector<int> numbers;
        while (detected) {
            detected = false;
            string number = "";
            int y = 0;
            while (y < 4) {
                if (index < _matrix[y].size() && _matrix[y][index] != ' ') {
                    number += _matrix[y][index];
                    detected = true;
                }
                y++;
            }
            index++;
            if (detected) numbers.push_back(stoi(number));
        }

        long long sum;
        if (isAddittion) {
            sum = 0;
            for (int n : numbers) {
                sum += n;
            }
        } else {
            sum = 1;
            for (int n : numbers) {
                sum *= n;
            }
        }

        return sum;
    }
};

int main() {
    //PART 1
    /*
    vector<string> lines = readLines("data/day6_input.txt");
    stringstream s1(lines[0]);
    stringstream s2(lines[1]);
    stringstream s3(lines[2]);
    stringstream s4(lines[3]);
    stringstream s5(lines[4]); // operator
    long long sum = 0;

    string op;
    while (s5>>op) {
        long long a, b, c, d;
        s1>>a;
        s2>>b;
        s3>>c;
        s4>>d;

        if (op == "+") {
            sum += a + b + c + d;
        } else {
            sum += (a * b * c * d);
        }
    }

    cout<<sum;
    */

    //PART 2
    Reader r("data/day6_input.txt");
    long long sum  = 0;
    while (r.opIndex > 0) sum += r.calculateColumn(r.nextOpcode());

    cout<<"\n Sum is: "<<sum<<endl;

    return 0;
}