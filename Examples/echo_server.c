#include <stdio.h>
#include <stdlib.h>
#include "CommonLib/net_socket.h"

void panic(lerror *error) {
	lstring *buf = NULL;
	
	l_assert(error!=NULL);
	buf = lstring_new();
	buf = lerror_fill_f(error, buf);
	fprintf(stderr, "%s", buf);
	lstring_delete(buf);

	abort();
}

int main() {
	lerror *myError = NULL;
	TCPListenSocket *listeningSocket = NULL;
	TCPSocket *socket = NULL;

	listeningSocket = TCPListenSocket_new("0.0.0.0", "3233", &myError);
	if (myError!=NULL) panic(myError);

	while(1) {
		socket = TCPListenSocket_accept(listeningSocket, &myError);
		if (myError!=NULL) panic(myError);

		TCPSocket_send_string(socket, "Hello from the server!", &myError);
		if (myError!=NULL) panic(myError);

		TCPSocket_destroy(socket);
		socket = NULL;
	}

	TCPListenSocket_destroy(listeningSocket);
	return 0;
}

