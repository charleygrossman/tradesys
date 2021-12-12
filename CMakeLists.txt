cmake_minimum_required(VERSION 3.0.0)

project(tradesys C)

add_compile_options(-std=gnu17 -W -Wall -Werror)

link_libraries(-lrt)

add_executable(
    ${PROJECT_NAME}
    src/main.c
    src/sig/clock.c
)
