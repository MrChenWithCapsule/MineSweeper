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
target_include_directories(MineSweeper SYSTEM PRIVATE
    ${CursesIncludeDir} ${PthreadIncludeDir}
)
target_link_libraries(MineSweeper ${CursesLib} ${PthreadLib})
if(UsePDCurses)
    target_compile_definitions(MineSweeper
        PRIVATE USE_PDCURSES NCURSES_MOUSE_VERSION
    )
endif()
if(UseSystemPthread)
    target_compile_definitions(MineSweeper
        PRIVATE USE_SYSTEM_PTHREAD
    )
endif()
