cmake_minimum_required(VERSION 3.13)

project(console-poster)

# packages
find_package(FLTK REQUIRED)

# fltk ui, requires fluid
fltk_wrap_ui(console-poster src/ui.fl)

# core functionality
set(console-poster-core_SOURCES
    src/console_poster_core.h
    src/console_poster_core.cpp
)

# win32 resource
set(console-poster_RESOURCES
    src/console_poster.rc
)

# app sources
set(console-poster_SOURCES
    ${console-poster-core_SOURCES}
    ${console-poster_FLTK_UI_SRCS}
    src/console_poster.cpp
    ${console-poster_RESOURCES}
)

# app with ui
add_executable(console-poster
    ${console-poster_SOURCES}
)

# use fltk library
target_include_directories(console-poster PUBLIC ${FLTK_INCLUDE_DIR})
target_link_libraries(console-poster ${FLTK_LIBRARIES})

# gdi
target_link_libraries(console-poster gdiplus)
