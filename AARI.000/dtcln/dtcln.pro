################################################################
# DTCln
# Copyright (C) 2019   Vladimir Noskov
################################################################

TEMPLATE = app
TARGET = dtcln
DEPENDPATH += . src
INCLUDEPATH += . src
#OBJECTS_DIR = obj
#DESTDIR  = ../bin
COMMONSRC = ../common

QMAKE_CXXFLAGS   *= -std=c++17
QMAKE_CXXFLAGS	 *= -O3

QT += network widgets

#QMAKE_LFLAGS += -static -static-libgcc

INCLUDEPATH += \
		$${COMMONSRC}

# Input
HEADERS += \
        src/dtcln.h \
        src/clientsocketadapter.h \
        $${COMMONSRC}/isocketadapter.h \
        $${COMMONSRC}/socketadapter.h

SOURCES += \
        src/dtcln.cpp \
        src/main.cpp \
        src/clientsocketadapter.cpp \
        $${COMMONSRC}/isocketadapter.cpp \
        $${COMMONSRC}/socketadapter.cpp

FORMS += resources/dtcln.ui
#RESOURCES += resources/br.qrc resources/icons.qrc
#TRANSLATIONS += src/english.ts src/russian.ts

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt
CONFIG           += warn_on
#CONFIG           += no_keywords
CONFIG           += silent

######################################################################
# release/debug mode
######################################################################

win32 {
    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version. For your own application development you
    # might need a debug version.
    # Enable debug_and_release + build_all if you want to build both.

    CONFIG           += debug_and_release
    CONFIG           += build_all
}
else {

    CONFIG           += debug_and_release
    VER_MAJ           = $${QWT_VER_MAJ}
    VER_MIN           = $${QWT_VER_MIN}
    VER_PAT           = $${QWT_VER_PAT}
    VERSION           = $${QWT_VERSION}
}

linux-g++ | linux-g++-64 {
    #CONFIG           += separate_debug_info
     #QMAKE_CXXFLAGS   *= -Wfloat-equal
     #QMAKE_CXXFLAGS   *= -Wshadow
     #QMAKE_CXXFLAGS   *= -Wpointer-arith
     #QMAKE_CXXFLAGS   *= -Wconversion
     #QMAKE_CXXFLAGS   *= -Wsign-compare
     #QMAKE_CXXFLAGS   *= -Wsign-conversion
     #QMAKE_CXXFLAGS   *= -Wlogical-op
     #QMAKE_CXXFLAGS   *= -Werror=format-security
     QMAKE_CXXFLAGS   *= -std=c++17
     QMAKE_CXXFLAGS	  *= -O3

    # when using the gold linker ( Qt < 4.8 ) - might be
    # necessary on non linux systems too
    #QMAKE_LFLAGS += -lrt
}

MOC_DIR      = moc
RCC_DIR      = resources

!debug_and_release {

    # in case of debug_and_release object files
    # are built in the release and debug subdirectories
    OBJECTS_DIR       = obj
}

ROOT = $${PWD}
OUT_ROOT = $${OUT_PWD}

INCLUDEPATH += $${ROOT}/src
DEPENDPATH  += $${ROOT}/src

!debug_and_release {

    DESTDIR      = $${OUT_ROOT}/release
}
else {
    CONFIG(debug, debug|release) {

        DESTDIR      = $${OUT_ROOT}/debug
    }
    else {

        DESTDIR      = $${OUT_ROOT}/release
    }
}

QMAKE_RPATHDIR *= $${OUT_ROOT}/lib
