QT       += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Union des deux anciens .pro pour ne garder qu'un seul projet buildable.
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    smartmarket.cpp \
    oussama.cpp

HEADERS += \
    mainwindow.h \
    smartmarket.h \
    oussama.h

FORMS += \
    mainwindow.ui \
    smartmarket.ui \
    oussama.ui

TRANSLATIONS += \
    reviewers_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
