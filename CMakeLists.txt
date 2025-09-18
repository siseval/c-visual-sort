cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)
project(visual-sort)
add_executable(
    visual-sort
    main.c
    visual-sort.c
    list.c
    cli.c
    )

find_package(Curses REQUIRED)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(visual-sort ${CURSES_LIBRARY})
