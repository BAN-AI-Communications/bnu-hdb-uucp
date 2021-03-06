/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysfiles.h: version 25.1 created on 12/2/91 at 18:15:51	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysfiles.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"uucp:sysfiles.h	1.4"		*/

#ident	"@(#)uucp:sysfiles.h	25.1"

#define SYSDIR		"/usr/lib/uucp"
#define SYSFILES	"/usr/lib/uucp/Sysfiles"
#define SYSTEMS		"/usr/lib/uucp/Systems"
#define DEVICES		"/usr/lib/uucp/Devices"
#define DIALERS		"/usr/lib/uucp/Dialers"
#define	DEVCONFIG	"/usr/lib/uucp/Devconfig"

#define	SAME	0
#define	TRUE	1
#define	FALSE	0
#define	FAIL	-1

/* flags to check file access for REAL user id */
#define	ACCESS_SYSTEMS	1
#define	ACCESS_DEVICES	2
#define	ACCESS_DIALERS	3

/* flags to check file access for EFFECTIVE user id */
#define	EACCESS_SYSTEMS	4
#define	EACCESS_DEVICES	5
#define	EACCESS_DIALERS	6

#ifdef lint
#define ASSERT(e, s1, s2, i1)	;

#else /* NOT lint */

#define ASSERT(e, s1, s2, i1) if (!(e)) {\
	assert(s1, s2, i1, __FILE__, __LINE__);\
	cleanup(FAIL);};
#endif

