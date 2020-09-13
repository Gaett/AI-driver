#include <stdio.h>
#include <stdlib.h>
#include <vector.h>

void vector_init(vector *v) {
    v->capacite = 1;
    v->total = 0;
    v->elements = malloc(sizeof(data *) * v->capacite);
}

int vector_total(vector *v) {
    return v->total;
}

void vector_resize(vector *v, int capacity) {
    data **elements = realloc(v->elements, sizeof(data *) * capacity);
    if (elements) {
        v->elements = elements;
        v->capacite = capacity;
    }
}

void vector_add(vector *v, data* item) {
    if (v->capacite == v->total) {
        vector_resize(v, v->capacite * 2);
    }
    v->elements[v->total++] = item;
}

void vector_set_at(vector *v, int index, data *item) {
    if (index >= 0 && index < v->total) {
        v->elements[index] = item;
    }
}

data *vector_get(vector *v, int index) {
    if (index >= 0 && index < v->total) {
        return v->elements[index];
    }
    return NULL;
}

void vector_delete(vector *v, int index)
{
    if (index < 0 || index >= v->total) {
        return;
    }
    v->elements[index] = NULL;
    for (int i = index; i < v->total - 1; i++) {
        v->elements[i] = v->elements[i + 1];
        v->elements[i + 1] = NULL;
    }
    v->total--;
    // Cela permet d'économiser de la place mémoire
    if (v->total > 0 && v->total == v->capacite / 4) {
        vector_resize(v, v->capacite / 2);
    }
}

void vector_clear(vector *v) {
    free(v->elements);
}

void deleteitem (vector *v, data* item) {
    for (int i = 0; i < v->total; i++) {
        if (v->elements[i] == item) {
            vector_delete(v, i);
            return;
        }
    }
}

int invector (vector *v, data* item) {
    for (int i = 0; i < v->total; i++) {
        if (v->elements[i] == item) {
            return i;
        }
    }
    return 0;
}

TILE getlowestf(vector *v){
    TILE lowest = v->elements[0];
    for (int i = 0; i < v->total; i++) {
        if (lowest->f > v->elements[i]->f ) {
            lowest = v->elements[i];
        }
    }
    return lowest;
}

int getFromAdress(vector *v, data* item) {
    int i;
    if (v->total == 0) {
        return 0;
    }
    for (i = 0;  i < v->total; i++) {
        if (v->elements[i] == item) {
            return i;
        }
    }
    return -1;
}
