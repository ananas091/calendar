#include "calendar_print.h"

bool isLeapYear(int year) { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }

int getDaysInMonth(int year, int month) {
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return daysInMonth[month - 1];
}

// Возвращает день недели (0 = понедельник, 6 = воскресенье)
int getStartDayOfWeekMondayFirst(int year, int month) {
    std::tm time_in = {};
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = 1;
    std::mktime(&time_in);

    int wday = time_in.tm_wday;
    return (wday == 0) ? 6 : wday - 1;
}

std::string PrintMonth(int year, int month) {
    static const char* monthNames[] = {"",     "Январь", "Февраль",  "Март",    "Апрель", "Май",    "Июнь",
                                       "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};

    std::ostringstream result;
    result << "     " << monthNames[month] << " " << year << "\n";
    result << "Пн Вт Ср Чт Пт Сб Вс\n";

    int startDay = getStartDayOfWeekMondayFirst(year, month);
    int daysInMonth = getDaysInMonth(year, month);

    int currentDay = 1;

    // Отступ перед началом месяца
    for (int i = 0; i < startDay; ++i) {
        result << "   ";
    }

    // Печать дней месяца
    for (int i = startDay; currentDay <= daysInMonth; ++i) {
        result << std::setw(2) << currentDay++ << " ";
        if (i % 7 == 6) result << "\n";
    }
    result << "\n";
    return result.str();
}