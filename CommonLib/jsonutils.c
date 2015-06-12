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

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

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

struct JsonBuffer 
{
    lstring *internal;
	lbool indentFlag;
	int currentIndentLevel;
};

JsonBuffer* JsonBuffer_new() 
{
	JsonBuffer *self = (JsonBuffer *)lmalloc(sizeof(JsonBuffer));
    self->internal = lstring_new();
	self->indentFlag = LFALSE;
	self->currentIndentLevel = 0;
    return self;
}

void JsonBuffer_setIndent(JsonBuffer *self, lbool indentEnabled)
{
	l_assert(self!=NULL);

	self->indentFlag = indentEnabled;
}

lbool JsonBuffer_getIndent(JsonBuffer *self)
{
	l_assert(self!=NULL);

	return self->indentFlag;
}

static void JsonBuffer_newLineAndIndent(JsonBuffer *self)
{
	int i = 0;

	l_assert(self!=NULL);
	if (self->currentIndentLevel<0) return;

	self->internal = lstring_append_char_f(self->internal, '\n');
	for (i=0; i<self->currentIndentLevel; i++)
	{
		self->internal = lstring_append_char_f(self->internal, '\t');
	}
}

void JsonBuffer_destroy( JsonBuffer *self ) 
{
    if ( self==NULL ) return;

    lstring_delete( self->internal );
    lfree( self );
}

void JsonBuffer_startList( JsonBuffer *self ) 
{
    self->internal = lstring_append_cstr_f( self->internal, "[" );

	if (self->indentFlag)
	{
		self->currentIndentLevel++;
		JsonBuffer_newLineAndIndent(self);
	}
}

void JsonBuffer_endList( JsonBuffer *self ) 
{
	if (self->indentFlag)
	{
		self->currentIndentLevel--;
		JsonBuffer_newLineAndIndent(self);
	}

    self->internal = lstring_append_cstr_f( self->internal, "]" );
}

void JsonBuffer_startObject( JsonBuffer *self ) 
{
    self->internal = lstring_append_cstr_f( self->internal, "{" );

	if (self->indentFlag)
	{
		self->currentIndentLevel++;
		JsonBuffer_newLineAndIndent(self);
	}
}

void JsonBuffer_endObject( JsonBuffer *self ) 
{
	if (self->indentFlag)
	{
		self->currentIndentLevel--;
		JsonBuffer_newLineAndIndent(self);
	}

    self->internal = lstring_append_cstr_f( self->internal, "}" );
}

void JsonBuffer_writePropertyName( JsonBuffer *self, const char *name ) 
{
    JsonBuffer_writeString( self, name );
    self->internal = lstring_append_cstr_f( self->internal, ":" );
}

void JsonBuffer_writeString( JsonBuffer *self, const char *str ) 
{
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

        self->internal = lstring_append_char_f( self->internal, '\"' );
        for( i=0; i<lunghezza; i++ ) {
            if ( wstring[i]>255 || !isprint(wstring[i]) ) {
                char buffer[7];
                snprintf( buffer, 7, "\\u%04x", wstring[i]);
                self->internal = lstring_append_cstr_f( self->internal, buffer );
            } else if ( wstring[i]=='\"' ) {
                self->internal = lstring_append_cstr_f( self->internal, "\\\"" );
            } else if ( wstring[i]=='\\' ) {
                self->internal = lstring_append_cstr_f( self->internal, "\\\\" );
            } else {
                char c = (char)wstring[i];
                self->internal = lstring_append_char_f( self->internal, c );
            }
        }
        self->internal = lstring_append_char_f( self->internal, '\"' );

        lfree( wstring );
    }
}

void JsonBuffer_writeNull( JsonBuffer *self ) 
{
    self->internal = lstring_append_cstr_f( self->internal, "null" );
}

void JsonBuffer_writeTrue( JsonBuffer *self ) 
{
    self->internal = lstring_append_cstr_f( self->internal, "true" );
}

void JsonBuffer_writeFalse( JsonBuffer *self ) 
{
    self->internal = lstring_append_cstr_f( self->internal, "false" );
}

void JsonBuffer_writeSeparator( JsonBuffer *self ) 
{
    self->internal = lstring_append_char_f( self->internal, ',' );

	if (self->indentFlag)
	{
		JsonBuffer_newLineAndIndent(self);
	}
}

int JsonBuffer_size( JsonBuffer *self ) 
{
    return lstring_len( self->internal );
}

const char * JsonBuffer_get( JsonBuffer *self ) 
{
    return self->internal;
}

void JsonBuffer_writeInt(JsonBuffer *self, int val)
{
	char space[64];

	l_assert(self!=NULL);
	sprintf(space, "%i", val);
	self->internal = lstring_append_cstr_f(self->internal, space);
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

lstring *lstring_new_from_ujobject(UJObject object) {
	const wchar_t *data;
	size_t size;
	char *sBuffer;
	lstring *result;

	l_assert(object!=NULL);
	data = UJReadString( object, &size );
	sBuffer = stringWCharTo1252( data );
	result = lstring_new_from_cstr(sBuffer);
	lfree(sBuffer);
	return result;
}

lstring *lstring_from_ujobject_f(lstring *result, UJObject object) {
	const wchar_t *data;
	size_t size;
	char *sBuffer;

	l_assert(object!=NULL);
	data = UJReadString( object, &size );
	sBuffer = stringWCharTo1252( data );

	lstring_truncate(result, 0);
	result = lstring_from_cstr_f(result, sBuffer);

	return result;
}

lstring *lstring_from_ujstring_f(lstring *result, UJString str) {
	char *sBuffer = stringWCharTo1252( str.ptr );
	result = lstring_from_cstr_f(result, sBuffer);
	free(sBuffer);
	return result;
}

int get_ujarray_element_count(UJObject object) {
	void *iter;
	int result;
	UJObject element;

	l_assert(object!=NULL);
	l_assert(UJIsArray(object));
	
	result = 0;

	iter = UJBeginArray(object);
	while(UJIterArray(&iter, &element)) result++;
	
	return result;
}
