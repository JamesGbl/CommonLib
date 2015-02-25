#ifndef __WEBSERVER_H
#define __WEBSERVER_H

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
Author: Leonardo Cecchi <leonardoce@interfree.it>
*/ 

/**
 * File: webserver.h
 */


#include "lerror.h"

struct webserver_t;
struct webrequest_t;
struct webresponse_t;

/**
 * Function: webserver_new
 * Create a new web server
 *
 * Parameters:
 *     address - Address of the interface that must be listening for requests
 *     port - The port number
 *     n_threads - The number of concurrent HTTP connection served
 *     docRoot - The document root
 */
struct webserver_t *webserver_new( const char *address, const char *port, int n_threads, const char *docRoot );

/**
 * Function: webserver_start
 * Start this web server
 */
void webserver_start( struct webserver_t *self, lerror **error );

/**
 * Function: webserver_destroy
 * Deallocate and stop the web server
 */
void webserver_destroy( struct webserver_t *self );

typedef void (*webservice_handler_t)( void *ctx, struct webrequest_t *req, struct webresponse_t *resp, lerror **error );

/**
 * Function: webserver_add_service
 * Register a web service, must be done before the server is started
 *
 * Parameters:
 *     self - The web server
 *     uri - The URI to manage
 *     handler - The handler
 *     ctx - Will be passed to the handler
 */     
void webserver_add_service( struct webserver_t *self, const char *uri, webservice_handler_t handler, void *ctx );

/**
 * Function: webresp_set_http_status
 * Change the HTTP status code
 */
void webresp_set_http_status( struct webresponse_t *self, int status );

/**
 * Function: webresp_set_http_status_description
 * Change the HTTP status description
 */
void webresp_set_http_status_description( struct webresponse_t *self, const char *status_desc );

/**
 * Function: webresp_get_http_status
 * Change the HTTP status code
 */
int webresp_get_http_status( struct webresponse_t *self );

/**
 * Function: webresp_set_content_type
 * Change the content_type
 */
void webresp_set_content_type( struct webresponse_t *self, const char *content_type );

/**
 * Function: webresp_write_text
 * Write a text response to the client
 */
void webresp_write_text( struct webresponse_t *conn, const char *txt );

/**
 * Function: webresp_write_lstring
 * Write a text response to the client
 */
void webresp_write_lstring( struct webresponse_t *conn, lstring *txt );

/**
 * Function: webresp_write_text_len
 * Write a text response to the client
 */
void webresp_write_text_len( struct webresponse_t *conn, const char *txt, int len );

/**
 * Function: webreq_get_param
 * Get a parameter from an HTTP request
 */
lstring *webreq_get_param_f( struct webrequest_t *req, lstring *dest, const char *paramName );

#endif
