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



#User defined
SOURCES += \
    Main/main.cpp \
    Main/mainwindow.cpp

HEADERS += \
    General/Common/Common.h \
    General/Common/MetaPrograming.h \
    General/Common/Typedefs.h \
    General/Common/Defines.h \
    General/File/CFileManager.h \
    General/File/CharSupport.h \
    General/File/CompilerSupport.h \
    General/File/File.h \
    General/File/LegacyUniversalString.h \
    General/File/OpenMode.h \
    General/File/OsSupport.h \
    General/File/UniversalString.h \
    General/File/UnsafeFile.h \
    General/File/UseExamples.h \
    General/File/UtfConversion.h \
    \
    General/ConstexprNumConversion/ConstexprNumConversion.h \
    \
    General/DynamicLog/Log.h \
    General/DynamicLog/LogDataInterpret.h \
    General/DynamicLog/LogDefines.h \
    General/DynamicLog/LogTests.h \
    General/DynamicLog/LogWriter.h \
    \
    General/OptionalTemplateArgs/OptionalTemplateArgs.h \
    \
    General/StaticLog/StaticLog.h \
    General/StaticLog/DefaultDataInterpret.h \
    General/StaticLog/DefaultLogger.h \
    General/StaticLog/LoggerInterface.h \
    General/StaticLog/TesterClasses.h \
    General/StaticLog/LogDefines.h \
    General/StaticLog/SpeedTests.h \
    \
    Main/NeuralNetwork.h \
    Main/Temp.h \
    Main/mainwindow.h

FORMS += \
    Main/mainwindow.ui

#FMT
SOURCES +=\
    Libraries/Fmt/format.cc \
    Libraries/Fmt/os.cc \

HEADERS += \
    Libraries/Fmt/fmt/chrono.h \
    Libraries/Fmt/fmt/color.h \
    Libraries/Fmt/fmt/compile.h \
    Libraries/Fmt/fmt/core.h \
    Libraries/Fmt/fmt/format.h \
    Libraries/Fmt/fmt/format-inl.h \
    Libraries/Fmt/fmt/locale.h \
    Libraries/Fmt/fmt/os.h \
    Libraries/Fmt/fmt/ostream.h \
    Libraries/Fmt/fmt/posix.h \
    Libraries/Fmt/fmt/printf.h \
    Libraries/Fmt/fmt/ranges.h \
    Main/UnbufferedFile.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#include(UnitTests/UnitTests.pro)

#DISTFILES += \
#    Libraries/fmt.lib
