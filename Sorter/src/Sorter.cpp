#include "stdafx.h"
#include "Sorter.h"
#include "logger.h"

using namespace std;
namespace fs = std::tr2::sys;


Sorter::Sorter(string sourcePath, string outDir)
{
    abbreviateMonths = true;
    abort_flag       = false;
    completed        = false;
    move             = false;
    this->outDir     = outDir;
    progress         = 0.0f;
    sortNonExif      = true;
    separateNonExif  = true;
    sortByYear       = true;
    sortByMonth      = true;
    sortByDay        = false;
    sortByHour       = false;
    sortByMinute     = false;
    sortBySecond     = false;
    this->sourcePath = sourcePath;
}


Sorter::~Sorter()
{
}

void Sorter::Start()
{
    completed = false;
    string fileExt;
    vector<string> files;
    for (fs::recursive_directory_iterator it{ fs::path(sourcePath) }, end; it != end; ++it)
    {
        if (fs::is_regular_file(it->status()))
        {
            fileExt = it->path().extension().string();
            transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
            if (fileExt == ".jpg" || fileExt == ".jpeg" || fileExt == ".png" ||
                fileExt == ".bmp" || fileExt == ".dib")
            {
                files.push_back(it->path().string());
            }
        }
    }
    numFiles = files.size();
    unsigned short int numProcs = thread::hardware_concurrency();
    numThreads = (numProcs <= numFiles) ? numProcs : numFiles;
    vector<string*> splitIndices(numThreads);
    unsigned long splitLength = numFiles / numThreads;
    unsigned short remainder = numFiles % numThreads;
    // partition files for threads
    for (int i = 0, k = 0; i < numThreads; ++i)
    {
        if (k >= numFiles)
            break;
        splitIndices.at(i) = &files[k];
        if (remainder > 0)
        {
            k += splitLength + 1;
            remainder -= 1;
        }
        else
            k += splitLength;
    }
    logger::print("Number of Files: " + to_string(numFiles));
    // start threads
    vector<string> filesPart;
    long numRanFiles = 0;
    for (vector<string*>::iterator it = splitIndices.begin(); it != splitIndices.end(); ++it)
    {
        if (*it != splitIndices.back())
            filesPart.assign(*it, *(it + 1));
        else
            filesPart.assign(*it, &files.back() + 1);
        numRanFiles += filesPart.size();
        threads.push_back(thread(&Sorter::Sort, this, filesPart));
    }
    for (int i = 0; i < threads.size(); ++i)
    {
        threads.at(i).join();
    }
    threads.clear();
    completed = true;
    logger::print("Number of Sorted Files: " + to_string(numRanFiles));
}

Sorter::datetime Sorter::FormatDate(const string &date, const string &make, const string &model)
{
    //Edit date to common format or parse into struct
    datetime dt;
    vector<string> splitDate = utils::split_string(date, " :.,");
    dt.year                  = splitDate.at(0);
    int month                = stoi(splitDate.at(1)) - 1;
    dt.fullMonth             = months[month];
    dt.abbreviatedMonth      = dt.fullMonth.substr(0,3);
    dt.dayOfMonth            = splitDate.at(2);
    if (dt.dayOfMonth[0] == '0')
        dt.dayOfMonth = dt.dayOfMonth[1];
    dt.dayOfYear = to_string(daysToMonth[utils::isLeapYear(stoi(dt.year)) ? 1 : 0][month] + stoi(dt.dayOfMonth));
    dt.hour      = splitDate.at(3);
    dt.minute    = splitDate.at(4);
    dt.second    = splitDate.at(5);

    return dt;
}

string Sorter::ParseEXIF(const easyexif::EXIFInfo &imageInfo, string &dateType)
{
    string date;
    // Get date
    if (!imageInfo.DateTimeOriginal.empty())
    {
        dateType = "original time";
        date = imageInfo.DateTimeOriginal;
    }
    else if (!imageInfo.DateTimeDigitized.empty())
    {
        dateType = "digitized time";
        date = imageInfo.DateTimeDigitized;
    }
    else
        date = "NonEXIF";

    return date;
}

string Sorter::ParseNonEXIF(const fs::path &file, string &dateType)
{
    dateType = "file modified time";
    auto ftime = fs::last_write_time(file);
    time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
    struct tm dateTime;
    localtime_s(&dateTime, &cftime);
    stringstream ss;
    ss << dateTime.tm_year + 1900 << ':'
        << setfill('0') << setw(2) << dateTime.tm_mon + 1 << ':'
        << setfill('0') << setw(2) << dateTime.tm_mday    << ' '
        << setfill('0') << setw(2) << dateTime.tm_hour    << ':'
        << setfill('0') << setw(2) << dateTime.tm_min     << ':'
        << setfill('0') << setw(2) << dateTime.tm_sec;

    return ss.str();
}

