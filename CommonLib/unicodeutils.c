#include <stdio.h>
#include "unicodeutils.h"
#include "lcross.h"
#include "lmemory.h"

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

#ifdef _WIN32

#include <windows.h>
/**
 * Traduce una stringa di caratteri CP1252 in una stringa di caratteri
 * UTF-16 Windows. Il risultato deve essere deallocato dal chiamante.
 */
WCHAR* string1252ToWChar(const char *orig) {
	int requiredBufferSize;
	WCHAR *spazioConversione;

	if(orig==NULL) {
		return NULL;
	}

	requiredBufferSize=MultiByteToWideChar(1252, 0, orig, strlen(orig)+1, NULL, 0);
	spazioConversione=(WCHAR *)lmalloc(sizeof(WCHAR)*requiredBufferSize);

	if(spazioConversione!=NULL) {
		/* Conversione dal codepage 1252 a multibyte */
		MultiByteToWideChar(1252, 0, orig, strlen(orig)+1, spazioConversione, requiredBufferSize);
		return spazioConversione;
	} else {
		return NULL;
	}
}

/**
 * Traduce una stringa di caratteri UTF-16 Windows in una stringa UTF-8.
 * Il risultato deve essere deallocato dal chiamante
 */
char* stringWCharToUtf8(const WCHAR* orig) {
	int requiredBufferSize;
	char *spazioConversione;

	if(orig==NULL) {
		return NULL;
	}

	requiredBufferSize=WideCharToMultiByte(CP_UTF8, 0, orig, -1, NULL, 0, NULL, NULL);
	spazioConversione=(char *)lmalloc(requiredBufferSize);
	
	if(spazioConversione!=NULL) {
		WideCharToMultiByte(CP_UTF8, 0, orig, -1, spazioConversione, requiredBufferSize, NULL, NULL);
		return spazioConversione;
	} else {
		return NULL;
	}
}

/**
 * Traduce una stringa di caratteri UTF8 in una stringa di caratteri
 * UTF-16 Windows. Il risultato deve essere deallocato dal chiamante.
 */
WCHAR* stringUtf8ToWChar(const char *orig) {
	int requiredBufferSize;
	WCHAR *spazioConversione;

	if(orig==NULL) {
		return NULL;
	}

	requiredBufferSize=MultiByteToWideChar(CP_UTF8, 0, orig, strlen(orig)+1, NULL, 0);
	spazioConversione=(WCHAR *)lmalloc(sizeof(WCHAR)*requiredBufferSize);

	if(spazioConversione==NULL) {
		return NULL;
	}

	MultiByteToWideChar(CP_UTF8, 0, orig, strlen(orig)+1, spazioConversione, requiredBufferSize);
	return spazioConversione;
}

/**
 * Traduce una stringa di caratteri UTF-16 Windows in una stringa CP1252.
 * Il risultato deve essere deallocato dal chiamante
 */
char* stringWCharTo1252(const WCHAR* orig) {
	int requiredBufferSize;
	char *spazioConversione;

	if(orig==NULL) {
		return NULL;
	}
	
	requiredBufferSize=WideCharToMultiByte(1252, 0, orig, -1, NULL, 0, NULL, NULL);
	spazioConversione=(char *)lmalloc(requiredBufferSize);

	if(spazioConversione==NULL) {
		return NULL;
	}
	
	WideCharToMultiByte(CP_ACP, 0, orig, -1, spazioConversione, requiredBufferSize, NULL, NULL);
	return spazioConversione;
}

/**
 * Traduce una stringa di caratteri CP1252 in una stringa di caratteri
 * a lunghezza variabile UTF8. Il risultato deve essere deallocato
 * dal chiamante
 */
char *string1252ToUtf8(const char *orig) {
	WCHAR *multiByte;
	char *utf8String;

	if(orig==NULL) {
		return NULL;
	}

	multiByte=string1252ToWChar(orig);
	if(multiByte==NULL) {
		return NULL;
	}

	utf8String=stringWCharToUtf8(multiByte);
	lfree(multiByte);
	if(utf8String==NULL) {
		return NULL;
	}

	return(utf8String);
}

/**
 * Traduce una stringa di caratteri UTF8 in una stringa di caratteri
 * 1252. Il risultato deve essere deallocato
 * dal chiamante
 */
char *stringUtf8To1252(const char *orig) {
	WCHAR *wString;
	char *cpString;

	if(orig==NULL) {
		return NULL;
	}

	wString=stringUtf8ToWChar(orig);

	if(wString==NULL) {
		return NULL;
	}

	cpString=stringWCharTo1252(wString);
	lfree(wString);

	if(cpString==NULL) {
		return NULL;
	}
	return cpString;
}

#else

#include <iconv.h>
#include <stdlib.h>
#include <string.h>

char *stringUtf8To1252(const char *orig) {
    iconv_t h;
    size_t len;
    size_t destLen;
    char *outputBuf = NULL;
    char *outputBufStart = NULL;

    if ( orig==NULL ) return NULL;

    h = iconv_open( "CP1252", "UTF-8" );
    l_assert ( h!=(iconv_t)(-1) );

    len = strlen( orig );
    destLen = len*2;
    outputBuf = (char *)calloc( destLen + 2, 1 );
    outputBufStart = outputBuf;
    iconv( h, (char **)&orig, &len, &outputBuf, &destLen );

    iconv_close( h );

    return outputBufStart;
}

char *string1252ToUtf8(const char *orig) {
    iconv_t h;
    size_t len;
    size_t destLen;
    char *outputBuf = NULL;
    char *outputBufStart = NULL;

    if ( orig==NULL ) return NULL;

    h = iconv_open( "UTF-8", "CP1252" );
    l_assert ( h!=(iconv_t)(-1) );

    len = strlen( orig );
    destLen = len*2;
    outputBuf = (char *)calloc( destLen + 2, 1 );
    outputBufStart = outputBuf;
    iconv( h, (char **)&orig, &len, &outputBuf, &destLen );

    iconv_close( h );

    return outputBufStart;
}

wchar_t *string1252ToWChar(const char *orig) {
    iconv_t h;
    size_t len;
    size_t destLen;
    wchar_t *outputBuf = NULL;
    wchar_t *outputBufStart = NULL;

    if ( orig==NULL ) return NULL;

    h = iconv_open( "WCHAR_T", "CP1252" );
    l_assert ( h!=(iconv_t)(-1) );

    len = strlen( orig );
    destLen = len*2*sizeof(wchar_t);
    outputBuf = (wchar_t *)calloc( destLen + 2, 1 );
    outputBufStart = outputBuf;
    iconv( h, (char **)&orig, &len, (char **)&outputBuf, &destLen );

    iconv_close( h );

    return outputBufStart;
}

char *stringWCharTo1252(const wchar_t *orig) {
    iconv_t h;
    size_t len;
    size_t destLen;
    char *outputBuf = NULL;
    char *outputBufStart = NULL;

    if ( orig==NULL ) return NULL;

    h = iconv_open( "CP1252", "WCHAR_T" );
    l_assert ( h!=(iconv_t)(-1) );

    len = (wcslen( orig )) *sizeof( wchar_t );
    destLen = len*2;
    outputBuf = (char *)calloc( destLen + 2, 1 );
    outputBufStart = outputBuf;
    iconv( h, (char **)&orig, &len, &outputBuf, &destLen );
    outputBuf[0] = '\0';

    iconv_close( h );

    return outputBufStart;
}

#endif
