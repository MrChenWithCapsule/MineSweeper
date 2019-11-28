# Use PDCurses on Windows, otherwise ncurses.
if(WIN32)
    include(ExternalProject)
    ExternalProject_Add(PDCurses
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/PDCurses/wincon"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "nmake -f Makefile.vc"
    )
    find_library(CursesLib "pdcurses.lib")
    set(CursesIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/PDCurses")
    set(UsePDCurses ON)
else()
    find_library(CursesLib "curses")
    set(CursesIncludeDir "")
    set(UsePDCurses OFF)
endif()
