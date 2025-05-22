#include "calendar_print.h"

constexpr bool isLeapYear(int year) { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }

constexpr int getDaysInMonth(int year, int month) {
    constexpr std::array<int, 12> daysInMonth{{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    return (month == 2 && isLeapYear(year)) ? 29 : daysInMonth[month - 1];
}

// Возвращает день недели (0 = понедельник, 6 = воскресенье)
int getDayOfWeek(int year, int month, int day) {
    std::tm time_in = {};
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = day;

    std::mktime(&time_in);

    int wday = time_in.tm_wday;
    return (wday == 0) ? 6 : wday - 1;
}

std::string PrintMonth(int year, int month) {
    constexpr std::array<std::string_view, 12> monthNames = {"Январь",   "Февраль", "Март",   "Апрель",
                                                             "Май",      "Июнь",    "Июль",   "Август",
                                                             "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};

    std::ostringstream result;
    result << "     " << monthNames[month - 1] << " " << year << "\n";
    result << "Пн Вт Ср Чт Пт Сб Вс\n";

    int startDay = getDayOfWeek(year, month);
    int daysInMonth = getDaysInMonth(year, month);

    for (int i = 0; i < startDay; ++i) {
        result << "   ";
    }

    for (int i = startDay, currentDay = 1; currentDay <= daysInMonth; ++i) {
        result << std::setw(2) << currentDay++ << " ";
        if (i % 7 == 6) result << "\n";
    }
    result << "\n";
    return result.str();
}

std::string PrintYear(int year) {
    std::ostringstream result;
    for (int month = 1; month <= 12; ++month) {
        result << PrintMonth(year, month) << "\n";
    }
    return result.str();
}