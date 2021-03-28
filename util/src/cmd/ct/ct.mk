#	START NEW ARIX SCCS HEADER
#
#	@(#) ct.mk: version 25.1 created on 12/2/91 at 14:09:28
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)ct.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:#ident	"ct:ct.mk	2.8"
#ident	"@(#)ct:ct.mk	25.1"

IROOT =
SROOT =
include $(IROOT)/macdefs

INSDIR = $(ROOT)/usr/bin
UUDIR=../uucp

CTDIR = .
# CFLAGS = -O -DSTANDALONE -I$(UUDIR)
# CFLAGS = -O -DSTANDALONE -DSMALL -I$(UUDIR) # for smaller a.outs
LOCAL= -O -DSTANDALONE -I$(UUDIR)
LIBS= -lnsl_s -lsec


CTOBJS =  ct.o callers.o getargs.o line.o uucpdefs.o ulockf.o\
	 conn.o interface.o sysfiles.o strsave.o strecpy.o stoa.o push.o 

SOURCE =  ct.c $(UUDIR)/callers.c $(UUDIR)/getargs.c \
	$(UUDIR)/line.c $(UUDIR)/uucpdefs.c $(UUDIR)/ulockf.c \
	 $(UUDIR)/conn.c $(UUDIR)/interface.c $(UUDIR)/sysfiles.c \
	 $(UUDIR)/strsave.c $(UUDIR)/strecpy.c $(UUDIR)/stoa.c $(UUDIR)/push.o 

.c.o:
	$(CC) -c -O $(LOCAL) $(CFLAGS) $*.c

all ct:	$(CTOBJS)
	$(CC) $(CFLAGS) $(CTOBJS) $(LOCAL) -o ct $(LIBS) $(LDFLAGS)

install: ct
	$(INS) ct $(INSDIR)/ct
	chmod 4111 $(INSDIR)/ct
	chgrp sys $(INSDIR)/ct
	chown root $(INSDIR)/ct

callers.o:	$(UUDIR)/callers.c   $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/callers.c

getargs.o:	$(UUDIR)/getargs.c   $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/getargs.c
 
line.o:		$(UUDIR)/line.c   $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/line.c

uucpdefs.o:	$(UUDIR)/uucpdefs.c  $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/uucpdefs.c

ulockf.o:	$(UUDIR)/ulockf.c  $(UUDIR)/uucp.h $(UUDIR)/parms.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/ulockf.c

conn.o:		$(UUDIR)/conn.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/conn.c
 
interface.o:	$(UUDIR)/interface.c  $(UUDIR)/uucp.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/interface.c
 
sysfiles.o:	$(UUDIR)/sysfiles.c  $(UUDIR)/sysfiles.h
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/sysfiles.c
 
strsave.o:	$(UUDIR)/strsave.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/strsave.c
 
strecpy.o:	$(UUDIR)/strecpy.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/strecpy.c

stoa.o:	$(UUDIR)/stoa.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/stoa.c

push.o:	$(UUDIR)/push.c
	$(CC) -c $(CFLAGS) $(LOCAL) $(UUDIR)/push.c


ctags:
	ctags	$(SOURCE)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCE)		;\
	do echo "$$i \\" >> greplist	;\
	done
	echo /dev/null >> greplist
	chmod +x greplist

clean:
	-rm -f *.o greplist tags

clobber: clean
	rm -f ct
