QT       += core gui charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    arduino.cpp \
    conference.cpp \
    connection.cpp \
    main.cpp \
    participant.cpp \
    publication.cpp \
    smartmarket.cpp

HEADERS += \
    arduino.h \
    conference.h \
    connection.h \
    participant.h \
    publication.h \
    smartmarket.h

FORMS += \
    smartmarket.ui

RESOURCES += \
    resources.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT += sql
QT += core gui widgets sql charts network printsupport
CONFIG += c++11
