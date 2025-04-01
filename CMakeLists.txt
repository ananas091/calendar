cmake_minimum_required(VERSION 3.10)
project(calendar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Добавляем путь к файлам, которые нужно проверить
set(SOURCE_FILES ../src/main.cpp)

# Цель для запуска clang-format
add_custom_target(
    clang-format-check
    COMMAND clang-format -i ${SOURCE_FILES}   # Применить clang-format к каждому исходному файлу
    COMMENT "Running clang-format on source files"
)

# Основная цель сборки
add_executable(calendar ${SOURCE_FILES})

# Зависимость основной цели от clang-format
add_dependencies(calendar clang-format-check)

# Включаем стандарт компиляции и другие флаги
target_compile_options(calendar PRIVATE
    -std=gnu++17
    -ggdb3 -grecord-gcc-switches
    -flto -O2
    -D_FORTIFY_SOURCE=2
    -fstack-protector-strong -fstack-clash-protection
    -Wall -Wextra -Wunused-result -Wformat=2 -Wstrict-prototypes -Wmissing-declarations -Wswitch-unreachable -Wshadow
    -Werror
    -fdiagnostics-color=always
)
