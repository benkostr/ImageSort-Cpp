#ifndef LOGGER_H
#define LOGGER_H
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

namespace logger {

static bool threadsafe = true;
std::mutex mtx;
static std::ofstream stream("log.txt");

static void print(const std::string &msg)
{
    if (threadsafe)
        mtx.lock();
    stream << msg << std::endl;
    if (threadsafe)
        mtx.unlock();
}

}

#endif // LOGGER_H
