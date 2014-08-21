#ifndef __LLOGGING_H
#define __LLOGGING_H

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

/**
 * File: llogging.h
 */

/**
 * Enum: LoggingLevel
 * Various logging levels
 */
enum LoggingLevel {
    LEVEL_ERROR,
    LEVEL_CRITICAL,
    LEVEL_WARNING,
    LEVEL_MESSAGE,
    LEVEL_INFO,
    LEVEL_DEBUG
};

/**
 * Function: l_log
 * Logs a message
 * 
 * Parameters:
 *     domain - The logging domain. NULL means the main application
 *     level - The level
 *     format - A format string
 */
void l_log( const char *domain, enum LoggingLevel level, const char *format, ... );

/**
 * Function: l_message
 * Convenience function with NULL logging domain and MESSAGE level
 */
void l_message( const char *format, ... );

/**
 * Function: l_info
 * Convenience function with NULL logging domain and INFO level
 */
void l_info( const char *format, ... );

/**
 * Function: l_debug
 * Convenience function with NULL logging domain and DEBUG level
 */
void l_debug( const char *format, ... );

/**
 * Function: l_error
 * Convenience function with NULL logging domain and ERROR level
 */
void l_error( const char *format, ... );

#endif

