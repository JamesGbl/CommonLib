#ifndef __COMMONLIB_NET_SOCKET_H
#define __COMMONLIB_NET_SOCKET_H

#include "lerror.h"

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

typedef struct TCPListenSocket TCPListenSocket;

/**
 * Function: TCPListenSocket_new
 * Creates a new listening socket.
 * Parameters:
 *     hostname - The IP address to use to bound the socket
 *     port - The TCP port number or the service name
 * Returns:
 *     A new listening socket already bound to the port and 
 *     listening
 */
TCPListenSocket* TCPListenSocket_new(const char *hostname, const char *port, lerror **error);

/**
 * Function: TCPListenSocket_accept
 * Accept an incoming connection from this server socket 
 * Parameters:
 *     self - The server socket
 */
void TCPListenSocket_accept(TCPListenSocket *self, lerror **error);

/**
 * Function: TCPListenSocket_delete
 * Delete all the associated resources of this socket
 * Parameters:
 *     self - The server socket
 */
void TCPListenSocket_delete(TCPListenSocket *self);

#endif
