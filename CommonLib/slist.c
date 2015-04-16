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

#include "slist.h"
#include "lvector.h"
#include "lstring.h"
#include "lmemory.h"
#include <stdlib.h>

struct slist {
	lvector *vect;
};

slist* slist_new( int initialSize ) {
	int i;
	slist* self;
    
	self = (slist *)lmalloc( sizeof(slist) );
	self->vect = lvector_new( initialSize );

	for ( i=0; i<initialSize; i++ ) {
		lvector_set( self->vect, i, NULL );
	}

	return self;
}

slist *slist_new_copy(const slist *other) {
	slist *self;

	if (other==NULL) return NULL;
	self = (slist *)lmalloc(sizeof(slist));
	self->vect = lvector_new_copy(other->vect);
	return self;
}

void slist_destroy( slist* self ) {
	int i;

	if ( !self ) return;

	for( i=0; i<lvector_len( self->vect ); i++ ) {
		lstring_delete( (lstring*) lvector_at( self->vect, i ) );
	}
	lvector_delete( self->vect );
	lfree( self );
}

int slist_len( slist* self ) {
	if ( !self ) return 0;
	return lvector_len( self->vect );
}

void slist_resize( slist* self, int newLen ) {
	int i;
	int oldLen;

	if ( !self ) return;
	oldLen = lvector_len( self->vect );

	if ( oldLen < newLen ) {
		lvector_resize( self->vect, newLen );

		for ( i=oldLen ; i<newLen; i++ ) {
			lvector_set( self->vect, i, NULL );
		}
	} else if ( oldLen > newLen ) {
		for ( i=newLen; i<oldLen; i++ ) {
			lstring_delete( (lstring*)lvector_at( self->vect, i ) );
		}

		lvector_resize( self->vect, newLen );
	}
}

const char* slist_at( slist *self, int n ) {
	lstring *s;

	if ( !self ) return NULL;
	s = (lstring*)lvector_at( self->vect, n );
	if ( !s ) return "";
	return s;
}

void slist_set( slist *self, int n, const char *str ) {
	lstring *s;

	if ( !self ) return;
	if ( n<0 || n>=lvector_len( self->vect ) ) return;
	s = (lstring*)lvector_at( self->vect, n );
	if ( !s ) {
		s = lstring_new_from_cstr(str);
		lvector_set( self->vect, n, s );
	} else {
		s = lstring_from_cstr_f( s, str );
	}
	lvector_set( self->vect, n, s );
}

