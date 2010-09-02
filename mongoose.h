/*
 * 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * f this software and associated documentation files (the "Software"), to
 * deal
 * the rights
 * and/or sell
 * Software is
 * 
 * The above copyright notice and this permission notice shall be included in
 * l copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * PLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * UTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * ILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * UT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * HE SOFTWARE.
 * 
 * $Id: mongoose.h 404 2009-05-28 10:05:48Z valenok $
 */ 

#ifndef MONGOOSE_HEADER_INCLUDED
#define	MONGOOSE_HEADER_INCLUDED

#ifdef __cplusplus
extern		"C" {
	
#endif	/* __cplusplus */
	
	struct mg_context;	/* Handle for the HTTP service itself	 */
	               
	struct mg_connection;	/* Handle for the individual connection	 */
	               
	               
	               
	/*
	 * 
	 * t is passed to the user-specified callback function as a
	 * parameter.
	 */            
	struct mg_request_info {
		
		char           *request_method;	/* "GET", "POST", etc	 */
		               
		char           *uri;	/* Normalized URI	 */
		               
		char           *query_string;	/* \0 - terminated	 */
		               
		char           *post_data;	/* POST data buffer	 */
		               
		char           *remote_user;	/* Authenticated user	 */
		               
		long		remote_ip;	/* Client's IP address	 */
		               
		int		remote_port;	/* Client's port	 */
		               
		int		post_data_len;	/* POST buffer length	 */
		               
		int		http_version_major;
		               
		int		http_version_minor;
		               
		int		status_code;	/* HTTP status code	 */
		               
		int		num_headers;	/* Number of headers	 */
		               
		struct mg_header {
			
			char           *name;	/* HTTP header name	 */
			               
			char           *value;	/* HTTP header value	 */
			               
		}		http_headers [64];	/* Maximum 64 headers	 */
		               
	};
	               
	               
	               
	/*
	 * 
	 * handling,
	 */            
			typedef void  (*mg_callback_t) (struct mg_connection *, 
					  		const		struct	mg_request_info *info, void *user_data);
	               
	               
	               
	/*
	 * 
	 * the application.
	 * context structure that
	 * and stop the server.
	 */            
	struct mg_context *mg_start(void);
	               
	               
	               
	/*
	 * 
	 * wants to stop the web server and
	 * resources. This function blocks until all Mongoose
	 * stopped. Context pointer becomes invalid.
	 */            
	void		mg_stop   (struct mg_context *);
	               
	               
	               
	/*
	 * 
	 */            
	const char     *mg_get_option(const struct mg_context *, const char *option_name);
	               
	               
	               
	/*
	 * 
	 * copy of the option value string, which must be
	 * nul-terminated ASCII or UTF-8 string. It is safe to change any
	 * option
	 * irrelevant with
	 * listening ports, a "ssl_cert"
	 * "ports" option.
	 * mg_set_option() failed
	 */            
	int		mg_set_option(struct mg_context *, const char *opt_name, const char *value);
	               
	               
	               
	/*
	 * 
	 * function allows an application to manipulate .htpasswd files on
	 * the
	 * one of the two
	 * side. For another,
	 * examples/authentication.c in the
	 * NULL, entry is added (or modified if already exists).
	 * is NULL, entry is deleted. Return:
	 */            
	int		mg_modify_passwords_file(struct mg_context *ctx, const char *file_name, 
				      		const		char  *user_name, const char *password);
	               
	               
	               
	/*
	 * 
	 * using * in the uri_regex, which
	 * user_data pointer will be passed to the
	 * parameter. If func is NULL, then the previously installed
	 * for this uri_regex is removed.
	 */            
	void		mg_set_uri_callback(struct mg_context *ctx, const char *uri_regex, 
		       		mg_callback_t	func, void *user_data);
	               
	               
	               
	/*
	 * 
	 * function if it wants to customize the error
	 * on the browser (for example, 404 File Not Found message).
	 * possible to specify a error handler for all errors by passing 0 as
	 * rror_code. That '0' error handler must be set last, if more
	 * specific error
	 * value can be taken from
	 * to the callback.
	 */            
	void		mg_set_error_callback(struct mg_context *ctx, int error_code, 
			 		mg_callback_t	func, void *user_data);
	               
	               
	               
	/*
	 * 
	 * mechanism to implement custom authorization,
	 * based (look at examples/authorization.c).
	 * must analyze the request, and make its own judgement
	 * should be authorized or not. After the decision is made, a
	 * k must call mg_authorize() if the request is authorized.
	 */            
	void		mg_set_auth_callback(struct mg_context *ctx, const char *uri_regex, 
					mg_callback_t	func, void *user_data);
	               
	               
	               
	/*
	 * 
	 * messages to stderr. If "error_log"
	 * errors are written in the specified file. However,
	 * application registers its own log handler, Mongoose will not log
	 * nything but call the handler function, passing an error message as
	 * user_data" callback argument.
	 */            
	void		mg_set_log_callback(struct mg_context *ctx, mg_callback_t func);
	               
	               
	               
	/*
	 * 
	 * certificate asks for a password. Instead of
	 * password on a console a specified function will be called.
	 */            
			typedef int   (*mg_spcb_t) (char *buf, int num, int w, void *key);
	               
	void		mg_set_ssl_password_callback(struct mg_context *ctx, mg_spcb_t func);
	               
	               
	               
	/*
	 * 
	 * number of bytes sent is less then
	 * network error occured, usually meaning the
	 * the connection.
	 */            
	int		mg_write   (struct mg_connection *, const void *buf, int len);
	               
	               
	               
	/*
	 * 
	 * exactly like mg_write(), but allows to do message formatting.
	 *  that mg_printf() uses internal buffer of size MAX_REQUEST_SIZE
	 *  Kb by default) as temporary message storage for formatting. Do
	 * not
	 * truncated.
	 */            
	int		mg_printf  (struct mg_connection *, const char *fmt,...);
	               
	               
	               
	/*
	 * 
	 * function. It traverses request_info->http_headers array,
	 * the header is present in the array, returns its value. If it is
	 * t present, NULL is returned.
	 */            
	const char     *mg_get_header(const struct mg_connection *, const char *hdr_name);
	               
	               
	               
	/*
	 * 
	 * mg_set_auth_callback() function.
	 */            
	void		mg_authorize(struct mg_connection *);
	               
	               
	               
	/*
	 * 
	 * (whatever comes after '?' in the URL) and a POST buffer
	 * scanned. If a variable is specified in both query string and POST
	 * uffer, POST buffer wins. Return value:
	 * is not found
	 * dynamically allocated
	 * needed. It is
	 * variable.
	 */            
	char           *mg_get_var(const struct mg_connection *, const char *var_name);
	               
	               
	               
	/*
	 * 
	 */            
	void		mg_free   (char *var);
	               
	               
	               
	/*
	 * 
	 */            
	const char     *mg_version(void);
	               
	               
	               
	/*
	 * 
	 */            
	void		mg_show_usage_string(FILE * fp);
	               
	               
#ifdef __cplusplus
}              
#endif	/* __cplusplus */
               
#endif	/* MONGOOSE_HEADER_INCLUDED */