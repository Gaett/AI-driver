#ifndef VECTOR_H
#define VECTOR_H
/**
* ENSICAEN
* 6 Boulevard Maréchal Juin
* F-14050 Caen Cedex
*
* This file is owned by ENSICAEN students.
* No portion of this document may be reproduced, copied
* or revised without written permission of the authors.
*/

/**
* @author Barret Gaetan <gaetan.barret@ecole.ensicaen.fr>
* @version     0.0.1 - 2020-03-18
*
* @todo the list of improvements dor the file.
* @bug the list of known bugs.
*
* code inspiré de https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/
*
*/
#include<driver.h>

#define VECTOR_INIT_CAPACITY 4

typedef _tile data;
typedef struct vector {
    data ** elements;
    int capacite;
    int total;
} vector;

void vector_init(vector *v);
int vector_total(vector *v);
void vector_resize(vector *v, int capacity);
void vector_add(vector *v, data* item);
void vector_set_at(vector *v, int index, data *item);
data *vector_get(vector *v, int index);
void vector_delete(vector *v, int index);
void vector_clear(vector *v);
int invector (vector *v, data* item);
void deleteitem (vector *v, data* item);
TILE getlowestf(vector *v);

#endif
