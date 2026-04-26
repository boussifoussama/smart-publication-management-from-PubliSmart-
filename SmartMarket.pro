QT += core gui widgets sql charts network printsupport serialport

CONFIG += c++17 utf8_source

TEMPLATE = app
TARGET = SmartMarket

SOURCES += main.cpp \
    arduino.cpp \
    arduino2.cpp \
    conference.cpp \
    connection.cpp \
    participant.cpp \
    publication.cpp \
    reviewer.cpp \
    smartmarket.cpp

HEADERS += arduino.h \
    arduino2.h \
    conference.h \
    connection.h \
    participant.h \
    publication.h \
    reviewer.h \
    smartmarket.h

FORMS += smartmarket.ui

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
