#	START NEW ARIX SCCS HEADER
#
#	@(#) bnu.admin.mk: version 25.1 created on 12/2/91 at 13:53:40
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)bnu.admin.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:#ident	"bne.admin:bne.admin.mk	2.3"

#ident	"@(#)bne.admin:bnu.admin.mk	25.1"

#
#	This makefile makes appropriate directories
#	and copies the simple admin shells into
#	the uucpmgmt directory
#

include $(IROOT)/macdefs

OWNER=uucp
GRP=daemon
MGMT=$(ROOT)/usr/admin/menu/packagemgmt/uucpmgmt


install:
	if [ ! -d  $(ROOT)/usr/admin ] ;\
	then \
		mkdir $(ROOT)/usr/admin ;\
	fi
	if [ ! -d  $(ROOT)/usr/admin/menu ] ;\
	then \
		mkdir $(ROOT)/usr/admin/menu ;\
	fi
	if [ ! -d  $(ROOT)/usr/admin/menu/packagemgmt ] ;\
	then \
		mkdir $(ROOT)/usr/admin/menu/packagemgmt ;\
	fi
	if [ ! -d  $(MGMT) ] ;\
	then \
		mkdir $(MGMT) ;\
	fi
	$(INS) DESC $(MGMT)/DESC
	$(INS) adddevice $(MGMT)/adddevice
	$(INS) addpoll $(MGMT)/addpoll
	$(INS) addport $(MGMT)/addport
	$(INS) addsystem $(MGMT)/addsystem
	$(INS) deldevice $(MGMT)/deldevice
	$(INS) delpoll $(MGMT)/delpoll
	$(INS) delport $(MGMT)/delport
	$(INS) delsystem $(MGMT)/delsystem
	$(INS) devicemgmt $(MGMT)/devicemgmt
	$(INS) lsdevice $(MGMT)/lsdevice
	$(INS) lspoll $(MGMT)/lspoll
	$(INS) lsport $(MGMT)/lsport
	$(INS) lssystem $(MGMT)/lssystem
	$(INS) modifyport $(MGMT)/modifyport
	$(INS) pollmgmt $(MGMT)/pollmgmt
	$(INS) portmgmt $(MGMT)/portmgmt
	$(INS) systemmgmt $(MGMT)/systemmgmt
	$(INS) trysystem $(MGMT)/trysystem
	$(INS) _delinittab $(MGMT)/_delinittab
	$(INS) _initprint $(MGMT)/_initprint
	$(INS) _systemprint $(MGMT)/_systemprint
	$(INS) _deviceprint $(MGMT)/_deviceprint
	chgrp $(GRP) $(MGMT)/*
	chmod 555 $(MGMT)/[a-Z]*
	chown $(OWNER) $(MGMT)/*
	chmod 755 $(MGMT)
	chgrp $(GRP) $(MGMT)
	chown $(OWNER) $(MGMT)
	chmod 755 $(ROOT)/usr/admin/menu/packagemgmt
	chgrp bin $(ROOT)/usr/admin/menu/packagemgmt
	chown root $(ROOT)/usr/admin/menu/packagemgmt
	chmod 755 $(ROOT)/usr/admin/menu
	chgrp bin $(ROOT)/usr/admin/menu
	chown root $(ROOT)/usr/admin/menu
	chmod 755 $(ROOT)/usr/admin
	chgrp bin $(ROOT)/usr/admin
	chown root $(ROOT)/usr/admin
	chmod 775 $(ROOT)/usr
	chgrp sys $(ROOT)/usr
	chown root $(ROOT)/usr

all:

clean clobber:
