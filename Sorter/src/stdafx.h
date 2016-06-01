// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifdef _MSC_VER
#define _BIND_TO_CURRENT_VCLIBS_VERSION 1
#include "targetver.h"
#endif

#include <fstream> //read files
#include <iostream> //console log
#include <string>
#include <sstream> //parsing string
#include <iomanip> //parsing string
#include <vector>
#include <filesystem> //file manipulation
#include <thread> //multi-threading
#include <chrono>
#include <mutex> //locking for exclusive access from thread

#include "easyexif/exif.h"
#include "utils.h"
