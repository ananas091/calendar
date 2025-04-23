cmake_minimum_required(VERSION 3.11)
project(calendar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# Подключаем сгенерированный скрипт conanbuildinfo.cmake, созданный Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# Выполняем макрос из conanbuildinfo.cmake, который настроит СMake на работу с библиотеками, установленными Conan
conan_basic_setup()

# Ищем Boost версии 1.78
find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  # Boost найден, добавляем к каталогам заголовочных файлов проекта путь к
  # заголовочным файлам Boost
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# Платформы вроде linux требуют подключения библиотеки pthread для
# поддержки стандартных потоков.
# Следующие две строки подключат эту библиотеку на таких платформах
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Добавляем путь к файлам, которые нужно проверить
set(SOURCE_FILES ../src/main.cpp ../src/calendar_print.h ../src/calendar_print.cpp ../src/utils.h ../src/utils.cpp)

# Цель для запуска clang-format
add_custom_target(
    clang-format-fix
    COMMAND clang-format -i ${SOURCE_FILES}   # Применить clang-format к каждому исходному файлу
    COMMENT "Running clang-format on source files"
)

# Основная цель сборки
add_executable(calendar ${SOURCE_FILES})

# Просим компоновщик подключить библиотеку для поддержки потоков
target_link_libraries(calendar PRIVATE Threads::Threads) 

# Зависимость основной цели от clang-format
add_dependencies(calendar clang-format-fix)

# Включаем стандарт компиляции и другие флаги
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
