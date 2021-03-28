#ifndef  NO_SCCS_ID
static char SCCS_ID [] = "@(#)uucpd.c	3.2	88/10/28";
#define NO_SCCS_ID
#endif /* NO_SCCS_ID */

#ifdef DEBUG
#define DEBUG 1                 /* DEBUGGING */
#include <stdio.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/tty_pty.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/termio.h>
#include <netdb.h>
#include <pwd.h>

/* --- local defines ---- */
#define until(x) while(!(x))
#define STDIN  0
#define STDOUT 1
#define STDERR 2

#ifndef NULL
#define NULL 0
#endif

/* externs */
/* WARNING: The following external declaration SHOULD be in <pwd.h> but 
 *          isn't (in s90_3.0_P8.1).  If and when it gets put into <pwd.h>,
 *          the following declaration will need to be deleted.
 */
extern struct passwd * getpwnam();
extern char	*strtok();

/* forward procedure declarations */
int	cleanup();
void	fatal();
void	fatalperror();

/* global variables */

int      err_fd = STDERR;
int      pty_portno;
int      master_pty;
int      slave_pty;
char	 master_dev[32];
char	 slave_dev[32];

#ifdef DEBUG
FILE    *file;
#endif

#ifdef DEBUG
#define trace(x,y,z)  {fprintf(x,y,z);fflush(x);}
#else
#define trace(x,y,z)  {}
#endif /* DEBUG */


void
become_user_uucp()
{
	struct passwd *pw;
	static char   user_name[] = "uucp";

	if ((pw = getpwnam( user_name )) == (struct passwd *)NULL)
		fatal( err_fd, "user uucp not in /etc/passwd" );
	if ( setgid( pw->pw_gid ) )
		fatalperror( err_fd, "setgid failed", errno );
	if ( setuid( pw->pw_uid ) )
		fatalperror( err_fd, "setuid failed", errno );
}

void
open_master_pty()
{
	struct stat stb;

	strcpy( master_dev, "/dev/ptypXXX" );
	sprintf(&master_dev[9], "%.3d", pty_portno);
	if (stat(master_dev, &stb) < 0) {
		fatal(err_fd,"Psuedo tty port does not exist");
	}
	do {
		master_pty = open( master_dev, O_RDWR/*|O_NDELAY*/);
		if (master_pty < 0) {
			nonfatalperror(err_fd, "Pseudo tty port in use", errno);
			sleep( 5 );
		}
	} while (master_pty < 0);
}

void
open_slave_pty()
{

	strcpy( slave_dev, master_dev );
	slave_dev[5] = 't';	/* /dev/ptyp0 ==> /dev/ttyp0 */
	slave_pty = open(slave_dev, O_RDWR/*|O_NDELAY*/);
	if (slave_pty < 0)
		fatalperror(err_fd, master_dev, errno);
}

/*
 *  like fgets but doesn't use the stdio package
 */
char *
fdgets( buf, nbyte, fildes )
	         char *	buf;
	register uint	nbyte;
	register int	fildes;
{
	register char * bufp = buf;
	register int	count = 0;
	register int	n;
	char	 	localbuf[4];

	while ((n = read(fildes, localbuf, 1)) != -1
	&&       localbuf[0] != '\r'
	&&       localbuf[0] != '\n'
	&&	 count < nbyte       ) {
		if (n == 0) 
			return ((char *)0);
		*bufp++ = localbuf[0];
		++count;
	}
	if (count && count < nbyte ) 
		*bufp = 0;
	if ( n == -1 )
		return ((char *)0);
	return buf;
}

