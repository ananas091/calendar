#pragma once
#include <string_view>

constexpr std::string_view ERR_INVALID_PARAM = "Ошибка: неверный параметр. year и month должны быть числами.\n";
constexpr std::string_view ERR_OUT_OF_RANGE =
    "Ошибка: параметр вне допустимого диапазона. month должен быть от 1 до 12.\n";
constexpr std::string_view ERR_NOT_FOUND = "Страница не найдена. Используйте /calendar/month или /calendar/year.\n";