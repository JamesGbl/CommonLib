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
#include "lstring.h"
#include "lcross.h"
#include "lmemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

lstring* lstring_new( void )
{
	lstring_header* str_header = (lstring_header *)lmalloc(32);
	lstring *str = ((lstring *)str_header) + sizeof(lstring_header);
	str_header->bufLen = 32-sizeof(struct lstring_header);
	str_header->len = 0;
	str[0]='\x0';
	return str;
}

lstring* lstring_new_from_cstr( const char *cstr )
{
	int len = strlen(cstr);	
	int bufLen = 2<<l_log2(len + 50 + sizeof(struct lstring_header));
	lstring_header* str = (lstring_header *)lmalloc(bufLen);
	lstring* s = ((char *)str)+sizeof(lstring_header);
	str->len = len;
	str->bufLen = bufLen - sizeof(struct lstring_header);
	l_strcpy(s, cstr);
	return s;
}    

lstring* lstring_new_from_lstr( lstring* self )
{
	lstring_header *self_header = (lstring_header*)(self-sizeof(struct lstring_header));
	lstring_header *str = (lstring_header *)lmalloc(sizeof(struct lstring_header) + self_header->len);
	lstring* s = ((char *)str)+sizeof(lstring_header);
	memcpy(str, self_header, self_header->bufLen + sizeof(struct lstring_header));
	return s;
}

lstring *lstring_from_cstr_f( lstring* str, const char *other )
{
	lstring_truncate( str, 0 );
	if ( other!=NULL ) {
		return lstring_append_generic_f( str, other, strlen(other) );
	} else {
		return str;
	}
}

lstring* lstring_from_lstr_f( lstring* str, lstring *other )
{
	lstring_header *other_header = (lstring_header*)(other-sizeof(struct lstring_header));

	l_assert( str!=NULL );
	l_assert( other!=NULL );

	lstring_truncate( str, 0 );
	return lstring_append_generic_f( str, other, other_header->len );
}

void lstring_delete( lstring* str )
{
	if (str==NULL) return;
	lfree(str-sizeof(struct lstring_header));
}    

lstring *lstring_append_generic_f( lstring* str, const char *other, int otherLen)
{
	lstring_header *str_header = NULL;
	int newLen = 0;
	int newBufLen = 0;

	l_assert(otherLen>=0);
	if (otherLen==0) return str;

	str_header = (lstring_header *)(str - sizeof(lstring_header));
	newLen = otherLen + str_header->len;
	newBufLen = 2<<l_log2(newLen + 50);

	if( str_header->bufLen > newBufLen )
	{
		newBufLen = str_header->bufLen;
	}    

	if( str_header->bufLen<newBufLen )
	{
		str_header = (lstring_header *)lrealloc(str_header, newBufLen+sizeof(lstring_header));
		str = ((lstring*)str_header)+sizeof(lstring_header);
	}    

	memcpy( str+str_header->len, other, otherLen );
	str[str_header->len+otherLen]=0;
	str_header->len = newLen;
	str_header->bufLen = newBufLen;

	return str;
}

lstring *lstring_append_cstr_f( lstring* str, const char *other )
{
	int otherLen;

	l_assert( str!=NULL );
	l_assert( other!=NULL );

	otherLen = strlen(other);
	return lstring_append_generic_f( str, other, otherLen );
}

lstring* lstring_append_char_f( lstring* str, char c ) {
	l_assert( str!=NULL );

	return lstring_append_generic_f( str, &c, 1 );
}

lstring* lstring_append_lstring_f( lstring* str, lstring* other )
{
	lstring_header *other_header = (lstring_header *)(other - sizeof(lstring_header));
	l_assert( str!=NULL );
	l_assert( other!=NULL );
	return lstring_append_generic_f( str, other, other_header->len );
}    

lstring* lstring_reserve_f( lstring* str, int len )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	l_assert( str!=NULL );
	if( str_header->bufLen<len )
	{
		str_header = (lstring_header *)lrealloc(str_header, len+sizeof(lstring_header));
		str = ((lstring*)str_header)+sizeof(lstring_header);
	}

	return str;
}    

void lstring_ltrim( lstring* str )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	int i;

	l_assert( str!=NULL );

	if( str_header->len==0 ) return;

	i = 0;
	while( str[i] != 0 && isspace(str[i]) )
	{
		i++;
	}

	if( i>0 )
	{    
		str_header->len = str_header->len - i;
		memmove( str, str+i, str_header->len );
		str[str_header->len] = 0;
	}
}

void lstring_drop_left(lstring *str, int l) {
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

	l_assert(str!=NULL);
	l_assert(l>=0);

	if (l==0) {
		return;
	}

	if (l>lstring_len(str)) {
		lstring_truncate(str, 0);
		return;
	}

	str_header->len = str_header->len - l;
	memmove( str, str+l, str_header->len );
	str[str_header->len] = 0;
}

void lstring_rtrim( lstring* str )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

	l_assert( str!=NULL );
	if( str_header->len==0 ) return;

	while( str_header->len > 0 && isspace(str[str_header->len-1]) )
	{
		str_header->len = str_header->len-1;
	}  

	str[str_header->len] = 0;
}

