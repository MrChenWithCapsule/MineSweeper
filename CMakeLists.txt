cmake_minimum_required(VERSION 3.10)
project(
    MineSweeper
    LANGUAGE C
)
set(CMAKE_C_STANDARD 11)

add_subdirectory(lib)

include_directories(include)
aux_source_directory(src SourceDir)

add_executable(MineSweeper ${SourceDir})
target_include_directories(${CursesIncludeDir})
target_link_libraries(${CursesLib})
