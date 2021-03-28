/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) strpbrk.c: version 25.1 created on 12/2/91 at 18:15:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)strpbrk.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"uucp:strpbrk.c	2.2"			*/

#ident	"@(#)uucp:strpbrk.c	25.1"


#include "uucp.h"

/*
 * Return ptr to first occurance of any character from `brkset'
 * in the character string `string'; NULL if none exists.
 */

char *
strpbrk(string, brkset)
register char *string, *brkset;
{
	register char *p;

	if(!string || !brkset)
		return(0);
	do {
		for(p=brkset; *p != '\0' && *p != *string; ++p)
			;
		if(*p != '\0')
			return(string);
	}
	while(*string++);
	return(0);
}
