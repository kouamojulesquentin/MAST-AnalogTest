#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T21:12:04
#
#-------------------------------------------------

QT -= core gui

TARGET   = Logger
TEMPLATE = lib

CONFIG  += c++14

#+QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
#+QMAKE_CXXFLAGS += -fmax-errors=3

#+QMAKE_CXXFLAGS_DEBUG += -g
#+QMAKE_CXXFLAGS  = -c -pipe -fno-keep-inline-dllexport -std=gnu++14 -frtti -fexceptions -mthreads
#+QMAKE_CXXFLAGS += -Wall
#+QMAKE_CXXFLAGS += -Wpedantic  -Wnon-virtual-dtor -Wredundant-decls -Wundef -Wmissing-include-dirs -Wswitch-enum -Wswitch-default
#+QMAKE_CXXFLAGS += -fmax-errors=3

INCLUDEPATH = g3log

SOURCES +=  \
    LogFormatter.cpp         \
    LoggerSinks.cpp          \
    CustomFileSink.cpp       \
    filesink.cpp             \
    g3log.cpp                \
    logcapture.cpp           \
    loglevels.cpp            \
    logmessage.cpp           \
    logworker.cpp            \
    time.cpp

HEADERS += \
    LogFormatter.h               \
    LoggerSinks.h                \
    CustomFileSink.h             \
    g3log/crashhandler.hpp       \
    g3log/active.hpp             \
    g3log/filesink.hpp           \
    g3log/future.hpp             \
    g3log/g3log.hpp              \
    g3log/g3log_debug.hpp        \
    g3log/g3log_export.hpp       \
    g3log/logcapture.hpp         \
    g3log/loglevels.hpp          \
    g3log/logmessage.hpp         \
    g3log/logworker.hpp          \
    g3log/moveoncopy.hpp         \
    g3log/shared_queue.hpp       \
    g3log/sink.hpp               \
    g3log/sinkhandle.hpp         \
    g3log/sinkwrapper.hpp        \
    g3log/std2_make_unique.hpp   \
    g3log/stlpatch_future.hpp    \
    g3log/time.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
    SOURCES  += Unix/crashhandler_unix.cpp
}
else: win32 {
  DEFINES += WINDOWS BUILD_DLL
  LIBS    += -lDbghelp \
             -limagehlp

  SOURCES += Windows/crashhandler_windows.cpp \
             Windows/stacktrace_windows.cpp

  HEADERS += Windows/stacktrace_windows.hpp
}

CONFIG(debug,   debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Logger/debug)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/debug)
}
else: CONFIG(release, debug|release) {
  OBJECTS_DIR = $$clean_path($$PWD/../Build_Qt/Logger/release)
  DESTDIR     = $$clean_path($$PWD/../Build_Qt/bin/release)
}