void
doit()
{
	char     *cptr;
	int      child_pid;
	int      wait_pid;
	char	 *host_name, *user_name;
	char	 b[32];
	static char *av[5];

#ifdef again
	signal(SIGINT, SIG_IGN);	/* Ignore Keyboard signals */
	signal(SIGQUIT,SIG_IGN);
#endif
	open_master_pty();
	cptr = fdgets(b, sizeof b, master_pty );
	if (cptr == (char *)0) {
		fatalperror( err_fd, "fgets", errno );
	}
	/* prepare args for ulogin */
	av[0] = "/usr/etc/ulogin";
	av[1] = strtok(cptr, " \n\t");
	if ((av[2] = strtok(NULL, " \n\t")) != NULL
	    && (av[3] = strtok(NULL, " \n\t")) == NULL) {
		av[3] = av[2];
		av[2] = "-l";
	}

#ifndef DEBUG
	child_pid = fork();
	if (child_pid < 0)
		fatalperror(err_fd, "Fork Failure", errno);
	if (child_pid) { /* PARENT */
		signal(SIGCLD, SIG_IGN );
		close( master_pty ); /* child still has it open */

		trace(file, "Waiting for death of child.\n",0);
		do {
			wait_pid = wait( (int *)0 );
		} until ( wait_pid == -1 && errno == ECHILD );
		sleep(3);
		return;
	}  /* end of parent */
#endif /* DEBUG */
	/*
	 * In the child exec netlogin
	 */
	dup2(master_pty, STDIN ); /* ptyp0 */
	dup2(master_pty, STDOUT);
     /* dup2(master_pty, STDERR); */
	close(master_pty);
	signal(SIGCLD, SIG_DFL);
#ifdef DEBUG
	fprintf(file,"uucpd: exec /usr/etc/ulogin %s %s %s\n", av[1] ? av[1]:0,
		av[2] ? av[2] : 0, av[3] ? av[3] : 0);
	fflush(file);
#endif /* DEBUG */
	execv(av[0], av);
	fatalperror(err_fd, "ulogin", errno);
	/*NOTREACHED*/
}

/*
 */
main(argc, argv)
	int argc;
	char **argv;
{

#ifdef DEBUG
	file = fopen("/tmp/uloglog", "a+");
	fprintf(file,"main: opening /tmp/uloglog, file=0x%x\n",file);
	fflush(file);
#endif /* DEBUG */

	if ( argc < 2 ) pty_portno = last_pty();
	else {
		pty_portno = atoi( argv[1] );
	}
	become_user_uucp();
	open_master_pty();	/* just to be sure */
	open_slave_pty();
	close( slave_pty );
	close( master_pty );

	/*
	 * Put myself in the back-ground.
	 */
#ifndef DEBUG
	if (fork())
		exit (0);
	setpgrp();
	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGHUP, SIG_IGN);
#endif /* DEBUG */

	for (;;) {
		/* stay alive forever */
		doit();
	}

}

int
cleanup()
{

	/*
	 * Instead of vhangup(), just send SIGHUP to
	 * my process group
	 */
	kill(0,SIGHUP);
	kill(-getpid(), SIGHUP);
	wait(0);	/* So that the Zombie will go away */
	exit(1);
}

void
fatal(err_fd, msg)
	int err_fd;
	char *msg;
{
	char buf[132];

	(void) sprintf(buf, "uucpd: %s.\r\n", msg);
	(void) write(err_fd, buf, strlen(buf));
	exit(1);
}

void
fatalperror(err_fd, msg, errno)
	int  err_fd;
	char *msg;
	int  errno;
{
	char buf[132];
	extern char *sys_errlist[];

	(void) sprintf(buf, "%s: %s", msg, sys_errlist[errno]);
	fatal(err_fd, buf);
}

nonfatalperror(err_fd, msg, errno)
	int  err_fd;
	char *msg;
	int  errno;
{
	char buf[132];
	extern char *sys_errlist[];

	(void) sprintf(buf, "uucpd: %s: %s.\r\n", msg, sys_errlist[errno]);
	(void) write(err_fd, buf, strlen(buf));
}

int
last_pty()
{
	int base = 0;
	int inc = 100;
	char *line = "/dev/ptyp000";
	struct stat stb;

	if (stat(line, &stb) < 0)
		return -1;

	while(inc) {
		while (stat(line, &stb) == 0) {
			base += inc;
			sprintf(&line[9], "%.3d", base);
		}
		base -= inc;
		inc = inc / 10;
		base += inc;
		sprintf(&line[9], "%.3d", base);
	}
	return base;
}
