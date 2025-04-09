#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string PrintMonth(int year, int month);

bool isLeapYear(int year);

int getDaysInMonth(int year, int month);

// Возвращает день недели (0 = понедельник, 6 = воскресенье)
int getStartDayOfWeekMondayFirst(int year, int month);