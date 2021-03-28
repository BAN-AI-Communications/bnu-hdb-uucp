/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) openit.c: version 25.1 created on 12/2/91 at 16:43:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)openit.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*   @(#)Arete/5.0   6/30/84   /usr/src/cmd/arete/openit.c   */

#include <fcntl.h>
#include <termio.h>
#include <signal.h>
struct termio term = {	
	IGNBRK | IXON, 			/* iflags */
	OPOST | ONLRET | TAB3, 		/* oflags */
	/*	B9600|CS8|CREAD|HUPCL|CLOCAL,	/* cflags */
	B9600 | CS8 | CREAD | CLOCAL, 		/* cflags */
	0, 		/* lflags */
	0, 		/* line */
	0, 0, 0, 0, 1, 0, 0, 0	/* cc's */
};


main(argc, argv)
int	argc;
char	**argv;
{
	int	xonflag;
	int	baud;

	if (argc < 3) 
		usage();

	switch (atoi(argv[2])) {
	case 38400: 
		baud = EXTB; 
		break;
	case 19200: 
		baud = EXTA; 
		break;
	case 9600: 
		baud = B9600; 
		break;
	case 2400: 
		baud = B2400; 
		break;
	case 1200: 
		baud = B1200; 
		break;
	case 300: 
		baud = B300; 
		break;
	case 110: 
		baud = B110; 
		break;
	case 75: 
		baud = B75; 
		break;
	case 50: 
		baud = B50; 
		break;
	default: 
		usage();
	}

	if (fork())
		return;

	if (argc == 4)
		xonflag++;

	op(argv[1], baud, xonflag);
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	while (1) 
		pause();
}


usage() 
{
	printf("openit: usage: \"openit /dev/xxx baud_rate [xon]\"\n");
	exit(1);
}


op(s, baud, f)
{
	int	fd;
/*
	struct termio term;
*/

	if ((fd = open(s, O_WRONLY | O_NDELAY)) < 0) {
		printf("cannot open %s\n", s);
		perror("openit");
		exit(1);
	}

/*
	if(ioctl(fd,TCGETA,&term) < 0 ) {
		printf("cannot get ioctl %s\n",s);
		perror("openit");
		exit(1);
	}

	term.c_iflag = 0;
*/
	if (f) 
		term.c_iflag |= IXON;

	term.c_cflag &= ~CBAUD;
	term.c_cflag |= baud;

	if (ioctl(fd, TCSETA, &term) < 0 ) {
		printf("cannot set ioctl %s\n", s);
		perror("openit");
		exit(1);
	}
}


