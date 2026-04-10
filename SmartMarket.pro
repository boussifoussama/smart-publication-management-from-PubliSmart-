QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    connection.cpp \
    main.cpp \
    smartmarket.cpp

HEADERS += \
    connection.h \
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
