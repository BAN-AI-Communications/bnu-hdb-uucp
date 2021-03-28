/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) altconn.c: version 25.1 created on 12/2/91 at 14:10:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)altconn.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"cu:altconn.c	2.3"			*/

#ident	"@(#)ct:altconn.c	25.1"
#define STREAMS 	1

#include "uucp.h"

static getto();

static struct call {		/* cu calling info-also in cu.c; */
				/* make changes in both places!*/
	char *speed;		/* transmission baud rate */
	char *line;		/* device name for outgoing line */
	char *telno;		/* ptr to tel-no digit string */
	char *class;		/* class of call */
};

/*

 * altconn - place a telephone call to system 
 * from cu when telephone number or direct line used
 *
 * return codes:
 *	FAIL - connection failed
 *	>0  - file no.  -  connect ok
 * When a failure occurs, Uerror is set.
 */

#ifdef STREAMS
altconn(call, av)
struct call *call;
char *av[];
#else
altconn(call)
struct call *call;
#endif
{
	int nf, fn = FAIL, i;
	char *alt[7];
	extern char *Myline;

	alt[F_NAME] = "dummy";	/* to replace the Systems file fields  */
	alt[F_TIME] = "Any";	/* needed for getto(); [F_TYPE] and    */
	alt[F_TYPE] = "";	/* [F_PHONE] assignment below          */
	alt[F_CLASS] = call->speed;
	alt[F_PHONE] = "";
	alt[F_LOGIN] = "";
	alt[6] = NULL;

	CDEBUG(4,"altconn called\r\n","");

	/* cu -l dev ...					*/
	/* if is "/dev/device", strip off "/dev/" because must	*/
	/* exactly match entries in Devices file, which usually	*/
	/* omit the "/dev/".  if doesn't begin with "/dev/",	*/
	/* either they've omitted the "/dev/" or it's a non-	*/
	/* standard path name.  in either case, leave it as is	*/

	if(call->line != NULL ) {
		if ( strncmp(call->line, "/dev/", 5) == 0 ) {
			Myline = (call->line + 5);
		} else {
			Myline = call->line;
		}
	}

	/* cu  ... telno */
	if(call->telno != NULL) {
		alt[F_PHONE] = call->telno;
		alt[F_TYPE] = "ACU";
	}
	/* cu direct line */
	else {
		alt[F_TYPE] = "Direct";
	}

#ifdef forfutureuse
	if (call->class != NULL)
		alt[F_TYPE] = call->class;
#endif

#ifdef STREAMS
	fn = getto(alt, av);
#else
	fn = getto(alt);
#endif

	CDEBUG(4, "getto ret %d\n", fn);

	return(fn);

}

