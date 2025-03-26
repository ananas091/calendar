cmake_minimum_required(VERSION 3.10)
project(calendar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(calendar src/main.cpp)

target_compile_options(calendar PRIVATE
    -std=gnu++17
    -ggdb3 -grecord-gcc-switches
    -flto -O2
    -D_FORTIFY_SOURCE=2
    -fstack-protector-strong -fstack-clash-protection
    -Wall -Wextra -Wpedantic -Wunused-result -Wformat=2 -Wstrict-prototypes -Wmissing-declarations -Wswitch-unreachable -Wshadow
    -Werror
    -fdiagnostics-color=always
)
