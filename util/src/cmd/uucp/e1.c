/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) e1.c: version 25.1 created on 12/2/91 at 18:13:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)e1.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/


error1(format,arg1,arg2,arg3,arg4)
char *format;
int arg1,arg2,arg3,arg4;
{
	register FILE *fp;

	if ((fp = fopen(debugfile,"a")) == NULL)
		return;
	fprintf(fp,"fp=%d ", fileno(fp));
	fprintf(fp,format,arg1,arg2,arg3,arg4);

	fclose(fp);
}