void Sorter::Sort(vector<string> &files)
{
    ifstream inFile;
    stringstream logss;
    unsigned char *buffer;
    unsigned long fsize;
    int parseCode;
    easyexif::EXIFInfo imageInfo;
    fs::path file, targetPath, destFile;
    string date, dateType;
    datetime dt;
    bool nonExif = false, moveSuccess;
    for (string fileString : files)
    {
        if (abort_flag)
            break;
        progress += 1.0f / numFiles;
        // Read file into a buffer
        inFile.open(fileString.c_str(), ios::binary|ios::ate);
        if (inFile.fail())
        {
            logger::print("Error: Cannot open file: " + fileString);
            continue;
        }
        fsize = inFile.tellg();
        inFile.seekg(0, inFile.beg);
        buffer = new unsigned char[fsize];
        inFile.read(reinterpret_cast<char*>(buffer), fsize);
        inFile.close();
        if (!inFile)
        {
            delete[] buffer;
            logger::print("Error: Cannot read file: " + fileString);
            continue;
        }
        // Parse EXIF data
        parseCode = imageInfo.parseFrom(buffer, fsize);
        delete[] buffer;
        targetPath = fs::path(outDir);
        file = fs::path(fileString);
        if (parseCode)
        {
            if (parseCode == PARSE_EXIF_ERROR_NO_EXIF ||
                parseCode == PARSE_EXIF_ERROR_NO_JPEG)
            {
                date = ParseNonEXIF(file, dateType);
                nonExif = true;
            }
            else
            {
                string msg;
                switch (parseCode)
                {
                    case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
                        msg = "Byte alignment unknown (not Motorola or Intel).";
                        break;
                    case PARSE_EXIF_ERROR_CORRUPT:
                        msg = "EXIF header was found, but data was corrupted.";
                        break;
                    default:
                        msg = "Unknown.";
                }
                logger::print("Error parsing EXIF for file: \"" + fileString + "\"\ninfo: " + msg);
                continue;
            }
        }
        else
        {
            date = ParseEXIF(imageInfo, dateType);
            if (date == "NonEXIF")
            {
                date = ParseNonEXIF(file, dateType);
                nonExif = true;
            }
        }
        if (nonExif)
        {
            nonExif = false;
            if (!sortNonExif)
                continue;
            else if (separateNonExif)
                targetPath /= "NonEXIF";
        }

        if (date.empty())
        {
            logger::print("Error: No date data: " + fileString);
            continue;
        }

        dt = FormatDate(date, imageInfo.Make, imageInfo.Model);
        logss << "Image: " << fs::path(fileString).filename().string() << endl
            << "Date Type: " << dateType << endl
            << "Date: " << date << endl
            << "SubSecTime: " << imageInfo.SubSecTimeOriginal << endl
            << "Make: " << imageInfo.Make << endl
            << "Model: " << imageInfo.Model << endl
            << "dt: " << dt.fullMonth << ", " << dt.abbreviatedMonth << ", "
            << dt.dayOfMonth << ", " << dt.dayOfYear << ", " << dt.year << ", "
            << dt.hour << ":" << dt.minute << ":" << dt.second << endl;
        logger::print(logss.str());
        stringstream().swap(logss);
        // Sort
        if (sortByYear)
            targetPath /= dt.year;
        if (sortByMonth)
        {
            if (abbreviateMonths)
                targetPath /= dt.abbreviatedMonth;
            else
                targetPath /= dt.fullMonth;
        }
        if (sortByDay)
        {
            if (sortByMonth)
                targetPath /= dt.dayOfMonth;
            else
                targetPath /= dt.dayOfYear;
        }
        if (sortByHour)
            targetPath /= dt.hour;
        if (sortByMinute)
            targetPath /= dt.minute;
        if (sortBySecond)
            targetPath /= dt.second;
        if (!fs::exists(targetPath) && !fs::create_directories(targetPath))
        {
            logger::print("Error: Cannot create directory tree: " + targetPath.string());
            break;
        }
        destFile = targetPath / file.filename();
        if (move)
        {
            if (fs::exists(destFile))
                fs::remove(destFile);
            fs::rename(file, destFile);
        }
        else if (!fs::exists(destFile))
        {
            moveSuccess = fs::copy_file(file, destFile);
            if (!moveSuccess)
            {
                logger::print("Error: failed to copy file: " + destFile.string());
            }
        }
    }
}
