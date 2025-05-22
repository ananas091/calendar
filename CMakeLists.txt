cmake_minimum_required(VERSION 3.11)
project(calendar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Подключаем Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Платформы вроде linux требуют подключения библиотеки pthread для поддержки потоков
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Файлы проекта
set(SOURCE_FILES
    ../src/main.cpp
    ../src/calendar_print.h
    ../src/calendar_print.cpp
    ../src/utils.h
    ../src/utils.cpp
    ../src/constants.h
)

# Цель для запуска clang-format
add_custom_target(
    clang-format-fix
    COMMAND clang-format -i ${SOURCE_FILES}
    COMMENT "Running clang-format on source files"
)

# Основная цель сборки
add_executable(calendar ${SOURCE_FILES})

# Линкуем библиотеки из Conan + pthread
target_link_libraries(calendar PRIVATE ${CONAN_LIBS} Threads::Threads)

# Зависимость от clang-format
add_dependencies(calendar clang-format-fix)

# Флаги компилятора
target_compile_options(calendar PRIVATE
    -std=gnu++17
    -ggdb3 -grecord-gcc-switches
    -O2
    -D_FORTIFY_SOURCE=2
    -fstack-protector-strong -fstack-clash-protection
    -Wall -Wextra -Wunused-result -Wformat=2 -Wmissing-declarations -Wswitch-unreachable -Wno-error=shadow
    -Werror
    -fdiagnostics-color=always
)
