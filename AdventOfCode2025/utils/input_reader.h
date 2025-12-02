// utils/input_reader.h
#ifndef INPUT_READER_H
#define INPUT_READER_H

#include <map>
#include <set>
#include <vector>
#include <string>

std::vector<std::string> readLines(const std::string& filename);
std::vector<std::vector<char>> readCharMatrix(const std::string& filename);
std::map<int, std::set<int>> readMap(const std::string& filename, std::vector<std::string>& updates);
std::string readString(const std::string& filename);
#endif // INPUT_READER_H