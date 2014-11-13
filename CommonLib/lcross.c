#include "lcross.h"
#include "llogging.h"
#include "assert.h"
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

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

#ifdef __unix__
#include <signal.h>

void l_break_to_debugger(void)
{
	raise(SIGINT);
}
#endif

