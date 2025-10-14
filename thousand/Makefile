#################################################
# Created by: wocson
# $FreeBSD: 
#
# wocson (c) / v.0.68
#
#################################################
PROJECT_NAME=thousand
SHELL=/bin/sh
CONFIGDIR=./.settings

.if ${PROJECT_NAME}

PREFIXDIR=/usr/local
BINDIR=/bin
LIBDIR=/lib
SHARE=/share/${PROJECT_NAME}

all: release
release:
	@${CONFIGDIR}/make.sh ${PROJECT_NAME} release
debug: 
	@${CONFIGDIR}/make.sh ${PROJECT_NAME} debug
reinstall: install
	  
install:
	@${CONFIGDIR}/make.sh ${PROJECT_NAME} install ${PREFIXDIR} ${BINDIR}
deinstall:
	@${CONFIGDIR}/make.sh ${PROJECT_NAME} deinstall ${PREFIXDIR} ${BINDIR}
clean:
	@${CONFIGDIR}/make.sh ${PROJECT_NAME} clean
help:
	@${CONFIGDIR}/make.sh ${PROJECT_NAME} help
.else
all: .DEFAULT
.DEFAULT:
	@echo " ERROR: Value PROJECT_NAME on Makefile not defined"
	@echo "    Ex. PROJECT_NAME=name"
clean:
	@${CONFIGDIR}/make.sh None Clean
.endif
