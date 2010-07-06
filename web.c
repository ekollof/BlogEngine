#include "blog.h"

void
web_start(void)
{
	struct mg_context *ctx;
	char           *buf;


	/* preload header and footer */
	blog_preload();


	ctx = mg_start();
	mg_set_option(ctx, "ports", portnum);
	mg_set_option(ctx, "root", webroot);
	asprintf(&buf, "%d", dirlisting);
	mg_set_option(ctx, "dir_list", buf);
	free(buf);
	mg_set_option(ctx, "access_log", "access.log");
	mg_set_option(ctx, "error_log", "error.log");
	mg_set_uri_callback(ctx, "/blog/*", &blogpage, NULL);
	mg_set_uri_callback(ctx, "/", &indexpage_db, NULL);

	return;
}
