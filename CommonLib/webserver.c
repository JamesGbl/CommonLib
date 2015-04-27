/*
Author: Leonardo Cecchi <leonardoce@interfree.it>

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
*/ 

#include "third-party/mongoose.h"
#include "webserver.h"
#include "lstring.h"
#include "lcross.h"
#include "lmemory.h"
#include "buffer.h"
#include "llogging.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_RULES 1024

struct rules_t {
    lstring *uri;
    void *ctx;
    webservice_handler_t handler;
};

struct webserver_t {
    struct mg_context *ctx;
    lstring *addr;
    lstring *docRoot;
    lstring *port;
    lstring *n_threads;

    struct rules_t rules[MAX_RULES];
    int rules_count;
};

struct webrequest_t {
    struct mg_connection *conn;
    struct mg_request_info *info;
};

struct webresponse_t {
    struct mg_connection *conn;
    int http_status;
    lstring *http_status_desc;
    lstring *content_type;
    MemBuffer *buffer;
};

/* WebRequest {{{ */

struct webrequest_t *webreq_new( struct mg_connection *conn ) {
    struct webrequest_t *result = NULL;

    l_assert( conn!=NULL );
    
    result = (struct webrequest_t *)lmalloc(sizeof(struct webrequest_t));
    result->conn = conn;
    result->info = mg_get_request_info( conn );

    return result;
}

void webreq_destroy( struct webrequest_t *self ) {
    if ( self!=NULL ) {
        lfree( self );
    }
}

lstring* webreq_get_param_f( struct webrequest_t *req, lstring *dest, const char *paramName ) {
    char buffer[1024];
    size_t bufferLen = 1024;
    int status = 0;

    l_assert( req!=NULL );
    l_assert( dest!=NULL );
    l_assert( paramName!=NULL );

    if ( req->info->query_string==NULL ) {
        /* not found */
        lstring_truncate( dest, 0 );
        return NULL;
    }

    status = mg_get_var(req->info->query_string, strlen(req->info->query_string),
               paramName, buffer, bufferLen);
    if ( status==-1 ) {
        /* not found */
        lstring_truncate( dest, 0 );
    } else if ( status==-2 ) {
        /* no sufficient space */
        lstring_truncate( dest, 0 );
    } else {
        dest = lstring_from_cstr_f( dest, buffer );
    }

	return dest;
}

lbool webreq_has_param( struct webrequest_t *req, const char *paramName ) {
    char buffer[1024];
    size_t bufferLen = 1024;
    int status = 0;

    l_assert( req!=NULL );
    l_assert( paramName!=NULL );

    if ( req->info->query_string==NULL ) {
        return LFALSE;
    }

    status = mg_get_var(req->info->query_string, strlen(req->info->query_string),
               paramName, buffer, bufferLen);
    if ( status==-1 ) {
	return LFALSE;
    } else {
	return LTRUE;
    }
}

lstring *webreq_get_required_param_f(struct webrequest_t *req, lstring *dest, const char *paramName, lerror **error) {
	lstring *result;

	l_assert(error==NULL || *error==NULL);
	l_assert(paramName!=NULL);
	l_assert(req!=NULL);
	l_assert(dest!=NULL);

	if (!webreq_has_param(req, paramName)) {
		lerror_set_sprintf(error, "Manca il parametro %s", paramName);
	}

	result = webreq_get_param_f(req, dest, paramName);
	return result;
}

/* }}} */

/* WebResponse {{{ */

struct webresponse_t *webresp_new( struct mg_connection *conn ) {
    struct webresponse_t *result = NULL;

    l_assert( conn!=NULL );
    
    result = (struct webresponse_t *)lmalloc(sizeof(struct webresponse_t));
    result->conn = conn;
    result->http_status = 200;
    result->http_status_desc = lstring_new_from_cstr( "OK" );
    result->content_type = lstring_new_from_cstr( "text/plain" );
    result->buffer = MemBuffer_new( 1024 );

    return result;
}

void webresp_destroy( struct webresponse_t *self ) {
    if ( self!=NULL ) {
        lstring_delete( self->http_status_desc );
        lstring_delete( self->content_type );
        MemBuffer_destroy( self->buffer );
        lfree( self );
    }
}

void webresp_set_http_status( struct webresponse_t *self, int status ) {
    l_assert( self!=NULL );
    self->http_status = status;
}

int webresp_get_http_status( struct webresponse_t *self ) {
    l_assert( self!=NULL );
    return self->http_status;
}

void webresp_set_http_status_description( struct webresponse_t *self, const char *status_desc ) {
    l_assert( self!=NULL && status_desc!=NULL );
    self->http_status_desc = lstring_from_cstr_f( self->http_status_desc, status_desc );
}

void webresp_set_content_type( struct webresponse_t *self, const char *content_type ) {
    l_assert( self!=NULL );
    l_assert( content_type!=NULL && content_type!=NULL );
    self->content_type = lstring_from_cstr_f( self->content_type, content_type );
}

