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
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

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

void write_lstring_to_file(const char *fname, lstring *buffer, lerror **error) {
	FILE *out;

	l_assert(fname!=NULL);
	l_assert(buffer!=NULL);
	l_assert(error==NULL || *error==NULL);

	if(fopen_s(&out, fname, "wb")) {
		lerror_set_sprintf(error, "IO error writing to %s", fname);
		return;
	}

	fwrite(buffer, 1, lstring_len(buffer), out);
	fclose(out);
}

#ifdef WIN32
lbool file_exists(const char *fname) {
	DWORD dwAttrib = GetFileAttributes(fname);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
#else
lbool file_exists(const char *fname) {
	l_assert(fname!=NULL);

	if (access(fname, F_OK) != -1) {
		return LTRUE;
	} else {
		return LFALSE;
	}
}
#endif

lstring* lcreatetempname_f(lstring *buffer) {
#ifdef _WIN32
	char tempspace[L_tmpnam_s];
	l_assert(buffer!=NULL);
	tmpnam_s(tempspace, L_tmpnam_s);
	return lstring_from_cstr_f(buffer, tempspace);
#else
	return lstring_from_cstr_f(buffer, tmpnam(NULL));
#endif
}