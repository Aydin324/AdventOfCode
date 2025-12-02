// utils/input_reader.cpp
#include "input_reader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> readLines(const std::string& filename) {
    std::ifstream file("../"+filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return {}; // Return an empty vector
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::vector<char>> readCharMatrix(const std::string& filename) {
    std::ifstream file("../"+filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<std::vector<char>> matrix;
    std::string line;
    while (std::getline(file, line)) {
        matrix.emplace_back(line.begin(), line.end());
    }

    return matrix;

}

std::map<int, std::set<int>> readMap(const std::string& filename, std::vector<std::string>& updates) {
    std::ifstream file("../"+filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return {};
    }

    std::map<int, std::set<int>> map;
    std::string line;
    getline(file, line);
    while (line != "") {
        std::pair<int, int> pair(std::stoi(line.substr(0,2)), std::stoi(line.substr(3,2)));
        map[pair.first].insert(pair.second);
        getline(file, line);
    }
    while (getline(file, line)) {
        updates.push_back(line);
    }

    return map;
}

std::string readString(const std::string& filename) {
    std::ifstream file("../"+filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
    }

    std::stringstream ss("");
    std::string line;
    while (std::getline(file, line)) {
        ss<<line;
    }

    return ss.str();
}