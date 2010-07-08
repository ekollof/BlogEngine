#include "blog.h"

void
mysql_poolinit(void)
{
	pthread_t	pthread[THREADS];
	db_config	dbc;
	int		i;

	strlcpy(dbc.host, dbhost, strlen(dbhost) + 1);
	strlcpy(dbc.user, dbuser, strlen(dbuser) + 1);
	strlcpy(dbc.pass, dbpass, strlen(dbpass) + 1);
	strlcpy(dbc.name, dbname, strlen(dbname) + 1);

	dbc.port = 0;
	dbc.socket = NULL;

	if (!mysql_thread_safe()) {
		vbprintf("Using own threaded pool with %d mysql threads\n", THREADS);
	} else {
		vbprintf("Using threaded mysql connection pool of %d threads\n", THREADS);
	}

	vbprintf("DB Connections: %d, Threads: %d, Queries per Thread: %d, Total Queries: %d\n",
		 CONPOOL, THREADS, QPERTHR, THREADS * QPERTHR);

	//Pre - init
		for (i = 0; i < CONPOOL; i++) {
		dbm[i].db = mysql_conn(dbm[i].db, &dbc);
		pthread_mutex_init(&dbm[i].lock, NULL);
	}
}

MYSQL          *
mysql_conn(MYSQL * mysql, db_config * dbc)
{
	if (!(mysql = mysql_init(mysql)))
		mysql_die("mysql_init failed: %s", mysql_error(mysql));
	else {
		if (!mysql_real_connect(mysql, dbc->host, dbc->user, dbc->pass,
				    dbc->name, dbc->port, dbc->socket, 0)) {
			vbprintf("host: %s user: %s pass: %s db: %s\n",
				 dbc->host, dbc->user, dbc->pass, dbc->name);
			mysql_die("mysql_real_connect failed: %s", mysql_error(mysql));
		}
	}
	dbprintf("DB connection active.\n");
	return (mysql);
}

void
mysql_die(char *fmt,...)
{
	int		i;
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	putc('\n', stderr);

	//tear down connection pool
		for (i = 0; i < CONPOOL; i++) {
		pthread_mutex_destroy(&dbm[i].lock);
		mysql_disc(dbm[i].db);
	}
	exit(EXIT_FAILURE);
	//Might change this...
}

void
mysql_disc(MYSQL * mysql)
{
	if (mysql)
		mysql_close(mysql);
}

MYSQL_RES      *
mysql_my_query(MYSQL * mysql, pthread_mutex_t * lock, const char *query)
{
	MYSQL_RES      *res;
	long		retc;

	pthread_mutex_lock(lock);
	retc = mysql_query(mysql, query);

	if (retc) {
		pthread_mutex_unlock(lock);
		vbprintf("mysql_query problem: %s", mysql_error(mysql));
		return NULL;
	}
	res = mysql_store_result(mysql);
	pthread_mutex_unlock(lock);

	if (res) {
		return res;
	} else {
		if (mysql_field_count(mysql) == 0) {
			dbprintf("Query affected %d rows.\n", mysql_affected_rows(mysql));
		} else {
			vbprintf("MySQL problem: %s\n", mysql_error(mysql));
		}
	}
	return NULL;
}