void lstring_trim( lstring* str )
{
	l_assert( str!=NULL );

	lstring_rtrim( str );
	lstring_ltrim( str );
}    

int lstring_contains( const lstring* str, const char *other )
{
	l_assert( str!=NULL );
	l_assert( other!=NULL );

	if( strstr( str, other )!=NULL )
	{
		return 1;
	} else
	{
		return 0;
	}    
}

int lstring_equals_cstr( lstring* str, const char *cstr )
{
	l_assert( str!=NULL );
	l_assert( cstr!=NULL );

	if( 0==strcmp(str, cstr) )
	{
		return 1;
	} else
	{
		return 0;
	}
}    

int lstring_equals_lstr( lstring* str_1, lstring* str_2 )
{
	lstring_header *str1_header = (lstring_header *)(str_1 - sizeof(lstring_header));
	lstring_header *str2_header = (lstring_header *)(str_2 - sizeof(lstring_header));

	l_assert( str_1!=NULL );
	l_assert( str_2!=NULL );

	if( str_1==NULL )
	{
		return str_2==NULL;
	} else if( str_2==NULL )
	{
		return 0;
	} else if( str1_header->len != str2_header->len )
	{
		return 0;
	} else if( str_1==str_2 )
	{
		return 1;
	} else if( 0==memcmp(str_1, str_2, str1_header->len) )
	{
		return 1;
	} else
	{
		return 0;
	}
}

void lstring_toupper( lstring* str )
{
	int i;
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

	if( str!=NULL ) {
		for ( i=0; i<str_header->len; i++ ) {
			str[i] = toupper( str[i] );
		}
	}    
}

void lstring_tolower( lstring* str )
{
	int i;
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));

	if( str!=NULL ) {
		for ( i=0; i<str_header->len; i++ ) {
			str[i]=tolower(str[i]);
		}
	}    
}    

void lstring_replacechar( lstring* str, char orig, char dest )
{
	char *buf = str;

	if( buf!=NULL )
	{
		while( (*buf)!=0 )
		{
			if( (*buf)==orig )
			{
				(*buf)=dest;
			}
			buf = buf+1;
		}
	}
}

int lstring_ends_with_cstr( lstring* str, const char *suffix )
{
	char *temp;
	int tempLen;
    
	if( str==NULL )
	{
		return 0;
	} else
	{
		temp = strstr( str, suffix );
		tempLen = strlen( suffix );
		if( temp==(str + lstring_len(str) - tempLen ) )
		{
			return 1;
		} else
		{
			return 0;
		}
	}
}

int lstring_last_index_of( lstring* str, char c )
{
	int i;

	for( i=lstring_len(str)-1; i>=0; i-- )
	{
		if( str[i]==c )
		{
			return i;
		}    
	}

	return -1;
}

void lstring_truncate( lstring* str, int l )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	l_assert( str!=NULL );
	l_assert( l>=0 );

	if( l<=str_header->len )
	{
		str[l] = '\x0';
		str_header->len = l;
	}    
}

void lstring_reset( lstring* str )
{
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	str_header->len=0;
	str[0]='\x0';
}

int lstring_len( const lstring* str ) {
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	return str_header->len;
}

lstring* lstring_append_sprintf_f( lstring *str, const char *format, ... ) {
	va_list args;
	char buffer[1024];

	va_start( args, format );
	l_vsnprintf( buffer, 1023, format, args );
	va_end( args );

	buffer[1023] = '0';
	return lstring_append_cstr_f( str, buffer );
}

lbool lstring_empty(lstring *str) {
	return lstring_len(str) == 0;
}

int lstring_index_of_char(lstring *str, int startidx, char p) {
	lstring_header *str_header = (lstring_header *)(str - sizeof(lstring_header));
	int i;

	l_assert(str!=NULL);
	l_assert(startidx>=0);

	if (startidx >= str_header->len) {
		return -1;
	}

	for (i=startidx; i<str_header->len; i++) {
		if (str[i]==p) {
			return i;
		}
	}

	return -1;
}

lstring *lstring_replace_all_f(lstring *str, const char *needle, const char *replacement) {
	int startidx = 0;
	int idx = 0;
	lstring *result = NULL;

	l_assert(str!=NULL);
	l_assert(needle!=NULL);
	l_assert(replacement!=NULL);

	if (0==strlen(needle)) {
		return str;
	}

	result = lstring_new();

	while (1) {
		idx = lstring_find_substring(str, needle, startidx);
		if (idx==(-1)) {
			result = lstring_append_cstr_f(result, str+startidx);
			break;
		} else {
			result = lstring_append_generic_f(result, str+startidx, idx-startidx);
			result = lstring_append_cstr_f(result, replacement);
			startidx = idx + strlen(needle);
		}
	}

	str = lstring_from_cstr_f(str, result);
	lstring_delete(result);

	return str;
}

int lstring_find_substring(lstring *str, const char *needle, int idx) {
	const char *ptr = NULL;

	l_assert(str!=NULL);
	l_assert(needle!=NULL);
	l_assert(idx>=0);

	if (idx>=lstring_len(str)) {
		return -1;
	}

	ptr = strstr(str+idx, needle);
	if (ptr==NULL) {
		return -1;
	} else {
		return ptr-str;
	}
}
