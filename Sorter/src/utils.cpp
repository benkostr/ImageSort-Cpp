#include "utils.h"

namespace utils {


bool isLeapYear(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}


std::vector<std::string> &split_string(const std::string &s, std::string delims, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string line, item;
    std::size_t prev, pos;
    while (std::getline(ss, line))
    {
        prev = 0;
        while ((pos = line.find_first_of(delims, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                item = line.substr(prev, pos-prev);
                if (!item.empty())
                    elems.push_back(item);
            }
            prev = pos + 1;
        }
        if (prev < line.length())
            elems.push_back(line.substr(prev, std::string::npos));
    }
    return elems;
}


std::vector<std::string> split_string(const std::string &s, std::string delims)
{
    std::vector<std::string> elems;
    split_string(s, delims, elems);
    return elems;
}


}