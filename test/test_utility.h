#pragma once

#include <ctime>

inline std::tm MakeTm(int year, int month, int day, int hour, int minute, int second) {
    std::tm tm = { 0 };
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    std::mktime(&tm);
    return tm;
}


inline std::time_t MakeTime(int year, int month, int day, int hour, int minute, int second) {
    auto tm = MakeTm(year, month, day, hour, minute, second);
    return std::mktime(&tm);
}
