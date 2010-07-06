/*
 * config_vars.h
 */

/*
 * EXPLANATION:
 * 
 * COMPARE_INT(cvar, fvar) COMPARE_BOOL(cvar, fvar) COMPARE_DOUBLE(cvar, fvar)
 * COMPARE_STRING(cvar, fvar)
 * 
 * PLEASE: Check what you are doing! Errors in this file means compilation
 * breakages.
 */

/* network stuff */
COMPARE_STRING(listenaddr, "listen_address");
COMPARE_STRING(portnum, "listen_port");

/* program flow vars */
COMPARE_STRING(logfile, "logfile");
COMPARE_BOOL(nodaemon, "nodaemon");
COMPARE_BOOL(verbose, "verbose");
COMPARE_BOOL(logging, "logging");
COMPARE_BOOL(debug, "debug");

/* database vars */
COMPARE_STRING(dbtype, "dbtype");
COMPARE_STRING(dbhost, "dbhost");
COMPARE_STRING(dbhost, "dbuser");
COMPARE_STRING(dbpass, "dbpass");
COMPARE_STRING(dbname, "dbname");
COMPARE_INT(dbport, "dbport");

/* webserver config */
COMPARE_STRING(webroot, "webroot");
COMPARE_BOOL(dirlisting, "dirlisting");

/* website config */
COMPARE_BOOL(doTidy, "tidy");
COMPARE_STRING(template, "template");
COMPARE_STRING(menu, "menu");
COMPARE_STRING(title, "title");
COMPARE_STRING(slogan, "slogan");
