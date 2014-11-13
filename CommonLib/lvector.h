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
#ifndef __LVECTOR_C_H
#define __LVECTOR_C_H

struct lvector {
  void **buffer;
  int size;
};

typedef struct lvector lvector;

/*
** Inizializza un nuovo vettore con la dimensione predefinita passata
*/
lvector* lvector_new(int size);

/*
** Rende la lunghezza di un vettore
*/
int lvector_len(lvector* self);

/*
** Prende un elemento in un vettore
*/
void *lvector_at(lvector* self, int idx);

/*
** Aumenta le dimensioni del vettore
*/
void lvector_resize(lvector* self, int newSize);

/*
** Modifica un elemento all'interno di un vettore
*/
void lvector_set(lvector* self, int idx, void *element);

/*
** Dealloca le risorse corrispondenti a questo vettore
*/
void lvector_delete(lvector* self);

/*
** L'indirizzo al quale e' allocato il vettore
*/
void *lvector_address(lvector *self);

#endif