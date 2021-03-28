/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) statlog.c: version 25.1 created on 12/2/91 at 18:15:30	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)statlog.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"uucp:statlog.c	1.2"			*/

#ident	"@(#)uucp:statlog.c	25.1"

#include "uucp.h"

/*
	Report and log file transfer rate statistics.
	This is ugly because we are not using floating point.
*/

void
statlog( direction, bytes, millisecs )
char		*direction;
unsigned long	bytes;
time_t		millisecs;
{
	char		text[ 100 ];
	unsigned long	bytes1000;

	bytes1000 = bytes * 1000;
	/* on fast machines, times(2) resolution may not be enough */
	/* so millisecs may be zero.  just use 1 as best guess */
	if ( millisecs == 0 )
		millisecs = 1;
	(void) sprintf(text, "%s %lu / %lu.%.3u secs, %lu bytes/sec",
		direction, bytes, millisecs/1000, millisecs%1000,
		bytes1000/millisecs );
	CDEBUG(4, "%s\n", text);
	syslog(text);
}
