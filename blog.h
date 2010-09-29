
#ifndef VERSION
#define VERSION "0.01"
#endif

#if defined(__linux__) || defined(GLIBC) || defined (__CYGWIN__)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef _SVID_SOURCE
#define _SVID_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <getopt.h>
#endif

/* includes */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/statvfs.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/mman.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <regex.h>
#include <pwd.h>
#include <dirent.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>
#include <utmp.h>
#if defined(Linux) || defined(SunOS)
#include <utmpx.h>
#include <lastlog.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mysql.h>
#include "mongoose.h"

#ifdef FreeBSD
#include <err.h>
#endif

#if defined(__linux__) || defined(GLIBC)
#include <getopt.h>
#endif

#ifdef Linux
#define NO_NATIVE_STRLCPY
#define NO_SETPROCTITLE
#define NO_ERRH
#endif

#ifdef SunOS
#define NO_ASPRINTF
#define NO_DAEMON
#define NO_SETPROCTITLE
#define NO_ERRH
#endif

#ifdef FreeBSD
#define NO_UTENT
#endif


/* macros */

#define bzero(b,len) memset(b,0,len)

typedef void    (*client_hndlr) (int);


/* defines */
#define CONFIGFILE "config"
#define SYSCONFIGDIR "/etc/blogengine"
#define USRCONFIGDIR ".blogengine"

#define true 1
#define false 0

#define QPERTHR 500
#define THREADS 8
#define CONPOOL (THREADS)


#include <pthread.h>

typedef struct db_conf {
	char		host      [255];
	char		user      [255];
	char		pass      [255];
	char		name      [255];
	unsigned int	port;
	char           *socket;
}		db_config;

typedef struct db_mutex {
	MYSQL          *db;
	pthread_mutex_t	lock;
}		db_mutex;


/* constanten/globals */

extern char    *optarg;
extern int	optind, opterr, optopt;
extern char	configfile[MAXPATHLEN];
extern char	logfile[MAXPATHLEN];
extern int	nodaemon;
extern int	verbose;
extern int	debug;
extern int	doTidy;
extern char	portnum[5];
extern char	listenaddr[512];
extern int	logging;
extern char	mail_server[1024];
extern char	mail_rcpt[1024];
extern char	dbtype[255];
extern char	dbhost[1024];
extern char	dbuser[1024];
extern char	dbpass[1024];
extern char	dbname[255];
extern int	dbport;
extern char	webroot[MAXPATHLEN];
extern int	dirlisting;
extern char	template[MAXPATHLEN];
extern char	menu[MAXPATHLEN];
extern char	filler[MAXPATHLEN];
extern char	title[1024];
extern char	slogan[1024];
extern char    *webpage;
extern char    *menutext;
extern char    *fillertext;

extern MYSQL	mydb;
extern int	dbconn;
extern int	mysql_inited;
extern db_mutex	dbm[CONPOOL];

#ifdef NO_UTENT
extern char    *utmpfil;
extern FILE    *ufp;
extern struct utmp ut;
#endif


#include "proto.h"
