QT += widgets serialport

TARGET = serial
TEMPLATE = app

HEADERS += \
    dialog.h \
    receiverthread.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    receiverthread.cpp

target.path = ./serial



