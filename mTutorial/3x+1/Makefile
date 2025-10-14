# Created by: wocson
# $FreeBSD: 
PROJECT_NAME=f3xp1

CONFIGDIR=./config
prefix=/usr/local
BINDIR=/bin
LIBDIR=/lib
SHARE=/share/${PROJECT_NAME}

all: build

build:
	@./${CONFIGDIR}/prj.build ${PROJECT_NAME} Release
    
debug: 
	@./${CONFIGDIR}/prj.build ${PROJECT_NAME} Debug

reinstall: install
	
install:
	@./${CONFIGDIR}/prj.build ${PROJECT_NAME} Install ${prefix} ${BINDIR}

deinstall:
	@./${CONFIGDIR}/prj.build ${PROJECT_NAME} Deinstall ${prefix} ${BINDIR}
clean:
	@./${CONFIGDIR}/prj.build ${PROJECT_NAME} Clean
