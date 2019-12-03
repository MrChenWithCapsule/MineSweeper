include(ExternalProject)

# Use PDCurses on Windows, otherwise ncurses.
if(WIN32)
    ExternalProject_Add(PDCurses
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/PDCurses/wincon"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND nmake -f Makefile.vc all
        BUILD_IN_SOURCE ON
        INSTALL_COMMAND ""
    )
    set(CursesLib "${CMAKE_CURRENT_SOURCE_DIR}/PDCurses/wincon/pdcurses.lib" PARENT_SCOPE)
    # find_library(CursesLib "pdcurses.lib")
    set(CursesIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/PDCurses" PARENT_SCOPE)
    set(UsePDCurses ON PARENT_SCOPE)
else()
    find_library(CursesLib "curses" PARENT_SCOPE)
    set(CursesIncludeDir "" PARENT_SCOPE)
    set(UsePDCurses OFF PARENT_SCOPE)
endif()

# Use bundled pthread on Windows, otherwise system pthread
if(WIN32)
    ExternalProject_Add(Pthreads4w      
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pthreads4w"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND nmake all
        BUILD_IN_SOURCE ON
        INSTALL_COMMAND ""
    )
    set(PthreadLib "${CMAKE_CURRENT_SOURCE_DIR}/pthreads4w/pthreadVC3.lib" PARENT_SCOPE)
    # find_library(PthreadLib "libpthread.a")
    # set(PthreadLib "D:/Downloads/mingw64/x86_64-w64-mingw32/lib/libpthread.a")
    set(PthreadIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/pthreads4w" PARENT_SCOPE)
    # set(PthreadIncludeDir "" PARENT_SCOPE)
    set(UseSystemPthread OFF PARENT_SCOPE)
    # set(UseSystemPthread ON PARENT_SCOPE)
else()
    find_library(PthreadLib "pthread" PARENT_SCOPE)
    set(PthreadIncludeDir "" PARENT_SCOPE)
    set(UseSystemPthread ON PARENT_SCOPE)
endif()

# message(${CursesIncludeDir})
# message(${PthreadIncludeDir})
