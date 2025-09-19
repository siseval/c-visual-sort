cmake_minimum_required(VERSION 3.10)
project(visual-sort)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(visual-sort
    src/main.c
    src/visual-sort.c
    src/list.c
    src/cli.c
)

target_include_directories(visual-sort
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(visual-sort ${CURSES_LIBRARY})