void webresp_write_text( struct webresponse_t *conn, const char *txt )
{
    l_assert( conn!=NULL && txt!=NULL );
    MemBuffer_write( conn->buffer, (void *)txt, strlen(txt) );
}

void webresp_write_lstring( struct webresponse_t *conn, lstring *txt ) 
{
    l_assert( conn!=NULL && txt!=NULL );
    MemBuffer_write( conn->buffer, (void *)txt, lstring_len(txt) );
}

void webresp_write_text_len( struct webresponse_t *conn, const char *txt, int len )
{
    l_assert( conn!=NULL && txt!=NULL );
    MemBuffer_write( conn->buffer, (void *)txt, len );
}

static void webresp_commit( struct webresponse_t *conn ) 
{
    l_assert( conn!=NULL );
    mg_printf(conn->conn,
               "HTTP/1.1 %i %s\r\n"
               "Content-Type: %s\r\n"
               "Content-Length: %d\r\n" // Always set Content-Length
               "\r\n",
               conn->http_status, conn->http_status_desc, 
               conn->content_type, 
               MemBuffer_len(conn->buffer)); 
    mg_write(conn->conn, MemBuffer_address(conn->buffer), MemBuffer_len(conn->buffer));
}

/* }}} */


/* Web server and framework {{{ */

struct webserver_t *webserver_new( const char *address, const char *port, int n_threads, const char *docRoot ) {
    struct webserver_t *result = (struct webserver_t *)lmalloc(sizeof(struct webserver_t) );

    l_assert( address!=NULL );
    l_assert( port!=NULL );
    l_assert( docRoot!=NULL );

    result->ctx = NULL;
    result->addr = lstring_new_from_cstr( address );
    result->docRoot = lstring_new_from_cstr( docRoot );
    result->port = lstring_new_from_cstr( port );
    result->rules_count = 0;
    result->n_threads = lstring_new();
    result->n_threads = lstring_append_sprintf_f( result->n_threads, "%i", n_threads);

    return result;
}

static int handle_rule( struct webserver_t *self, struct rules_t *rule, struct mg_connection *conn ) {
    struct webresponse_t *webresp;
    struct webrequest_t *webreq;
    lerror *my_error = NULL;
    lstring *data = NULL;

    l_assert( self!=NULL );
    l_assert( rule!=NULL );
    l_assert( conn!=NULL );

    webresp = webresp_new( conn );
    webreq = webreq_new( conn );

    rule->handler( rule->ctx, webreq, webresp, &my_error );
    if ( my_error!=NULL ) {
        data = lstring_new();
        data = lerror_fill_f( my_error, data );

        webresp_write_lstring( webresp, data );
        webresp_set_http_status( webresp, 500 );
        webresp_set_http_status_description( webresp, "Internal Error" );
        lerror_delete( &my_error );
    }

    webresp_commit( webresp );

    webreq_destroy( webreq );
    webresp_destroy( webresp );

    return 1;
}

static int request_handler( struct mg_connection *conn ) {
    struct mg_request_info *info = mg_get_request_info(conn);
    struct webserver_t *self = (struct webserver_t *)(info->user_data);
    int i = 0;

    lstring *buffer = lstring_new();
    buffer = lstring_append_sprintf_f(buffer, "%s %s [%s]", info->request_method, info->uri, info->query_string);
    l_info(buffer);
    lstring_delete(buffer);

    l_assert( self!=NULL );

    for ( i=0; i<self->rules_count; i++ ) {
        if ( 0==strcmp( info->uri, self->rules[i].uri) ) {
            return handle_rule( self, &self->rules[i], conn );
        }
    }

    return 0;
}

void webserver_start( struct webserver_t *self, lerror **error ) {
    const char *options[] = {
        "document_root", self->docRoot,
        "listening_ports", self->port, 
        "num_threads", self->n_threads,
        NULL
    };

    struct mg_callbacks callbacks;

    memset( &callbacks, 0, sizeof(struct mg_callbacks) );
    callbacks.begin_request = request_handler;

    l_assert( self!=NULL );
    l_assert( error==NULL || (*error)==NULL );

    self->ctx = mg_start( &callbacks, self, options );
    if ( self->ctx==NULL ) {
        lerror_set( error, "Can't start the web server." );
        return;
    }
}

void webserver_destroy( struct webserver_t *self ) {
    l_assert( self!=NULL );

    if ( self->ctx ) mg_stop( self->ctx );
    self->ctx = NULL;
    lstring_delete( self->addr );
    lstring_delete( self->docRoot );
    lstring_delete( self->port );
    lfree( self );
}

void webserver_add_service( struct webserver_t *self, const char *uri, webservice_handler_t handler, void *ctx ) {
    l_assert( self!=NULL );
    l_assert( uri!=NULL );
    l_assert( handler!=NULL );

    if ( self->rules_count>=MAX_RULES ) return;
    self->rules[ self->rules_count ].ctx = ctx;
    self->rules[ self->rules_count ].uri = lstring_new_from_cstr( uri );
    self->rules[ self->rules_count ].handler = handler;

    self->rules_count++;
}

void webserver_wait() {
	while(1) {
		lsleep(10);
	}
}

/* }}} */
