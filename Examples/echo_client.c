#include "../CommonLib/net_socket.h"
#include <stdio.h>
#include <stdlib.h>

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
	TCPSocket *clientSocket = NULL;
	lerror *myError = NULL;
	char buffer[1024];
	int len;
	
	clientSocket = TCPSocket_connect("127.0.0.1", "3233", &myError);
	if (myError!=NULL) panic(myError);

	while(1) {
		len = TCPSocket_recv(clientSocket, buffer, sizeof(buffer), &myError);
		if (myError!=NULL) panic(myError);

		if (len==0) {
			break;
		}
		
		fwrite(buffer, 1, len, stdout);
	}
	
	TCPSocket_destroy(clientSocket);
	
	return 0;
}
