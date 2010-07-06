/*
 * BLOGENGINE (c) 2010 Emiel Kollof <emiel@ninth-circle-alliance.net>
 * 
 * Door: Emiel Kollof <emiel@ninth-circle-alliance.net>
 * 
 * $Id: vars.c,v 1.1.1 2008/05/14 07:19:43 root Exp $
 */

#ifndef lint
static const char copyright[] =
"Copyright (c) 2010 Emiel Kollof (http://www.hackerheaven.org)";
#endif

#include "blog.h"

/* niet-extern versies van globale vars/consts */

char		configfile[MAXPATHLEN] = "blogrc";
char		logfile   [MAXPATHLEN] = "blog.log";
int		nodaemon = 0;	/* 0 == false, init done like this to shut up
				 * sunpro compiler */
int		verbose = 1;
int		debug = 1;
int		logging = 0;
int		doTidy = 0;

#if !defined(SunOS) && !defined(Linux) && !defined(FreeBSD)
#error Not Supported
#endif

char		mail_server[1024] = "mx.ninth-circle-alliance.net";
char		mail_rcpt [1024] = "emiel.kollof@gmail.com";
char		listenaddr[512] = "all";
char		portnum   [5] = "8080";
char		dbtype    [255] = "mysql";
char		dbhost    [1024] = "localhost";
char		dbuser    [1024] = "blogengine";
char		dbpass    [1024] = "bl0gm3";
char		dbname    [255] = "hackerheaven_org";
int		dbport = 3306;
char		webroot   [MAXPATHLEN] = "./www";
int		dirlisting = false;
char		template  [MAXPATHLEN] = "./www/template/template.html";
char		menu      [MAXPATHLEN] = "./www/template/menu.html";
char		filler    [MAXPATHLEN] = "./www/template/indexpage.html";
char		title     [1024] = "Hackerheaven dot ORG";
char		slogan    [1024] = "Hacking the Planet. Again...";
char           *webpage;
char           *menutext;
char           *fillertext;

MYSQL		mydb;
int		mysql_inited = 0;
int		dbconn = 0;
db_mutex	dbm    [CONPOOL];


#ifdef NO_UTENT
char           *utmpfil = "/var/run/utmp";	/* default utmp file */
FILE           *ufp = NULL;	/* file pointer to utmp file */
/* NULL = no utmp file open  */
struct utmp	ut;		/* buffer for utmp record */
#endif
