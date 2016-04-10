TEMPLATE = app

QT += qml quick multimedia sql
CONFIG += c++11

SOURCES += src/main.cpp \
    src/rucqusplayer.cpp \
    src/sqlquerymodel.cpp \
    src/artistmodel.cpp \
    src/rucqusapp.cpp \
    src/genremodel.cpp \
    src/albummodel.cpp \
    src/plistmodel.cpp \
    src/confighandler.cpp

RESOURCES += src/qml.qrc

QMAKE_CXXFLAGS += -march=native -mtune=native

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
    src/confighandler.h
