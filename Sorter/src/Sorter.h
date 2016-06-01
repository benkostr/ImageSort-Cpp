#ifndef SORTER_H
#define SORTER_H
#pragma once

#include <vector>
#include <string>

#if defined(WIN32) || defined(_WIN32)
#ifdef SORTER_EXPORTS
#define SORTER_API __declspec(dllexport)
#else
#define SORTER_API __declspec(dllimport)
#endif
#endif

class Sorter
{
public:
    SORTER_API Sorter(std::string sourcePath, std::string outDir);
    SORTER_API ~Sorter();
    SORTER_API void Start();
    bool move, sortNonExif, separateNonExif, sortByYear, sortByMonth, sortByDay,
        sortByHour, sortByMinute, sortBySecond, abbreviateMonths, abort_flag,
        completed;
    float progress;
    std::string sourcePath, outDir;
private:
    typedef struct datetime
    {
        std::string year;
        std::string abbreviatedMonth;
        std::string fullMonth;
        std::string dayOfYear;
        std::string dayOfMonth;
        std::string hour;
        std::string minute;
        std::string second;
    } datetime;
    const char *months[12] =
    {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };
    const int daysToMonth[2][12] =
    {
        { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
        { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 },
    };
    datetime FormatDate(const std::string &date, const std::string &make, const std::string &model);
    std::string ParseEXIF(const easyexif::EXIFInfo &imageInfo, std::string &dateType);
    std::string ParseNonEXIF(const std::tr2::sys::path &file, std::string &dateType);
    void Sort(std::vector<std::string> &files);
    std::vector<std::thread> threads;
    unsigned long int numFiles;
    unsigned short int numThreads;
};

#endif // SORTER_H
