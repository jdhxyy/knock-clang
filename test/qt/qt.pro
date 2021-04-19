TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    ../../knock.c \
    ../../lib/tzmalloc/bget.c \
    ../../lib/tzmalloc/tzmalloc.c \
    ../../lib/tzlist/tzlist.c \
    ../../lib/lagan-clang/lagan.c

INCLUDEPATH += ../../ \
    ../../lib/lagan-clang \
    ../../lib/tzlist \
    ../../lib/tzmalloc

HEADERS += \
    ../../knock.h \
    ../../lib/tzmalloc/bget.h \
    ../../lib/tzmalloc/tzmalloc.h \
    ../../lib/tzlist/tzlist.h \
    ../../lib/lagan-clang/lagan.h
