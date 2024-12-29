#-------------------------------------------------
#
# Project created by QtCreator 2024-11-22T15:02:41
#
#-------------------------------------------------
QMAKE_CXXFLAGS -= -fpch

QT       += core gui
QT   += serialbus serialport
QT += charts
QT += sql
QT += printsupport

QT += svg
QT += concurrent



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtModBus
TEMPLATE = app

INCLUDEPATH += $$PWD/poppler
LIBS += -L$$PWD/poppler -llibpoppler
LIBS += -L$$PWD/poppler -llibpoppler-qt5
RC_FILE = adminboot.rc
DISTFILES += \
    adminboot.rc \
    adminboot.manifest

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    sqliteaction.cpp \
    modbusconn.cpp \
    hstorylist.cpp \
    hoveredrowitemdelegate.cpp \
    pressstd.cpp \
    chartdel.cpp \
    configset.cpp \
    login.cpp \
    usercreate.cpp \
    loginset.cpp \
    wpdfwriter.cpp \
    form3fix.cpp \
    form1fix.cpp \
    form2_1fix.cpp \
    qdebugloginfo.cpp \
    historyform1.cpp

HEADERS  += mainwindow.h \
    sqliteaction.h \
    modbusconn.h \
    hstorylist.h \
    hoveredrowitemdelegate.h \
    pressstd.h \
    configset.h \
    login.h \
    usercreate.h \
    loginset.h \
    wpdfwriter.h \
    form3fix.h \
    form1fix.h \
    form2_1fix.h \
    qdebugloginfo.h \
    historyform1.h

FORMS    += mainwindow.ui \
    hstorylist.ui \
    pressstd.ui \
    configset.ui \
    login.ui \
    usercreate.ui \
    loginset.ui \
    form3fix.ui \
    form1fix.ui \
    form2_1fix.ui \
    historyform1.ui

RESOURCES += \
    res.qrc

DISTFILES +=
