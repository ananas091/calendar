# Создать образ на основе базового слоя gcc (там будет ОС и сам компилятор).
# 11.3 — используемая версия gcc.
FROM gcc:11.3 as build

# Выполнить установку зависимостей внутри контейнера.
RUN apt update && \
    apt install -y \
      cmake \
      clang-format

# Скопировать файлы проекта внутрь контейнера
COPY ./src /app/src
COPY CMakeLists.txt /app/

RUN mkdir /app/build && cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . 

# Второй контейнер в том же докерфайле
FROM ubuntu:22.04 as run

# Создадим пользователя www
RUN groupadd -r www && useradd -r -g www www
USER www

# Скопируем приложение со сборочного контейнера в директорию /app.
# Не забываем также папку data, она пригодится.
COPY --from=build /app/build/calendar /app/

# Запускаем игровой сервер
ENTRYPOINT ["/app/calendar"] 