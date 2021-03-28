#
# Makefile for uucpd and ulogin
#

SOURCES	= uucpd.c
OBJECTS	= uucpd.o 
BINARY	= uucpd

SSOURCES= ulogin.c
SOBJECTS= ulogin.o
SBINARY	= ulogin

SRCDIST	= ../../../srcdist
BINDIST	= ../../../bindist
INSTALL	=

MCS	= mcs -c
OPTS	= -O
DEFINES	=
INCLUDE	= -I../../../include
LDFLAGS	= -s
LIBS	= ../../lib/libnet.a -lnsl_s -lPW

include ../../../make.pre

DEFS	= -DSYSTEM5 -DUSG $(DEFINES)
CFLAGS	= ${OPTS} ${INCLUDE} ${DEFS}

all: ${BINARY} ${SBINARY} 
	@echo ${BINARY} and ${SBINARY} are up to date.

${BINARY}: ${OBJECTS}
	${CC} ${LDFLAGS} -o ${BINARY} ${OBJECTS} ${LIBS}
	${MCS} $(BINARY)

${SBINARY}: ${SOBJECTS}
	${CC} ${LDFLAGS} -o ${SBINARY} ${SOBJECTS} ${LIBS}
	${MCS} $(BINARY)

install	: $(BINARY)
	cpset $(BINARY) $(INSTALL)/usr/etc/$(BINARY) 6555 uucp daemon
	cpset $(SBINARY) $(INSTALL)/usr/etc/$(SBINARY) 4755 root bin

bindist	: $(BINARY) $(SBINARY)
	cpset $(BINARY) $(BINDIST)/user/usr/etc/$(BINARY) 6555 uucp daemon
	cpset $(SBINARY) $(BINDIST)/user/usr/etc/$(SBINARY) 4755 root bin

clean:
	rm -f a.out core ${OBJECTS} ${SOBJECTS} 

clobber	: clean
	rm -f $(BINARY) $(SBINARY)

relink:
	rm -f $(BINARY) $(SBINARY)

