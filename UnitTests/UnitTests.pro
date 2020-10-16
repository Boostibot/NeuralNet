#include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += \
        ../


#User files
HEADERS += \
        DynamicLog.h \
        File.h \
        StaticLog.h \
        AllocatorFramework.h \
        CFileManager.h

SOURCES += \


#Prerequisites
HEADERS += Catch2/Catch.hpp \

SOURCES += UnitTestsMain.cpp


#FMT
HEADERS += \
    ../Libraries/Fmt/fmt/chrono.h \
    ../Libraries/Fmt/fmt/color.h \
    ../Libraries/Fmt/fmt/compile.h \
    ../Libraries/Fmt/fmt/core.h \
    ../Libraries/Fmt/fmt/format.h \
    ../Libraries/Fmt/fmt/format-inl.h \
    ../Libraries/Fmt/fmt/locale.h \
    ../Libraries/Fmt/fmt/os.h \
    ../Libraries/Fmt/fmt/ostream.h \
    ../Libraries/Fmt/fmt/posix.h \
    ../Libraries/Fmt/fmt/printf.h \
    ../Libraries/Fmt/fmt/ranges.h \

SOURCES +=\
    ../Libraries/Fmt/format.cc \
    ../Libraries/Fmt/os.cc \



