#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "llogging.h"
#include <stdio.h>
#include <stdarg.h>

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

static const char *level_to_string( enum LoggingLevel level ) {
    switch( level ) {
    case LEVEL_ERROR:
	return "ERROR";
    case LEVEL_CRITICAL:
	return "CRITICAL";
    case LEVEL_WARNING:
	return "WARNING";
    case LEVEL_MESSAGE:
	return "MESSAGE";
    case LEVEL_INFO:
	return "INFO";
    case LEVEL_DEBUG:
	return "DEBUG";
    default:
	return "UNKNOWN";
    }
}

static void l_internal_log( const char *domain, enum LoggingLevel level, const char *format, va_list args ) {
    char logMessage[2048];

    vsnprintf( logMessage, 2047, format, args );

    fputs( level_to_string( level ), stderr );
    fputs( " - ", stderr );
    fputs( logMessage, stderr );
    fputs( "\n", stderr );
}

void l_log( const char *domain, enum LoggingLevel level, const char *format, ... ) {
    va_list args;

    va_start( args, format );
    l_internal_log( domain, level, format, args );
    va_end( args );
}

void l_message( const char *format, ... ) {
    va_list args;

    va_start( args, format );
    l_internal_log( NULL, LEVEL_MESSAGE, format, args );
    va_end( args );
}

void l_info( const char *format, ... ){
    va_list args;

    va_start( args, format );
    l_internal_log( NULL, LEVEL_INFO, format, args );
    va_end( args );
}

void l_debug( const char *format, ... ) {
    va_list args;

    va_start( args, format );
    l_internal_log( NULL, LEVEL_DEBUG, format, args );
    va_end( args );
}

void l_error( const char *format, ... ) {
    va_list args;

    va_start( args, format );
    l_internal_log( NULL, LEVEL_ERROR, format, args );
    va_end( args );
}
