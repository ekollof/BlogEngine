/*
 * LOADCHECK (c) 2004,2007 Emiel Kollof <emiel@ninth-circle-alliance.net>
 * 
 * Door: Emiel Kollof
 * 
 */

#ifndef lint
static const char copyright[] =
"Copyright (c) 2004,2007 Emiel Kollof <emiel@ninth-circle-alliance.net>";
#endif

#include "blog.h"

int
main(int argc, char **argv, char **environ)
{
	int		ch;
	char           *cwd;
	char           *cbuf;
	int		lpid      , status;
	struct sigaction sa;


	while ((ch = getopt(argc, argv, "ndlv")) != -1) {
		switch (ch) {
		case 'n':
			nodaemon = 1;
			break;
		case 'd':
			debug = 1;
			verbose = 1;
			break;
		case 'l':
			debug = 1;
			verbose = 1;
			logging = 1;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'c':
			strlcpy(configfile, optarg, sizeof(configfile) - 1);
			break;
		default:
			usage(argv[0]);
			break;
		}
	}
	argc -= optind;
	argv += optind;

#ifdef NO_SETPROCTITLE
	init_setproctitle(argc, argv);
	setproctitle("blogengine: main");
#endif

	cwd = getcwd(NULL, MAXPATHLEN);
	printf("Blog Engine v%s, by Emiel Kollof\n"
	       "Current working directory: %s\n",
	       VERSION, cwd);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGABRT, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	if (nodaemon == 0) {
		int		fd        , ret = 0;
		char           *buf;

		dbprintf("Forking to background, see sysalert.pid for pid number\n");
		if (daemon(0, 0) < 0) {
			dbprintf("Daemonizing failed, aborting\n");
			exit(1);
		}

		xasprintf(&buf, "%d\n", getpid());

		fd = open("blog.pid", O_RDWR | O_CREAT, 0644);
		chmod("blog.pid", 0644);
		ret = write(fd, buf, strlen(buf) + 1);
		close(fd);
		free(buf);
	}
	cbuf = (char *)config_read(CONFIGFILE);
	if (cbuf) {
		config_feed(cbuf);
		free(cbuf);
	}
	//Start webserver thread
		web_start();
	for (;;)
		sleep(1);

}

void
usage(char *progname)
{
	printf("%s usage:\n"
	       "%s [-c <configfile>] [-ndl]\n\n"
	       "-d             Debug mode (including verbose)\n"
	       "-l		Log mode. Implies daemon-mode + debug\n"
	       "-n		Don't become a daemon\n",
	       progname, progname);
	exit(0);
}
