cmake_minimum_required(VERSION 3.0.0)

project(tradesys C)

add_compile_options(-std=gnu17 -W -Wall -Werror)

link_libraries(-lm -lrt -pthread)

set(SRC ${PROJECT_SOURCE_DIR}/src)

include_directories(
    ${SRC}
    ${SRC}/feed
    ${SRC}/sig
    ${SRC}/strategy
)

add_executable(
    ${PROJECT_NAME}
    ${SRC}/main.c
    ${SRC}/feed/feed.c
    ${SRC}/feed/quote.c
    ${SRC}/sig/clock.c
    ${SRC}/strategy/strategy.c
)
