include(ExternalProject)

# Use PDCurses on Windows, otherwise ncurses.
if(WIN32)
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

# Use bundled pthread on Windows, otherwise system pthread
if(WIN32)
    ExternalProject_Add(Pthreads4w      
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pthreads4w"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "nmake -f Makefile all"
    )
    find_library(PthreadLib "pthreadVC3")
    set(PthreadIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/pthreads4w")
    set(UseSystemPthread OFF)
else()
    find_library(PthreadLib "pthread")
    set(PthreadIncludeDir "")
    set(UseSystemPthread ON)
endif()
