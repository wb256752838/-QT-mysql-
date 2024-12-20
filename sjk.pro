#-------------------------------------------------
#
# Project created by QtCreator 2024-12-17T15:51:05
#
#-------------------------------------------------

QT       += core gui
QT       +=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sjk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        addbookdialog.cpp \
        adduserdialog.cpp \
        admin.cpp \
        borhispage.cpp \
        bormane.cpp \
        main.cpp \
        modpwd.cpp \
        s_book.cpp \
        sjk.cpp

HEADERS += \
        addbookdialog.h \
        adduserdialog.h \
        admin.h \
        borhispage.h \
        bormane.h \
        modpwd.h \
        s_book.h \
        sjk.h

FORMS += \
        addbookdialog.ui \
        adduserdialog.ui \
        admin.ui \
        borhispage.ui \
        bormane.ui \
        modpwd.ui \
        s_book.ui \
        sjk.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

DISTFILES +=
