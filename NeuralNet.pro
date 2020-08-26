QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Main/main.cpp \
    Main/mainwindow.cpp

HEADERS += \
    GClasses/Common/G_Common.h \
    GClasses/Common/Log.h \
    GClasses/Common/LogDataInterpret.h \
    GClasses/Common/LogDefines.h \
    GClasses/Common/LogTests.h \
    GClasses/Common/LogWriter.h \
    GClasses/Common/StaticLog.h \
    GClasses/Common/Tester.h \
    Main/mainwindow.h

FORMS += \
    Main/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    Libraries/fmt.lib
