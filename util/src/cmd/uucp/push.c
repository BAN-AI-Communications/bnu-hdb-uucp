/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) push.c: version 25.1 created on 12/2/91 at 18:15:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)push.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <stdio.h>
#include "uucp.h"
#include	<utmp.h>
#include	<sys/crtctl.h>
#include	<sys/stropts.h>
#define 	STREAMS 1
 

#ifdef	STREAMS
pushmodules(fd,av)
int fd; char *av[];
{
/*register strlisttab_t	*strlistptr;	 mu1	*/
extern int errno;
char *arg;

	/* joe, on-site TMAC fix. */
	if (av == NULL)
		return;

	/*	Push all stream modules specified on invocation line.
	 *	Do this before calling setupline() so that the streams
	 * 	module responds to the setupline() settings.
	 */
	while (*av != NULL) {
		arg=*av++;
		/*
		 *	Push a streams module.
		 */


		/* if I_FIND returns -1, module does not exist */
		/* if I_FIND returns 0, module exists but is not pushed */
		/* if I_FIND returns 1, module exists and is pushed */
		if (ioctl(fd,I_FIND,arg) == 0 )
		    {
		if (ioctl(fd,I_PUSH,arg) < 0)
			{
			exit(1);
			}
	    	    }
	}
}
#endif	/* STREAMS */




