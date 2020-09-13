#ifndef DRIVER_H
#define DRIVER_H
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
* @version     0.0.1 - 2020-04-20
*
* @todo the list of improvements dor the file.
* @bug the list of known bugs.
*
*/

#define false 0
#define true 1

typedef struct tile {
    char value;
    int position[2]; // x , y
    float f;
    float g;
    float h;
    int locked;
    struct tile* next;
    struct tile* parent;
} *TILE, _tile;

typedef struct map {
    TILE** tilemap;
    int width;
    int height;
    int gasLevel;
    int tileCount;
} *MAP;

int sameTiles(TILE t1,TILE t2);

#endif
