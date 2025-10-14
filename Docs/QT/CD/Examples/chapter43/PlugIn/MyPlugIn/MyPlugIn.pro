TEMPLATE = lib
CONFIG   += plugin
QT       -= gui
DESTDIR  = ../plugins
SOURCES  = MyPlugin.cpp
HEADERS  = MyPlugin.h \
           ../Application/interfaces.h 
TARGET   = myplugin

