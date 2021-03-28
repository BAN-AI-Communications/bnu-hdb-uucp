/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pkdefs.c: version 25.1 created on 12/2/91 at 18:15:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pkdefs.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"uucp:pkdefs.c	2.2"			*/

#ident	"@(#)uucp:pkdefs.c	25.1"


#include "uucp.h"

#define USER 1
#include "pk.h"
char next[8]	={ 1,2,3,4,5,6,7,0};	/* packet sequence numbers */
char mask[8]	={ 1,2,4,010,020,040,0100,0200 };
int	pkactive;
int pkdebug;
int pksizes[] = {
	1, 32, 64, 128, 256, 512, 1024, 2048, 4096, 1
};
struct pack *pklines[NPLINES];
