/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) chremdir.c: version 25.1 created on 12/2/91 at 18:13:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)chremdir.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"uucp:chremdir.c	2.2"		*/

#ident	"@(#)uucp:chremdir.c	25.1"


#include "uucp.h"

/*
 * chremdir(sys)
 * char	*sys;
 *
 * create SPOOL/sys directory and chdir to it
 * side effect: set RemSpool
 */
void
chremdir(sys)
char	*sys;
{
	int	ret;

	mkremdir(sys);	/* set RemSpool, makes sure it exists */
	DEBUG(6, "chdir(%s)\n", RemSpool);
	ret = chdir(RemSpool);
	ASSERT(ret == 0, Ct_CHDIR, RemSpool, errno);
	(void) strcpy(Wrkdir, RemSpool);
	return;
}

/*
 * mkremdir(sys)
 * char	*sys;
 *
 * create SPOOL/sys directory
 */

void
mkremdir(sys)
char	*sys;
{
	(void) sprintf(RemSpool, "%s/%s", SPOOL, sys);
	(void) mkdirs2(RemSpool, DIRMASK);
	return;
}
