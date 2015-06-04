#ifndef __JSON_UTILS
#define __JSON_UTILS

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

#include "unicodeutils.h"
#include "lcross.h"
#include "lstring.h"
#include "third-party/ujdecode.h"

typedef struct JsonBuffer JsonBuffer;

/**
 * Function: JsonBuffer_new
 *
 * Create a new buffer where you can write JSON data
 */
JsonBuffer *JsonBuffer_new();

/**
 * Function: JsonBuffer_setIndent
 *
 * If the indent-enabled flag is active the json will
 * be indented while it is written.
 *
 * Parameters:
 *   self - The JsonBuffer object (must not be NULL)
 *   indentEnabled - True se il JSON viene indentato
 *     mentre viene scritto
 */
void JsonBuffer_setIndent(JsonBuffer *self, lbool indentEnabled);

/**
 * Function: JsonBuffer_getIndent
 *
 * Get the status of the indent-enabled flag
 *
 * Parameters:
 *   self - The JsonBuffer object (must not be NULL)
 *
 * Return: the status of the indent-flag
 */
lbool JsonBuffer_getIndent(JsonBuffer *self);

/**
 * Function: JsonBuffer_destroy
 *
 * Destroy the JSON buffer and all its contents
 *
 * Parameters:
 *   self - The buffer to destroy. If this is NULL the
 *     function will do nothing
 */
void JsonBuffer_destroy( JsonBuffer *self );

/**
 * Function: JsonBuffer_startList
 *
 * Write the list start marker to the JSON buffer. The list
 * has to be ended with the end-list marker.
 *
 * Parameters:
 *   self - The buffer where it will write to; it has to be
 *     not null
 */
void JsonBuffer_startList( JsonBuffer *self );

/**
 * Function: JsonBuffer_endList
 *
 * Write the end list marker to the JSON buffer.
 *
 * Parameters:
 *   self - The buffer where it will write to; it has to be
 *     not null
 */
void JsonBuffer_endList( JsonBuffer *self );

/**
 * Function: JsonBuffer_startObject
 *
 * Write the object start marker to the JSON buffer.
 *
 * Parameters:
 *   self - The buffer where it will write to; it has to be
 *     not null
 */
void JsonBuffer_startObject( JsonBuffer *self );

/**
 * Function: JsonBuffer_endObject
 *
 * Write the object end marker to the JSON buffer.
 *
 * Parameters:
 *   self - The buffer where it will write to; it has to be
 *     not null
 */
void JsonBuffer_endObject( JsonBuffer *self );

/**
 * Function: JsonBuffer_writePropertyName
 *
 * Write the property-start-marker to the buffer. The next JSON
 * object will be the value of this property and has to be followed
 * by the separator.
 *
 * Parameters:
 *   self - The buffer where it will write to; it has to be
 *     not null
 *   name - The property name, it must be not null
 */
void JsonBuffer_writePropertyName( JsonBuffer *self, const char *name );

/**
 * Function: JsonBuffer_writeStringAttribute 
 *
 * Write a property-value pair where the value is of type string
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be
 *     not null
 *   name - The name of the property; it has to be not null
 *   str - The value of the property
 *   writeSeparator - If true I will write a separator after the
 *     value
 */
void JsonBuffer_writeStringAttribute( JsonBuffer *self, const char *name, const char *str, lbool writeSeparator );

/**
 * Function: JsonBuffer_writeBoolAttribute
 *
 * Write a property-value pair where the value is of type boolean
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be
 *     not null
 *   name - The name of the property; it has to be not null
 *   value - The value of the property
 *   writeSeparator - If true I will write a separator after the
 *     value
 */
void JsonBuffer_writeBoolAttribute(JsonBuffer *self, const char *name, lbool value, lbool writeSeparator);

/**
 * Function: JsonBuffer_writeIntAttribute
 *
 * Write a property-value pair where the value is of type int
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be
 *     not null
 *   name - The name of the property; it has to be not null
 *   value - The value of the property
 *   writeSeparator - If true I will write a separator after the
 *     value
 */
void JsonBuffer_writeIntAttribute(JsonBuffer *self, const char *name, int value, lbool writeSeparator);

/**
 * Function: JsonBuffer_writeString
 *
 * Write a string to this JSON stream. The string can be NULL
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be
 *     not null
 *   str - The string that I will write; it can be NULL
 */
void JsonBuffer_writeString( JsonBuffer *self, const char *str );

/**
 * Function: JsonBuffer_writeInt
 *
 * Write an integer to this JSON stream.
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be
 *     not null
 *   val - The integer to write
 */
void JsonBuffer_writeInt(JsonBuffer *self, int val);

/**
 * Function: JsonBuffer_writeNull
 *
 * Write a null to this JSON stream.
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be not null
 */
void JsonBuffer_writeNull( JsonBuffer *self );

/**
 * Function: JsonBuffer_writeFalse
 *
 * Write false to this JSON stream.
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be not null
 */
void JsonBuffer_writeFalse( JsonBuffer *self );

/**
 * Function: JsonBuffer_writeTrue
 *
 * Write true to this JSON stream.
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be not null
 */
void JsonBuffer_writeTrue( JsonBuffer *self );

/**
 * Function: JsonBuffer_writeTrue
 *
 * Write this boolean to this JSON stream.
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be not null
 */
void JsonBuffer_writeBool(JsonBuffer *self, lbool value);

/**
 * Function: JsonBuffer_writeSeparator
 * 
 * Write the separator between couples (for objects) and objects (for arrays)
 *
 * Parameters:
 *   self - The buffer where I must write to; it has to be not null
 */
void JsonBuffer_writeSeparator( JsonBuffer *self );

/**
 * Function: JsonBuffer_size
 *
 * Get the size of this stream
 *
 * Parameters:
 *   self - The buffer
 */
int JsonBuffer_size( JsonBuffer *self );

/**
 * Function: JsonBuffer_size
 *
 * Get the memory address of the stream data
 *
 * Parameters:
 *   self - The buffer
 */
const char *JsonBuffer_get( JsonBuffer *self );

/**
 * Function: lstring_new_from_ujobject
 * Create an lstring from an UJObject
 * Parameters:
 *   object - The UJObject to decode
 */
lstring *lstring_new_from_ujobject(UJObject object);

/**
 * Function: lstring_from_ujobject_f
 * Fill an existing lstring from an ujobject
 * Parameters:
 *   result - The existing lstring to fill
 *   object - The UJObject to decode
 */
lstring *lstring_from_ujobject_f(lstring *result, UJObject object);

/**
 * Function: lstring_from_ujstring_f
 * Fill an existeng lstring from an ujobject
 * Parameters:
 *   result - The existing lstring to fill
 *   str - The UJString to decode
 */
lstring *lstring_from_ujstring_f(lstring *result, UJString str);

/**
 * Function: Gets the element count of an UJArray
 * Parameters:
 *   object - The UJObject containing the array, must be not NULL
 * Returns: The element count
 */
int get_ujarray_element_count(UJObject object);

#endif
