#################################################
# Created by: wocson
# $FreeBSD: 
#
# wocson (c) / v.0.68
#
#################################################
PROJECT_NAME=lpw
SHELL=/bin/sh
CONFIGDIR=./config

.if ${PROJECT_NAME}

PREFIXDIR=/usr/local
BINDIR=/bin
LIBDIR=/lib
SHARE=/share/${PROJECT_NAME}

all: release
release:
	@${CONFIGDIR}/prj.build ${PROJECT_NAME} release
debug: 
	@${CONFIGDIR}/prj.build ${PROJECT_NAME} debug
reinstall: install
	  
install:
	@${CONFIGDIR}/prj.build ${PROJECT_NAME} install ${PREFIXDIR} ${BINDIR}
deinstall:
	@${CONFIGDIR}/prj.build ${PROJECT_NAME} deinstall ${PREFIXDIR} ${BINDIR}
clean:
	@${CONFIGDIR}/prj.build ${PROJECT_NAME} clean
help:
	@${CONFIGDIR}/prj.build ${PROJECT_NAME} help
.else
all: .DEFAULT
.DEFAULT:
	@echo " ERROR: Value PROJECT_NAME on Makefile not defined"
	@echo "    Ex. PROJECT_NAME=name"
clean:
	@${CONFIGDIR}/prj.build None Clean
.endif
