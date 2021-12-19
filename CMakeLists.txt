cmake_minimum_required(VERSION 3.0.0)

project(tradesys C)

add_compile_options(-std=gnu17 -W -Wall -Werror)

link_libraries(-lrt -pthread)

set(SRC ${PROJECT_SOURCE_DIR}/src)

include_directories(
    ${SRC}
    ${SRC}/feed
    ${SRC}/sig
)

add_executable(
    ${PROJECT_NAME}
    ${SRC}/main.c
    ${SRC}/feed/feed.c
    ${SRC}/feed/rate.c
    ${SRC}/sig/clock.c
)
