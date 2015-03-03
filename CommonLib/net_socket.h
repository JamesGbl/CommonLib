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

/**
 * Class: TCPListenSocket
 * This class represent a TCP listening socket
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
 * Function: TCPListenSocket_destroy
 * Delete all the associated resources of this socket
 * Parameters:
 *     self - The server socket (may be NULL)
 */
void TCPListenSocket_destroy(TCPListenSocket *self);

/**
 * Class: TCPListenSocket
 * This class represent a TCP socket
 */
typedef struct TCPSocket TCPSocket;

/**
 * Function: TCPSocket_new_from_fd
 * This function incapsulate a file descriptor inside
 * a TCP socket class. The file descriptor must represent
 * an opened socket.
 * Parameters:
 *     fd - File descriptor to incapsulate
 */
TCPSocket *TCPSocket_new_from_fd(int fd);

/**
 * Function: TCPSocket_send_full
 * This function send `buf_len` bytes of data from the buffer 
 * to the socket
 * Parameters:
 *     self - The socket (must be not NULL)
 *     buf - The buffer where to read the data (must be not NULL)
 *     buf_len - How much data to send (must be greater than zero)
 */
void TCPSocket_send_full(TCPSocket *self, void *buf, int buf_len, lerror **error);

/**
 * Function: TCPSocket_recv
 * This function receive data from the socket and writes it in the
 * buffer for a maximum of `len` bytes.
 * Parameters:
 *     self - The socket (must be not NULL)
 *     buf - The buffer where to write the data (must be not NULL)
 *     len - The maximum bytes of data to read
 * Return:
 *     The actual number of bytes read
 */
int TCPSocket_recv(TCPSocket *self, void *buf, int len, lerror **error);

/**
 * Function: TCPSocket_destroy
 * Closes this socket and deallocates the associated resources
 * Paremeters:
 *     self - The socket (may be NULL)
 */
void TCPSocket_destroy(TCPSocket *self);

#endif
