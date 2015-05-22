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

#include "lcross.h"
#include "llogging.h"
#include "assert.h"
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#endif

int l_stricmp( const char *s1, const char *s2 ) {
    char c1, c2;

    l_assert( s1!=NULL );
    l_assert( s2!=NULL );

    while (1) {
        c1 = tolower( *s1 );
        c2 = tolower( *s2 );

        if ( c1<c2 ) return -1;
        if ( c1>c2 ) return 1;
        if ( c1==0 || c2==0 ) return 0;

        s1++;
        s2++;
    }
}

int l_strnicmp(const char *s1, const char *s2, int size) {
    char c1, c2;
    int i = 0;

    l_assert( s1!=NULL );
    l_assert( s2!=NULL );

    while (1) {
        if (i==size) return 0;
        c1 = tolower( *s1 );
        c2 = tolower( *s2 );

        if ( c1<c2 ) return -1;
        if ( c1>c2 ) return 1;
        if ( c1==0 || c2==0 ) return 0;

        s1++;
        s2++;
    }
}

void l_assert_internal( lbool condition, const char *function, const char *fileName, int lineNo )
{
    if ( !condition ) {
        l_error( "Assertion failed. Function %s at %s:%i", function, fileName, lineNo );
        abort();
    }
}

uint16_t l_log2(uint32_t n) {
     uint16_t logValue = -1;

     if (n==0) return 0;
     while (n) {//
         logValue++;
         n >>= 1;
     }
     return logValue;
 }

char *l_strdup(const char *d) {
#ifdef _WIN32
	return _strdup(d);
#else
	return strdup(d);
#endif
}

void l_strcpy(char *d, const char *s) {
#ifdef _WIN32
	strcpy_s(d, strlen(s)+1, s);
#else
	strcpy(d, s);
#endif
}

void l_strcat(char *d, const char *s) {
#ifdef _WIN32
	strcat_s(d, strlen(d)+strlen(s)+1, s);
#else
	strcat(d, s);
#endif
}

void l_vsnprintf(char *d, int len, const char *format, va_list args) {
#ifdef _WIN32
	vsnprintf_s(d, len, _TRUNCATE, format, args);
#else
	vsnprintf(d, len, format, args);
#endif
}

int l_atoi(const char *s) {
	if (s==NULL) {
		return 0;
	} else {
		return atoi(s);
	}
}

#ifdef _WIN32
long l_current_time_millis(void) {
	SYSTEMTIME time;
	GetSystemTime(&time);
	return (time.wSecond * 1000) + time.wMilliseconds;	
}
#else
long l_current_time_millis(void) {

	struct timeval  tv;
	gettimeofday(&tv, NULL);

	double time_in_mill = 
		 (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
	return (long)time_in_mill;
}
#endif

int lfopen_s(FILE **pFile, const char *fileName, const char *mode) {
#ifdef _WIN32
	return fopen_s(pFile, fileName, mode);
#else
	FILE *f;
	if (fileName==NULL || mode==NULL || pFile==NULL) return 1;
	f = fopen(fileName, mode);
	if (f==NULL) {
		return 1;
	}
	*pFile = f;
	return 0;
#endif
}

int lunlink(const char *name) {
	if (name==NULL) return 1;
#ifdef _WIN32
	return _unlink(name);
#else
	return unlink(name);
#endif
}

int lfileno(FILE *f) {
	if (f==NULL) return -1;
#ifdef _WIN32
	return _fileno(f);
#else
	return fileno(f);
#endif
}

void l_strlwr(char *s) {
#ifdef _WIN32
	if (s==NULL) return;
	_strlwr_s(s, strlen(s)+1);
#else
        for ( ; *s; ++s) *s = tolower(*s);
#endif
}

void l_strrev(char *s) {
#ifdef _WIN32
	_strrev(s);
#else
	strrev(s);
#endif
}

void l_strncpy(char *dest, const char *src, int count) {
#ifdef _WIN32
	strncpy_s(dest, count+1, src, count);
#else
	strncpy(dest, src, count);
#endif
}

void lsleep(int secs) {
#ifdef _WIN32
	Sleep(secs*1000);
#else
	sleep(secs);
#endif
} 
	
