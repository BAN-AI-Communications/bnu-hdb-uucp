/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) versys.c: version 25.1 created on 12/2/91 at 18:17:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)versys.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"uucp:versys.c	2.3"			*/

#ident	"@(#)uucp:versys.c	25.1"


#include "uucp.h"

/*
 * verify system name
 * input:
 *	name	-> system name
 * returns:  
 *	0	-> success
 *	FAIL	-> failure
 */
versys(name)
char *name;
{
	register char *iptr;
	char line[300];

	if (name == 0 || *name == 0)
		return(FAIL);

	if (EQUALS(name, Myname))
		return(0);

	while (getsysline(line, sizeof(line))) {
		if((line[0] == '#') || (line[0] == ' ') || (line[0] == '\t') || 
			(line[0] == '\n'))
			continue;

		if ((iptr=strpbrk(line, " \t")) == NULL)
		    continue;	/* why? */
		*iptr = '\0';
		if (EQUALS(name, line)) {
			sysreset();
			return(0);
		}
	}
	sysreset();
	return(FAIL);
}
