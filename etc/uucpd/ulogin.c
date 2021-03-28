#ifndef NO_SCCS_ID
#ident "@(#)ulogin.c	3.1	88/10/28"
#define NO_SCCS_ID
#endif /* NO_SCCS_ID */
/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */

/*
 * ulogin - remote login
 */
#include <sys/types.h>
#include <sys/stropts.h>
/******************************** Do we really need any of this stuff ?
#include <sys/inet.h>
#include <sys/socket.h>
#include <sys/stream.h>

#include <sys/in.h>
#include <sys/ip.h>
#include <sys/inetioctl.h>
********************************/
#include <stdio.h>
#include <termio.h>
#include <sys/fcntl.h>
#include <sys/tiuser.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <netdb.h>

/****#define t_snd write **/
/****#define t_rcv read ***/
/****#define t_rcmd rcmd ***/
#define CRLF "\r\n"

/* externals */
char	*index(), *rindex(), *malloc(), *getenv();
struct passwd *getpwuid();
extern int t_errno;

/* global variables */
char	*name;
int	rem; 
char	cmdchar = '~';
int	eight;
char	*speeds[] =
    { "0", "50", "75", "110", "134", "150", "200", "300",
      "600", "1200", "1800", "2400", "4800", "9600", "19200", "38400" };
char	term[64] = "network"; /* term type, e.g. wy50, vt100 */
int	Parent, Child;
int	oobflag = 0;
int	child;
int	catchild();
int	tabflag;

char	deferase;
char	defkill;

struct termio defterm;
struct termio noterm;

/* forward declarations */
int	reset();
int	reader();

void
usage()
{
	fprintf(stderr,
	    "usage: ulogin host [ -l username ] \n");
	exit(1);
}


main(argc, argv)
	int argc;
	char **argv;
{
	char           *host;
	char           *cp;
	struct passwd  *pwd;
	struct servent *sp;
	int            uid;
	int            options = 0;
	struct termio  ttyb;

	argv++; --argc;
	host = *argv++; --argc;
	eight = 1;

	if (argc > 0 && !strcmp(*argv, "-l")) {
		argv++; argc--;
		if (argc == 0)
			usage();
		name = *argv++; argc--;
	}
	if (host == 0)
		usage();
	if (argc > 0)
		usage();
	pwd = getpwuid(getuid());
	if (pwd == 0) {
		fprintf(stderr, "Who are you?\n");
		exit(1);
	}
	sp = getservbyname("login", "tcp");
	if (sp == 0) {
		fprintf(stderr, "ulogin: login/tcp: unknown service\n");
		exit(2);
	}
	cp = getenv("TERM");
	if (cp)
		strcpy(term, cp);
	if (ioctl(0, TCGETA, &ttyb)==0) {
		strcat(term, "/");
		strcat(term, speeds[ttyb.c_cflag&CBAUD]);
	/* DEBUG	prf( "speed = %s", speeds[ttyb.c_cflag&CBAUD]);  */
	}
	rem = t_rcmd(&host, sp->s_port, pwd->pw_name,
		name ? name : pwd->pw_name, term, 0);
	if (rem < 0) {
		perror("ulogin: t_rcmd");
                exit(1);
	}
	uid = getuid();
	if (setuid(uid) < 0) {
		perror("ulogin: setuid");
		exit(1);
	}
	doit();
	/*NOTREACHED*/
}

