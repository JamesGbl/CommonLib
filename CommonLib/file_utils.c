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

#include "file_utils.h"
#include <stdio.h>

lstring *create_lstring_from_file(const char *fname, lerror **error) {
	FILE *in;
	lstring *data = NULL;
	lerror *myerror = NULL;
	char buffer[4096];
	const int BUFFER_SIZE = sizeof(buffer);
	int t;

	l_assert(error==NULL || *error==NULL);
	l_assert(fname!=NULL);
	
	if(fopen_s(&in, fname, "rb")) {
		lerror_set_sprintf(error, "Can't open file %s", fname);
		goto end;
	}

	data = lstring_new();
	while(!feof(in)) {
		t=fread(buffer, 1, BUFFER_SIZE-1, in);
		buffer[t]='\x0';
		data = lstring_append_cstr_f(data, buffer);
	}

end:
	if (in!=NULL) fclose(in);
	return data;
}
