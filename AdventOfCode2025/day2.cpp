#include <iostream>
#include <sstream>

#include "utils/input_reader.h"

using namespace std;

//PART 1
long long validateToken(string token) {
    size_t pos = token.find('-');
    long long a = stoll(token.substr(0, pos));
    long long b = stoll(token.substr(pos + 1));
    long long sum = 0;

    for (long long i = a; i <= b; i++) {
        string number = to_string(i);
        int num_of_digits = number.length();

        if (num_of_digits % 2 != 0) continue;

        int half_index = num_of_digits / 2;
        if (number.substr(0, half_index) == number.substr(half_index)) {
            sum += i;
        }
    }

    return sum;
}

//PART 2
long long validateTokenComplex(string token) {
    size_t pos = token.find('-');
    long long a = stoll(token.substr(0, pos));
    long long b = stoll(token.substr(pos + 1));
    long long sum = 0;

    for (long long i = a; i <= b; i++) {
        string number = to_string(i);
        int len = number.length();
        for (int pattern_len = 1; pattern_len <= len/2; pattern_len++) {
            if (len%pattern_len != 0) continue;
            int index = 0;
            bool found = true;
            while (index < len-pattern_len) {
                found = number.substr(index, pattern_len) == number.substr(index+pattern_len, pattern_len);
                if (!found) break;
                index += pattern_len;
            }
            if (found) {
                sum += i;
                break;
            }
        }
    }

    return sum;
}

int main() {
    string input = readString("data/day2_input.txt");

    string segment;
    stringstream ss(input);
    vector<string> tokens;
    while (getline(ss, segment, ',')) {
        tokens.push_back(segment);
    }

    cout<<tokens.size()<<endl;
    long long sum = 0;
    for (string token : tokens) {
        //sum += validateToken(token);
        sum += validateTokenComplex(token);
    }

    cout<<"Sum: "<<sum<<endl;

    return 0;
}