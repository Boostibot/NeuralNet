#include(Tests/gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += \
    Tests/

#Base
HEADERS += Tests/Catch2/Catch.hpp
SOURCES += Tests/main.cpp



#User added
HEADERS += \
        Tests/DynamicLog.h \
        Tests/StaticLog.h \
        Tests/tst_loggertests.h

SOURCES += \

