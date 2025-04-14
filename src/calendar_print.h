#pragma once

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

std::string PrintMonth(int year, int month);

constexpr bool isLeapYear(int year);

constexpr int getDaysInMonth(int year, int month);

// Возвращает день недели (0 = понедельник, 6 = воскресенье)
int getDayOfWeek(int year, int month, int day = 1);