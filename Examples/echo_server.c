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

	puts("Echo server is accepting connections"); fflush(stdout);
	
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

