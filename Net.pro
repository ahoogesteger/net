#-------------------------------------------------
#
# Project created by QtCreator 2018-05-31T17:11:28
#
#-------------------------------------------------

QT       -= core gui

TARGET = Net
TEMPLATE = lib
CONFIG += staticlib c++14

unix {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Werror
}

win32 {
    QMAKE_CXXFLAGS_WARN_ON -= -W3
    QMAKE_CXXFLAGS_WARN_ON += -W4
    QMAKE_CXXFLAGS += -WX -D_USE_MATH_DEFINES

    CONFIG(release) {
        QMAKE_CXXFLAGS += /wd4127
    }
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataBlockContainer.cpp \
    TCPConnection.cpp \
    TCPServer.cpp \
    AbstractTCPConnection.cpp \
    Win32TCPConnection.cpp \
    AbstractTCPServer.cpp \
    Network.cpp

HEADERS += \
    DataBlock.h \
    DataBlockContainer.h \
    DataViewBlock.h \
    Endian.h \
    IConnectionPool.h \
    IDataBlock.h \
    IPv4EndPoint.h \
    TCPConnection.h \
    TCPServer.h \
    AbstractTCPConnection.h \
    Win32TCPConnection.h \
    AbstractTCPServer.h \
    Network.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
