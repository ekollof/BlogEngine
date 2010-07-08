
#include "blog.h"

void
blog_preload(void)
{
	char           *buf;

	vbprintf("Preloading data.\n");
	webpage = suckfile_mmap(template);
	menutext = suckfile_mmap(menu);
	fillertext = suckfile_mmap(filler);

	vbprintf("Done.\n");
	mysql_poolinit();
}

void
blogpage(struct mg_connection *conn,
	 const struct mg_request_info *request_info,
	 void *user_data)
{
	mg_printf(conn, "Here would be a blog...\n");
	return;
}

void
indexpage(struct mg_connection *conn,
	  const struct mg_request_info *request_info,
	  void *user_data)
{
	char           *content;
	char           *result;
	char           *tmp;

	//replace() is	leaky.Make sure pages are freed to avoid memleaks.
			content = replace(webpage, "{{{TITLE}}}", title);
	tmp = replace(content, "{{{SLOGAN}}}", slogan);
	free(content);
	content = strdup(tmp);
	free(tmp);
	tmp = replace(content, "{{{BODY}}}", fillertext);
	free(content);
	content = strdup(tmp);
	free(tmp);
	tmp = replace(content, "{{{MENU}}}", menutext);
	free(content);
	content = strdup(tmp);
	free(tmp);

	if (doTidy) {
		result = tidy_html(content);
	} else {
		result = strdup(content);
	}

	free(content);

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", result);

	free(result);
}

char           *
indexpage_db_sql(int dbconn)
{
	char           *bodyquery;
	char           *body;
	int		cancelstate;
	MYSQL_RES      *ret;
	MYSQL_ROW	row;

	xasprintf(&bodyquery, "SELECT title, content FROM content WHERE type='frontpage'");

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancelstate);
	if (mysql_thread_init() != 0)
		mysql_die("mysql_thread_init(): ", mysql_error(dbm[dbconn].db));

	ret = mysql_my_query(dbm[dbconn].db, &dbm[dbconn].lock, bodyquery);


	if (ret != NULL) {
		row = mysql_fetch_row(ret);
		xasprintf(&body, "<h3>%s</h3>\n%s", row[0], row[1]);
	} else {
		xasprintf(&body, "MYSQL_RES was NULL...  %s\n", mysql_error(dbm[dbconn].db));
	}

	free(bodyquery);
	mysql_free_result(ret);
	mysql_thread_end();

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &cancelstate);

	return body;
}

void
indexpage_db(struct mg_connection *conn,
	     const struct mg_request_info *request_info,
	     void *user_data)
{
	char           *content;
	char           *result;
	char           *tmp;
	char           *body;
	const char     *bodytext;
	const char     *pgtitle;

	//pick a connection from the connection pool
		if (dbconn == CONPOOL - 1)
		dbconn = 0;
	dbconn++;

	/* get text from db */
	body = indexpage_db_sql(dbconn);

	//replace() is	leaky.Make sure pages are freed to avoid memleaks.

			content = replace(webpage, "{{{TITLE}}}", title);
	tmp = replace(content, "{{{SLOGAN}}}", slogan);
	free(content);
	(content = strdup(tmp)) ? : err(1, "indexpage_db: strdup");
	free(tmp);
	tmp = replace(content, "{{{BODY}}}", body);
	free(content);
	(content = strdup(tmp)) ? : err(1, "indexpage_db: strdup");
	free(tmp);
	tmp = replace(content, "{{{MENU}}}", menutext);
	free(content);
	(content = strdup(tmp)) ? : err(1, "indexpage_db: strdup");
	free(tmp);

	if (doTidy) {
		result = tidy_html(content);
	} else {
		result = strdup(content);
	}

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", result);

	free(content);
	free(body);
	free(result);
}
