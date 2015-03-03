#include "net_socket.h"
#include "lmemory.h"
#include "lcross.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_CONNECTION_BACKLOG 5

struct TCPListenSocket {
	int fd;
};

TCPListenSocket* TCPListenSocket_new(const char *hostname, const char *port, lerror **error) {
	TCPListenSocket *result = NULL;
	struct addrinfo hints, *res, *rp;
	lerror *myError = NULL;
	int rc = 0;
	int sfd = 0;
	int yes = 1;

	l_assert(hostname!=NULL);
	l_assert(port!=NULL);
	l_assert(error==NULL || *error==NULL);
	
	rc = getaddrinfo(hostname, port, &hints, &res);
	if (0!=rc) {
		lerror_set_sprintf(&myError, "TCPListenSocket", "getaddrinfo: %s", gai_strerror(rc));
		lerror_propagate(error, myError);
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
		lerror_set(&myError, "Can't bind find an address to bind");
		lerror_propagate(error, myError);
		return NULL;
	}

	rc = listen(sfd, DEFAULT_CONNECTION_BACKLOG);
	if (rc==0) {
		lerror_set_sprintf(&myError, "Can't mark this socket as listening");
		close(sfd);
		return NULL;
	}
	
	result = (TCPListenSocket *)lmalloc(sizeof(struct TCPListenSocket));
	result->fd = sfd;
	return result;
}

void TCPListenSocket_delete(TCPListenSocket *self) {
	close(self->fd);
	lfree(self);
}
