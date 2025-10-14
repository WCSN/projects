#################################################
# Shells
# FreeBSD 
#
# wocson (c)
#
#################################################
PROJECT_NAME=Shells
SHELL=/bin/sh
BINDIR="./.settings"

.if ${PROJECT_NAME}
all: build
user:
	@${BINDIR}/setup user
users:
	@${BINDIR}/setup users
root:
	@${BINDIR}/setup root
system:
	@${BINDIR}/setup system
pull:
	@${BINDIR}/setup pull
commit:
	@${BINDIR}/setup commit
build:
	@${BINDIR}/setup build
	@echo " Not installed"
.else
all: .DEFAULT
.DEFAULT:
	@echo " Value PROJECT_NAME on Makefile not set"
.endif
