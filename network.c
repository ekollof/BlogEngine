/*
 * Copyright (c) 2003, Emiel Kollof <emiel@ninth-circle-alliance.net> All
 * rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. Neither the name of
 * the <ORGANIZATION> nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific
 * prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * Created by Emiel Kollof on Tue Nov 25 2003. Copyright (c) 2003 All rights
 * reserved.
 * 
 * $Id: network.c,v 1.1.1.1 2008/05/14 07:19:43 root Exp $
 */

#include "blog.h"
#ifdef NO_ADDRINFO
#include "getaddrinfo.h"
#endif

#ifdef WANT_LIBWRAP
#include <tcpd.h>
#include <syslog.h>
static struct request_info request;
#endif

/**
 * \brief Signal handler voor network_mainloop
 * @param s
 */

/**
 * @brief Connects to hostname on port sname (is ipv6 compatibe)
 * @param hname Host name or IP to connect to
 * @param sname Service port or name
 * @return Connection socket
 */
int
network_connect(char *hname, char *sname)
{
	struct addrinfo *res, *aip, hints;
	int		error;
	int		s = -1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	error = getaddrinfo(hname, sname, &hints, &res);
	if (error != 0) {
		vbprintf("network_connect: getaddrinfo: %s \n", gai_strerror(error));
		exit(1);
	}
	for (aip = res; aip != NULL; aip = aip->ai_next) {
		s = socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
		if (s < 0)
			continue;

		if (connect(s, aip->ai_addr, aip->ai_addrlen) == -1) {
			(void)close(s);
			s = -1;
			continue;
		}
	}
	freeaddrinfo(res);
	return s;
}

/**
 * \brief Send raw string to network socket
 * @param fd File descriptor to write to
 * @param buf Buffer to send over the network
 * @return Amount of bytes sent
 */
int
network_send(int fd, char *buf)
{
	int		bout;
	bout = 0;

	if (fd < 0) {
		vbprintf("Socket not connected...\n");
		return -1;
	}
	if ((bout = send(fd, buf, strlen(buf), 0)) < 0) {
		if (bout == 0) {
			vbprintf("netsend: socket disconnected.\n");
			close(fd);
			exit(0);
		} else {
			perror("netsend");
			exit(1);
		}
	}
	return (bout);
}

/**
 * \brief Network printf
 * @param sock File descriptor to send to
 * @param fmt fmt buffer (printf style)
 * @return Amount of bytes sent
 */
int
nprintf(int sock, char *fmt,...)
{

	va_list		ap;
	char           *buf;
	int		ret;

	va_start(ap, fmt);
	ret = vasprintf(&buf, fmt, ap);
	if (ret < 0) {
		perror("network.c: nprintf: vasprintf error");
		return -1;
	}
	va_end(ap);

	network_send(sock, buf);
	free(buf);
	return 0;

}

/**
 * \brief Read len bytes from socket fd
 * @param fd File descriptor to read from
 * @param bp Buffer to fill
 * @param len length of buffer
 * @return Amount of bytes read
 */
int
network_read(int fd, char *bp, size_t len)
{
	size_t		cnt;
	ssize_t		rc;

	cnt = len;
	while (cnt > 0) {
		rc = recv(fd, bp, cnt, 0);
		if (rc < 0) {	/* read error? */
			if (errno == EINTR)	/* interruption */
				continue;
			return -1;
		}
		if (rc == 0)	/* EOF */
			return len - cnt;
		bp += rc;
		cnt -= rc;
	}
	return (len);
}

/**
 * \brief Read a line from fd until end of file or \n is encountered
 * @param fd Filedescriptor to read from
 * @param bufptr Buffer to fill
 * @param len buffer length
 * @return Amount of bytes read
 */
int
network_gets(int fd, char *bufptr, size_t len)
{
	char           *bufx = bufptr;
	static char    *bp;
	ssize_t 	cnt = 0;
	static char	b   [1500];
	char		c;

	if (len == 0)	// prevent underflow
		return -1;

	while (--len > 0) {
		if (--cnt <= 0) {
			cnt = recv(fd, b, sizeof(b), 0);
			if (cnt < 0) {
				if (errno == EINTR) {
					len++;
					continue;
				}
				return -1;
			}
			if (cnt == 0)
				return 0;
			bp = b;
		}
		c = *bp++;
		*bufptr++ = c;
		if (c == '\n') {
			*bufptr = '\0';
			return bufptr - bufx;
		}
	}
	errno = (EMSGSIZE);
	return -1;
}
