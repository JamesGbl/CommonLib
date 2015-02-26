#include "lcross.h"
#include "llogging.h"
#include "assert.h"
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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
	strcat_s(d, strlen(s)+1, s);
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
