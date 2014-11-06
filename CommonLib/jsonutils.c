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
#include "jsonutils.h"
#include "lstring.h"
#include "lmemory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if _MSC_VER
#define snprintf _snprintf
#endif

struct JsonBuffer {
    lstring *internal;
};


JsonBuffer* JsonBuffer_new() {
    JsonBuffer *self = lmalloc(sizeof(JsonBuffer));
    self->internal = lstring_new();
    return self;
}

void JsonBuffer_destroy( JsonBuffer *self ) {
    if ( self==NULL ) return;

    lstring_delete( self->internal );
    lfree( self );
}

void JsonBuffer_startList( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "[" );
}

void JsonBuffer_endList( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "]" );
}

void JsonBuffer_startObject( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "{" );
}

void JsonBuffer_endObject( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "}" );
}

void JsonBuffer_writePropertyName( JsonBuffer *self, const char *name ) {
    JsonBuffer_writeString( self, name );
    lstring_append_cstr( self->internal, ":" );
}

void JsonBuffer_writeString( JsonBuffer *self, const char *str ) {
    wchar_t *wstring;
    size_t lunghezza;
    size_t i;

    l_assert(self!=NULL);

    if (str==NULL)
    {
        JsonBuffer_writeNull(self);
    }
    else
    {
        wstring = string1252ToWChar( str );
        lunghezza = wcslen( wstring );

        lstring_append_char( self->internal, '\"' );
        for( i=0; i<lunghezza; i++ ) {
            if ( wstring[i]>255 || !isprint(wstring[i]) ) {
                char buffer[7];
                snprintf( buffer, 7, "\\u%04x", wstring[i]);
                lstring_append_cstr( self->internal, buffer );
            } else if ( wstring[i]=='\"' ) {
                lstring_append_cstr( self->internal, "\\\"" );
            } else if ( wstring[i]=='\\' ) {
                lstring_append_cstr( self->internal, "\\\\" );
            } else {
                char c = (char)wstring[i];
                lstring_append_char( self->internal, c );
            }
        }
        lstring_append_char( self->internal, '\"' );

        lfree( wstring );
    }
}

void JsonBuffer_writeNull( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "null" );
}

void JsonBuffer_writeTrue( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "true" );
}

void JsonBuffer_writeFalse( JsonBuffer *self ) {
    lstring_append_cstr( self->internal, "false" );
}

void JsonBuffer_writeSeparator( JsonBuffer *self ) {
    lstring_append_char( self->internal, ',' );
}

int JsonBuffer_size( JsonBuffer *self ) {
    return lstring_len( self->internal );
}

const char * JsonBuffer_get( JsonBuffer *self ) {
    return lstring_to_cstr( self->internal );
}

void JsonBuffer_writeInt(JsonBuffer *self, int val)
{
	char space[64];

	l_assert(self!=NULL);
	sprintf(space, "%i", val);
	lstring_append_cstr(self->internal, space);
}

void JsonBuffer_writeBool(JsonBuffer *self, lbool value)
{
	l_assert(self!=NULL);
	if (value)
	{
		JsonBuffer_writeTrue(self);
	}
	else
	{
		JsonBuffer_writeFalse(self);
	}
}

void JsonBuffer_writeStringAttribute( JsonBuffer *self, const char *name, const char *str, lbool writeSeparator )
{
	l_assert(self!=NULL);
	l_assert(name!=NULL);

	JsonBuffer_writePropertyName(self, name);
	JsonBuffer_writeString(self, str);

	if (writeSeparator)
	{
		JsonBuffer_writeSeparator(self);
	}
}

void JsonBuffer_writeBoolAttribute(JsonBuffer *self, const char *name, lbool value, lbool writeSeparator)
{
	l_assert(self!=NULL);
	l_assert(name!=NULL);

	JsonBuffer_writePropertyName(self, name);
	JsonBuffer_writeBool(self, value);

	if (writeSeparator)
	{
		JsonBuffer_writeSeparator(self);
	}
}

void JsonBuffer_writeIntAttribute(JsonBuffer *self, const char *name, int value, lbool writeSeparator)
{
	l_assert(self!=NULL);
	l_assert(name!=NULL);

	JsonBuffer_writePropertyName(self, name);
	JsonBuffer_writeInt(self, value);

	if (writeSeparator)
	{
		JsonBuffer_writeSeparator(self);
	}
}