doit()
{
	struct termio sb;

	ioctl(0,TCGETA,&sb);
	defterm  = sb;
	tabflag  = sb.c_oflag & TABDLY;
	deferase = sb.c_cc[VERASE];
	defkill  = sb.c_cc[VKILL];

	noterm   = sb;
	noterm.c_cc[VMIN] = 1;
	noterm.c_cc[VTIME] = -1;
	noterm.c_lflag = 0;

	noterm.c_oflag &= ~( ONLCR | OCRNL | ONOCR | ONLRET | OPOST );

	noterm.c_iflag &= ~( ICRNL | INLCR | IGNCR | ISTRIP | INPCK );
	noterm.c_iflag |=  ( IGNBRK | IGNPAR );

	noterm.c_cflag &= ~( PARENB | CSIZE );
	noterm.c_cflag |= CS8;

	/* Tabdelay is same as expand tabs */
	if ((noterm.c_oflag & TABDLY) == TAB3)
		noterm.c_oflag &= ~TAB3;
	noterm.c_cc[VERASE]=noterm.c_cc[VKILL]= -1;

	signal(SIGHUP, reset);
	signal(SIGQUIT, reset);
	signal(SIGINT, SIG_IGN);
#ifdef TWO_PROCS
	Parent = getpid();
	Child = fork();
	if (Child < 0){
		perror("fork");
		exit(1);
	}
	mode(1);
	if (Child == 0){
		/*
		 * Be a reader from remote
		 */
		
		reader();
		mode(0);
		(void)kill(Parent, SIGHUP);
		(void)printf(" Connection closed..\n");
		exit(1);
	}
#else
	/* -ac set up interrupt handler first before enabling the interrupt */
	(void)sigset(SIGPOLL, reader);
	(void)fcntl(rem, F_SETFL, O_NDELAY);
  	(void)ioctl(rem, I_SETSIG, S_INPUT);
	(void)mode(1);
	(void)reader(SIGPOLL);	/* Pretend that we just got a signal */
#endif /* TWO_PROCS */
	(void)writer();
	(void)sleep(1);
	/* TWU 05/14/91 (void)prf("\007Connection closed."); */
	done();
}

done()
{

	mode(0);
#ifdef TWO_PROCS
	if (Child) (void)kill(Child, SIGHUP);
	if (Parent) (void)kill(Parent, SIGHUP);
#endif /* TWO_PROCS */
	exit(0);
}

/*
 * writer: write to remote: 0 -> line.
 */
writer()
{
	char b[BUFSIZ];
	register int  n;

	for (;;) {

		n = read(0, b, sizeof( b ) );
		if (n == 0)
			break;
		if (n < 0) {
			if (errno == EINTR)
				continue;
			prf("Bad read from terminal..\n");
			break;
		}

		if (t_snd(rem, b, n, 0) != n) {
			prf("line gone");
			return;
		}

	}
}


/*
 * Receive SIGPOLL from the remote file descriptor.
 */
/*ARGSUSED*/
#ifdef TWO_PROCS
reader()
#else
reader(sig)
#endif /* TWO_PROCS */
{
	char rb[BUFSIZ];
	register int cnt;
	int flags;
	int again;

#ifndef TWO_PROCS
	sighold(SIGPOLL);
#endif
	for (;; t_errno = flags = 0) {
		again = 0;
Retry:		cnt = t_rcv(rem, rb, sizeof(rb), &flags);
		/*
		 * A zero length read with the expedited data indication
		 * signals that there is urgent data.
		 */
		if ((flags & T_EXPEDITED) && (cnt == 0)) {
			oobflag++;
			continue;
		}
#ifdef t_rcv
		if (cnt == 0 || (cnt < 0 && errno == EAGAIN))
#else
		if (cnt == 0 || (cnt < 0 && t_errno == TNODATA))
#endif /* t_rcv */
			break;
		if (cnt < 0) {
			if (again == 0) {
				again++;
				goto Retry;
			}
			prf("Connection closed..");
#ifdef TWO_PROCS
			break;
#else
			done();
#endif /* TWO_PROCS */
			/* NOTREACHED */
		}

		(void)write(1, rb, cnt);
	}
#ifndef TWO_PROCS
	(void)sigrelse(SIGPOLL);
#else /* TWO_PROCS */
	(void)kill(Parent, SIGHUP);
#endif
}

mode(f)
{
	struct termio sb;

	switch (f) {

	case 0:
		sb = defterm;
		break;

	case 1:
		sb = noterm;
		break;

	default:
		return;
	}
	ioctl(0,TCSETAW,&sb);
}

/*VARARGS*/
prf(f, a1, a2, a3)
	char *f;
{
	fprintf(stderr, f, a1, a2, a3);
	fprintf(stderr, CRLF);
}

reset(sig){
	defterm.c_lflag |= ECHO|ECHOE|ECHOK;
	ioctl(0,TCSETAW,&defterm);
	exit(sig);
}
