/*
About: License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>

Author: Leonardo Cecchi <mailto:leonardoce@interfree.it>
*/ 

#include "net_socket.h"
#include "lmemory.h"
#include "lcross.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_CONNECTION_BACKLOG 5

struct TCPListenSocket {	
	int fd;
	lstring *localAddress;
};

struct TCPSocket {
	int fd;
	lstring *localAddress;
	lstring *remoteAddress;
};

TCPListenSocket* TCPListenSocket_new(const char *hostname, const char *port, lerror **error) {
	TCPListenSocket *result = NULL;
	struct addrinfo hints, *res, *rp;
	int rc = 0;
	int sfd = 0;
	int yes = 1;

	l_assert(hostname!=NULL);
	l_assert(port!=NULL);
	l_assert(error==NULL || *error==NULL);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	rc = getaddrinfo(hostname, port, &hints, &res);
	if (0!=rc) {
		lerror_set_sprintf(error, "getaddrinfo: %s", gai_strerror(rc));
		return NULL;
	}

	for (rp = res; rp!=NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd==(-1)) {
			// Let's try the next one
			continue;
		}

		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))==(-1)) {
			// Can't reuse socket address. Why?
			continue;
		}

		if (0==bind(sfd, rp->ai_addr, rp->ai_addrlen)) {
			// Success!
			break;
		}

		close(sfd);
	}

	freeaddrinfo(res);
	if (rp==NULL) {
		lerror_set(error, "Can't bind find an address to bind");
		return NULL;
	}

	rc = listen(sfd, DEFAULT_CONNECTION_BACKLOG);
	if (rc!=0) {
		lerror_set_sprintf(error, "Can't mark this socket as listening");
		close(sfd);
		return NULL;
	}
	
	result = (TCPListenSocket *)lmalloc(sizeof(struct TCPListenSocket));
	result->fd = sfd;
	result->localAddress = lstring_new();
	result->localAddress = lstring_append_sprintf_f(result->localAddress, "%s:%i", hostname, port);
	return result;
}

TCPListenSocket* TCPListenSocket_new_from_fd(int fd, const char *localAddress) {
	TCPListenSocket *result = NULL;
	result = (TCPListenSocket *)lmalloc(sizeof(struct TCPListenSocket));
	result->fd = fd;
	result->localAddress = lstring_new_from_cstr(localAddress);
	return result;
}

TCPSocket *TCPListenSocket_accept(TCPListenSocket *self, lerror **error) {
	int rc = 0;
	char *ip = NULL;
	unsigned int len = sizeof(struct sockaddr_storage);
	struct sockaddr_storage remoteAddr;
	TCPSocket *result = NULL;
	
	l_assert(self!=NULL);
	l_assert(error==NULL || *error==NULL);

	rc = accept(self->fd, (struct sockaddr *)&remoteAddr, &len);
	if (rc==(-1)) {
		lerror_set(error, "Can't accept connections on this socket");
		return result;
	}
			
	ip = inet_ntoa(((struct sockaddr_in *)&remoteAddr)->sin_addr);
	result = TCPSocket_new_from_fd(rc, self->localAddress, ip);
	return result;
}

void TCPListenSocket_destroy(TCPListenSocket *self) {
	if (self==NULL) return;
	
	close(self->fd);
	lstring_delete(self->localAddress);
	lfree(self);
}

TCPSocket *TCPSocket_new_from_fd(int fd, const char *localAddress, const char *remoteAddress) {
	TCPSocket *self = (TCPSocket *)lmalloc(sizeof(struct TCPSocket));
	self->fd = fd;
	self->localAddress = lstring_new_from_cstr(localAddress);
	self->remoteAddress = lstring_new_from_cstr(remoteAddress);
	return self;
}

TCPSocket *TCPSocket_connect(const char *hostname, const char *port, lerror **error) {
	struct addrinfo hints, *res;
	int rc, sfd;
	TCPSocket *result = NULL;

	res = NULL;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	rc = getaddrinfo(hostname, port, &hints, &res);
	if (0!=rc) {
		lerror_set_sprintf(error, "getaddrinfo: %s", gai_strerror(rc));
		goto end;
	}

	sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if ((-1)==sfd) {
		lerror_set(error, "Can't create connection socket.");
		goto end;
	}

	rc = connect(sfd, res->ai_addr, res->ai_addrlen);
	if ((-1)==rc) {
		lerror_set_sprintf(error, "Can't connect to host: %s", strerror(errno));
		close(sfd);
		goto end;
	}

	result = TCPSocket_new_from_fd(sfd, "local", hostname);
	
end:
	if (res!=NULL) {
		freeaddrinfo(res);
	}
	return result;
}

int TCPSocket_recv(TCPSocket *self, void *buf, int len, lerror **error) {
	int result = 0;
	
	l_assert(self!=NULL);
	l_assert(error==NULL || *error==NULL);
	l_assert(len>0);

	result = recv(self->fd, buf, len, 0);
	if (result==(-1)) {
		lerror_set(error, "Can't read data from stream");
	}
	return result;
}

void TCPSocket_send_full(TCPSocket *self, void *buf, int buf_len, lerror **error) {
	int bytes_sent = 0;
	int rc = 0;
	
	l_assert(self!=NULL);
	l_assert(buf!=NULL);
	l_assert(buf_len>0);
	l_assert(error==NULL || *error==NULL);

	while (bytes_sent!=buf_len) {
		rc = send(self->fd, buf+bytes_sent, buf_len-bytes_sent, 0);
		if (rc==(-1)) {
			lerror_set(error, "Cannot send bytes to this socket");
			break;
		} else if (rc==0) {
			lerror_set(error, "Socket closed connection");
			break;
		} else {
			bytes_sent = bytes_sent + rc;
		}
	}
}

void TCPSocket_send_string(TCPSocket *self, const char *buf, lerror **error) {
	lerror *myError = NULL;
	
	l_assert(self!=NULL);
	l_assert(buf!=NULL);
	l_assert(error==NULL || *error==NULL);

	TCPSocket_send_full(self, (void *)buf, strlen(buf), &myError);
	lerror_propagate(error, myError);
}

void TCPSocket_destroy(TCPSocket *self) {
	if (self==NULL) return;
	close(self->fd);
	lstring_delete(self->localAddress);
	lstring_delete(self->remoteAddress);
	lfree(self);
}
