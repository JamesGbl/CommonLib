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

#include "db_utils.h"
#include "separatore_query.h"
#include "lcross.h"
#include <stddef.h>
#include <stdarg.h>

static lstring* db_append_sql_format_vf( lstring *str, const char *format, va_list args );
static lbool db_is_keyword( const char *str );

lstring* db_append_sql_escaped_f( lstring *str, const char *value ) 
{
    const char *c = NULL;

    l_assert( str!=NULL );

    if ( value==NULL ) {
        return lstring_append_cstr_f(str, "NULL");
    }

    str = lstring_append_cstr_f(str, "\'" );
    
    c = value;
    while (*c) {
        switch(*c) {
        case '\'':
            str = lstring_append_cstr_f(str, "\'\'" );
            break;
        default:
            str = lstring_append_char_f(str, *c);
            break;
        }
        
        c++;
    }
    
    str = lstring_append_cstr_f(str, "\'" );
	return str;
}

int db_compare_datetime( const char *first, const char *second ) 
{
    int result = 0;

    l_assert( first!=NULL );
    l_assert( second!=NULL );

    while ( 1 ) {
        if ( *first==0 ) { result=0; break; }
        else if ( *second==0 ) { result = 0; break; }
        else if ( (*first)<(*second) ) { result=-1; break; }
        else if ( (*second)<(*first) ) { result=1; break; }

        first++;
        second++;
    }

    return result;
}

lstring* db_put_sql_format_f( lstring *str, const char *format, ... ) {
    va_list args;

    l_assert( str!=NULL );
    l_assert( format!=NULL );

    lstring_truncate( str, 0 );

    va_start( args, format );
    str = db_append_sql_format_vf( str, format, args );
    va_end( args );

	return str;
}

lstring* db_append_sql_format_f( lstring *str, const char *format, ... ) {
    va_list args;

    l_assert( str!=NULL );
    l_assert( format!=NULL );

    va_start( args, format );
    str = db_append_sql_format_vf( str, format, args );
    va_end( args );

	return str;
}

void db_execute_sql_script(DbConnection *conndb, const char *sql_script, lerror **error) {
	const char *prossima = NULL;
	lstring *sql = NULL;
	lerror *myError = NULL;

	l_assert(sql_script!=NULL);
	l_assert(error==NULL || *error==NULL);

	sql = lstring_new();

	while(sql_script!=NULL) {
		prossima = DB_prossima_query(sql_script);
		lstring_reset(sql);

		sql = lstring_append_generic_f(sql, sql_script, prossima-sql_script);
		if ( (*prossima) == '\x0' ) {
			sql_script = NULL;
		} else {
			sql_script = prossima + 1;
		}

		DbConnection_sql_exec(conndb, sql, &myError);
		if (lerror_propagate(error, myError)) break;

	}

	lstring_delete(sql);
}


static lbool db_is_keyword( const char *str ) {
    l_assert( str!=NULL );

    if ( 0==l_stricmp( str, "level" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "from" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "select" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "update" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "where" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "is" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "order" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "by" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "as" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "inner" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "outer" ) ) return LTRUE;
    if ( 0==l_stricmp( str, "join" ) ) return LTRUE;

    return LFALSE;
}

static lstring* db_append_sql_format_vf( lstring *str, const char *format, va_list args ) {
    const char *tmp = NULL;
    lbool is_keyword = LFALSE;

    l_assert( str!=NULL );
    l_assert( format!=NULL );

    while( (*format)!='\0' ) {
        if ( (*format)!='%' ) {
            str = lstring_append_char_f( str, *format );
            format++;
        } else if ( (*(format+1))=='s' ) {
            str = db_append_sql_escaped_f( str, va_arg(args, const char *) );
            format += 2;
        } else if ( (*(format+1))=='i' ) {
            str = lstring_append_sprintf_f( str, "%i", va_arg(args, int) );
            format += 2;
        } else if ( (*(format+1))=='k' ) {
            str = lstring_append_cstr_f( str, va_arg(args, const char *) );
            format += 2;
        } else if ( (*(format+1))=='m' ) {

            tmp = va_arg(args, const char *);
            is_keyword = db_is_keyword( tmp );
            if ( is_keyword ) str = lstring_append_char_f( str, '\"');
            str = lstring_append_cstr_f( str, tmp );
            if ( is_keyword ) str = lstring_append_char_f( str, '\"');
            format += 2;
        }
    }

	return str;
}