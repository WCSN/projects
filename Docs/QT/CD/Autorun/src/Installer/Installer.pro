win32 {
    TEMPLATE     = app
    TRANSLATIONS = Installer_ru.ts
    RESOURCES    = resource.qrc
    HEADERS      = Installer.h
    SOURCES      = main.cpp \
                   Installer.cpp
    CONFIG      += release 
    TARGET       = install
    DESTDIR      = ../../..
    RC_FILE	+= resources/Installer.rc
}

