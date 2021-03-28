#	START NEW ARIX SCCS HEADER
#
#	@(#) cu.mk: version 25.1 created on 12/2/91 at 14:10:28
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)cu.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:#ident	"cu:cu.mk	2.8"
#ident	"@(#)cu:cu.mk	25.1"

IROOT =
SROOT =
include $(IROOT)/macdefs

OWNER=uucp
GRP=daemon
BIN=$(ROOT)/usr/bin
UUDIR=../uucp
# LDFLAGS = -s

CUDIR = .
# CFLAGS = -O -DSTANDALONE -I$(UUDIR)
# CFLAGS = -O -DSTANDALONE -DSMALL -I$(UUDIR) #for smaller a.outs
LOCAL= -DSTANDALONE -I$(UUDIR)
LIBS= -lnsl_s -lsec

#	Use the following lines for DATAKIT
# CFLAGS = -O -DSTANDALONE -I$(UUDIR) -DDATAKIT
# CFLAGS = -O -DDATAKIT -DSTANDALONE -DSMALL -I$(UUDIR) #for smaller a.outs
# LIBS=-ldk

CUOBJS =  cu.o callers.o getargs.o culine.o uucpdefs.o ulockf.o push.o\
	 conn.o altconn.o interface.o strsave.o sysfiles.o strecpy.o stoa.o

SOURCE =  cu.c altconn.c culine.c $(UUDIR)/callers.c\
	$(UUDIR)/conn.c $(UUDIR)/uucpdefs.c $(UUDIR)/ulockf.c \
	$(UUDIR)/getargs.c $(UUDIR)/interface.c $(UUDIR)/strsave.c \
	$(UUDIR)/sysfiles.c $(UUDIR)/strecpy.c $(UUDIR)/stoa.c $(UUDIR)/push.c

.c.o:
	$(CC) -c $(LOCAL) $(CFLAGS) $*.c

all cu:	$(CUOBJS)
	$(CC) $(CFLAGS) $(CUOBJS) $(LDFLAGS) $(LIBS) -o cu

install:	cu
	$(INS) cu $(BIN)/cu
	chmod 4111 $(BIN)/cu
	chgrp sys $(BIN)/cu
	chown uucp $(BIN)/cu

ctags:
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)		;\
	do echo "$$i \\" >> greplist	;\
	done
	echo /dev/null >> greplist
	chmod +x greplist

clean:
	-rm -f *.o tags greplist

clobber: clean
	rm -f cu

callers.o:	$(UUDIR)/callers.c   $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c  $(CFLAGS) $(LOCAL) $(UUDIR)/callers.c

getargs.o:	$(UUDIR)/getargs.c   $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/getargs.c
 
uucpdefs.o:	$(UUDIR)/uucpdefs.c  $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/uucpdefs.c

ulockf.o:	$(UUDIR)/ulockf.c  $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/ulockf.c

conn.o:		$(UUDIR)/conn.c
	$(CC) -c  $(CFLAGS) $(LOCAL) $(UUDIR)/conn.c

interface.o:	$(UUDIR)/interface.c  $(UUDIR)/uucp.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/interface.c

strsave.o:	$(UUDIR)/strsave.c 
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/strsave.c

sysfiles.o:	$(UUDIR)/sysfiles.c  $(UUDIR)/sysfiles.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/sysfiles.c

strecpy.o:	$(UUDIR)/strecpy.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/strecpy.c

stoa.o:	$(UUDIR)/stoa.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/stoa.c

push.o:	$(UUDIR)/push.c
	$(CC) -c  $(CFLAGS) $(LOCAL) $(UUDIR)/push.c

