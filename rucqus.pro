TEMPLATE = app

QT += qml quick network multimedia sql
CONFIG += c++11

SOURCES += src/main.cpp \
    src/rucqusplayer.cpp \
    src/sqlquerymodel.cpp \
    src/artistmodel.cpp \
    src/rucqusapp.cpp \
    src/genremodel.cpp \
    src/albummodel.cpp \
    src/plistmodel.cpp \
    src/confighandler.cpp \
    src/radiomodel.cpp

RESOURCES += src/qml.qrc

DEFINES += QT_RESTRICTED_CAST_FROM_ASCII

QMAKE_CXXFLAGS = -march=native -mtune=native -O2

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/rucqusplayer.h \
    src/sqlquerymodel.h \
    src/artistmodel.h \
    src/rucqusapp.h \
    src/genremodel.h \
    src/albummodel.h \
    src/plistmodel.h \
    src/confighandler.h \
    src/radiomodel.h
