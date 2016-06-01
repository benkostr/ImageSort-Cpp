#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace utils {

bool isLeapYear(int year);
std::vector<std::string> &split_string(const std::string &s, std::string delims, std::vector<std::string> &elems);
std::vector<std::string> split_string(const std::string &s, std::string delims);

}

#endif // UTILS_H
