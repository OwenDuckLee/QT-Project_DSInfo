QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    3rdparty/libcpuid/libcpuid/libcpuid.h \
    3rdparty/libpci/lib/pci.h \
    mainwindow.h
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.

TARGET = DSInfo_example_20231115
DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/libpci/lib/release/ -lpci
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/libpci/lib/debug/ -lpci
else:unix: LIBS += -L$$PWD/3rdparty/libpci/lib/ -lpci

INCLUDEPATH += $$PWD/3rdparty/libpci
DEPENDPATH += $$PWD/3rdparty/libpci

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/libcpuid/libcpuid/.libs/release/ -lcpuid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/libcpuid/libcpuid/.libs/debug/ -lcpuid
else:unix: LIBS += -L$$PWD/3rdparty/libcpuid/libcpuid/.libs/ -lcpuid

INCLUDEPATH += $$PWD/3rdparty/libcpuid/libcpuid/.libs
DEPENDPATH += $$PWD/3rdparty/libcpuid/libcpuid/.libs
