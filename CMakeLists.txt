cmake_minimum_required(VERSION 3.10)
project(
    MineSweeper
    LANGUAGES C
)
set(CMAKE_C_STANDARD 11)

add_subdirectory(lib)

include_directories(include)
aux_source_directory(src SourceDir)

add_executable(MineSweeper ${SourceDir})
target_include_directories(MineSweeper
    PRIVATE ${CursesIncludeDir}
)
target_link_libraries(MineSweeper ${CursesLib})
