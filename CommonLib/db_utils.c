#include "db_utils.h"
#include "lcross.h"
#include <stddef.h>
#include <stdarg.h>

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
