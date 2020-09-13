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

/**
*@file main.c
*
*Description of the program objectives.
*All necessary references.
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <driver.h>
#include <vector.h>
#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand) {
    int gas = accX * accX + accY * accY;
    gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
    if (inSand) {
        gas += 1;
    }
    return -gas;
}

MAP init_map() {
    char line_buffer[MAX_LINE_LENGTH];
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    int width, height, gas;

    sscanf(line_buffer, "%d %d %d", &width, &height, &gas);

    MAP map = (MAP)malloc(sizeof(struct map));

    map->width = width;
    map->height = height;
    map->gasLevel = gas;

    map->tilemap = (TILE**)malloc(map->height * sizeof(TILE**));
    for (int i = 0; i < map->height; i++) {
        map->tilemap[i] = (TILE*)malloc(map->width * sizeof(TILE*));
        for (int j = 0; j < map->width; j++) {
            map->tilemap[i][j] = (TILE)malloc(sizeof(struct tile));
        }
    }

    int tilecount = 0;
    for (int i = 0; i < map->height; i++) {
        fgets(line_buffer, MAX_LINE_LENGTH, stdin);
        for (int j = 0; j < map->width; j++) {
            map->tilemap[i][j]->value = line_buffer[j];
            map->tilemap[i][j]->position[0] = i;
            map->tilemap[i][j]->position[1] = j;
            map->tilemap[i][j]->parent = map->tilemap[i][j];
            fprintf(stderr, "%c", map->tilemap[i][j]->value);
            if ( (map->tilemap[i][j]->value == '#') ||  (map->tilemap[i][j]->value == '~') ) {
                tilecount++;
            }
        }
        fputs("\n", stderr);
    }
    map->tileCount = tilecount;
    return map;
}

void freeMap(MAP map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            free(map->tilemap[i][j]);
        }
    }
    for (int i = 0; i < map->height; i++) {
        free(map->tilemap[i]);
    }
    free(map->tilemap);
    free(map);
    map = NULL;
}

int isLoop(TILE one, TILE two) {
    if (one->parent == two && two->parent == one) {
        return 1;
    } else {
        return 0;
    }
}

float heuristic(TILE start, TILE goal) {
    if (start && goal) {
        int calcul = ((start->position[0] - goal->position[0])*(start->position[0] - goal->position[0]));
        calcul += ((start->position[1] - goal->position[1])*(start->position[1] - goal->position[1]));
        if (start->value == '~') {
            calcul += 2;
        }
        return calcul;
    }
    return 0;
}

vector construct_path(TILE end, TILE start) {
    vector path;
    vector_init(&path);
    TILE tmp = end;
    do {
        if ((tmp == tmp->parent) || (tmp == start) || isLoop(tmp,tmp->parent)) {
            return path;
        }
        vector_add(&path, tmp);
        tmp->parent->next = tmp;
        tmp = tmp->parent;
    } while (tmp->parent);
    return path;
}

int ist2diagonalfromt1(TILE t1,TILE t2) {
    int x1 = t1->position[0];
    int y1 = t1->position[1];
    int x2 = t2->position[0];
    int y2 = t2->position[1];
    if ( (((x1+1)== x2)||((x1-1)== x2)) &&   (((y1+1)== y2)||((y1-1)== y2))   ) {
        return 1;
    }
    return 0;
}

int sameTiles(TILE t1,TILE t2) {
    if ( (t1->position[0] == t2->position[0]) && (t1->position[1] == t2->position[1]) ) {
        return 1;
    }
    return 0;
}

void updateF (TILE t) {
    t->f = t->g + t->h;
}

vector voisins (TILE t, MAP map, vector * openlist, vector * closedlist) {
    vector neighbors;
    vector_init(&neighbors);
    int i, j;

    for (i = t->position[0]-1; i <= t->position[0]+1; i++) {
        for (j = t->position[1]-1; j <= t->position[1]+1; j++) {
            if ( ((i >= 0)&&(i < map->height)) && ((j >= 0)&&(j < map->width)) && (map->tilemap[i][j]->value != '.')  && (map->tilemap[i][j] != t) ) {
                if ( (t != map->tilemap[i][j]) && (!invector (openlist, map->tilemap[i][j])) && (!invector (closedlist,map->tilemap[i][j] )) && (!map->tilemap[i][j]->locked)   ) {
                    if (t->value == '~') {
                        if (ist2diagonalfromt1(map->tilemap[i][j],t) == 0) {
                            vector_add(&neighbors, map->tilemap[i][j]);
                        }
                    } else {
                        vector_add(&neighbors, map->tilemap[i][j]);
                    }
                }
            }
        }
    }

//On part du principe que tous les couts sont les memes...
    for (i = 0; i < neighbors.total; i++) {
        if (neighbors.elements[i]) {
            if (ist2diagonalfromt1(neighbors.elements[i]->parent,t)) {
                neighbors.elements[i]->g = t->g*2 + 3;
            } else {
                neighbors.elements[i]->g = t->g + 1;
            }
            if (neighbors.elements[i]->value == '~') {
                neighbors.elements[i]->g++;
            }
            updateF(neighbors.elements[i]);
        }
        if (neighbors.elements[i]->parent != t->parent) {
            neighbors.elements[i]->parent = t;
        }
    }
    return neighbors;
}

vector algorithm(TILE start, TILE end, MAP map, int sandallowed) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            map->tilemap[i][j]->h = heuristic(map->tilemap[i][j],end);
            map->tilemap[i][j]->g = 0;
            map->tilemap[i][j]->f = 0;
            if (map->tilemap[i][j]->value == '2' || map->tilemap[i][j]->value == '3' || map->tilemap[i][j]->value == '1') {
                map->tilemap[i][j]->h = 214748364;
            }
            map->tilemap[i][j]->locked = false;
            if ( (map->tilemap[i][j]->value == '~') && sandallowed) {
                map->tilemap[i][j]->locked = true;
            }
        }
    }


    vector open_list, closed_list, path;
    vector_init(&open_list);
    vector_add(&open_list, start);

    vector_init(&closed_list);
    vector_init(&path);

    start->g = 0;
    start->f = start->g + start->f;

    while (vector_total(&open_list) > 0) {

        TILE current = getlowestf(&open_list);

        if (current == end) {

            vector_clear(&path);
            path = construct_path(end,start);
            vector_clear(&open_list);
            vector_clear(&closed_list);
            return path;

        }
        deleteitem(&open_list,current);
        vector_add(&closed_list, current);

        vector voisin = voisins(current,map,&open_list ,&closed_list);
        TILE openneighbor = voisin.elements[0];
        for (int i = 0; i < voisin.total; i++) {

            if (!invector(&closed_list,voisin.elements[i])) {
                updateF(voisin.elements[i]);
                if (voisin.elements[i]->value == '~') {
                    voisin.elements[i]->f++;
                }
                if (!invector(&open_list, voisin.elements[i])) {
                    vector_add(&open_list, voisin.elements[i]);
                } else {
                    openneighbor = voisin.elements[i];
                    if (voisin.elements[i]->g < openneighbor->g) {
                        openneighbor->g = voisin.elements[i]->g;
                        openneighbor->parent = voisin.elements[i]->parent;
                    }
                }

            }

        }
        vector_clear(&voisin);

    }

    vector_clear(&open_list);
    vector_clear(&closed_list);
    return path;
}

TILE getTileFromPosition (int x, int y, MAP map ) {
    if (map->tilemap[x][y]) {
        return map->tilemap[x][y];
    }
    return NULL;
}

vector getAllGoal(MAP map) {
    vector goals;
    vector_init(&goals);
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if ((map->tilemap[i][j]->value == '=') && !(map->tilemap[i][j]->locked)) {
                vector_add(&goals, map->tilemap[i][j]);
            }
        }
    }
    return goals;
}

TILE getCloserGoal (vector* goals, TILE start) {
    TILE goal = vector_get(goals, 0);
    int critera = heuristic(goal, start);
    for (int i = 0; i < goals->total; i++) {
        TILE tmp = vector_get(goals, i);
        if (critera > heuristic(tmp, start)) {
            goal = tmp;
            critera = heuristic(tmp, start);
        }
    }
    return goal;
}

void lissage (TILE myTile, MAP map, int scope, TILE goal, int stop) {
    TILE ministar, ministargoal;
    ministar = myTile;
    while ( ministar != goal ) {
        if ( (heuristic(ministar,goal) > 0) && (heuristic(ministar,goal) < scope*200)) {
            vector goals = getAllGoal(map);
            goal = getCloserGoal(&goals,ministar);
            vector inter = algorithm(ministar,goal,map,true);
            vector_clear(&goals);
            vector_clear(&inter);
        }
        ministargoal = ministar;
        for (int i = 0; i < scope; i++) {
            if (ministargoal) {
                if (ministargoal == goal) {
                    break;
                }
                ministargoal = ministargoal->next;
            } else {
                break;
            }
        }
        vector inter = algorithm(ministar,ministargoal,map,true);
        vector_clear(&inter);
        ministar = ministargoal;
        if (stop) {
            break;
        }
    }
}

void pathInit(TILE myTile,MAP map, char marque) {
    vector goals = getAllGoal(map);
    vector truepath;
    TILE goal = getCloserGoal(&goals,myTile);
    truepath = algorithm(myTile,goal,map,true);
    vector_clear(&truepath);
    vector_clear(&goals);
    if (map->tileCount > 1000) {
        lissage (myTile, map, 10, goal, true);
        lissage (myTile, map, 5, goal,false);
    } else if (map->tileCount > 1500) {
        for (int i = 0; i < 100; i++) {
            lissage (myTile, map, 3+i, goal,false);
        }
    } else {
        if (map->tileCount < 500 ) {
            for (int i = 3; i < 20; i++) {
                lissage (myTile, map, i, goal, false);
            }
        } else {
            lissage (myTile, map, 10, goal, true);
        }
    }
    TILE it = myTile;
    do {
        it->value = marque;
        it = it->next;
    } while (it);
}

int main() {
	char action[100];
    int accelerationX = 0, accelerationY = 0;
    int speedX = 0, speedY = 0;


    MAP map = init_map();

    int hasStarted = false;

    TILE myTile, toGo, verif;
    int blocageMulti = 0;
    int blocage = 0;
    vector blocages;
    vector_init(&blocages);
    int notYetBlocked = 1;
    while (!feof(stdin)) {
        int myX, myY, secondX, secondY, thirdX, thirdY;
        char line_buffer[MAX_LINE_LENGTH];
        fgets(line_buffer, MAX_LINE_LENGTH, stdin);
        sscanf(line_buffer, "%d %d %d %d %d %d", &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
        myTile = getTileFromPosition (myY,myX, map);
        if (!hasStarted) {
            verif = myTile;
            hasStarted = true;
            pathInit(myTile, map,'9');
        }
        verif = toGo;
        toGo = myTile->next;
        int movementY,movementX;
        //Cela signifie que l'on n'a pas avance au tour precedent
        if (verif == toGo) {
            if (notYetBlocked) {
                notYetBlocked = 0;
                TILE it = myTile;
                do {
                    if ( (it->next == NULL) && (it->value == '=')) {
                        it->locked = 1;
                    }
                    it = it->next;
                } while (it);
                speedY = 0;
                speedX = 0;
                TILE recul = myTile->parent;
                for (int j = 0; j < blocageMulti; j++) {
                    recul = recul->parent;
                }
                pathInit(recul, map,'a');
                if (recul != myTile->parent) {
                    vector v = algorithm(myTile,recul,map,true);
                    vector_clear(&v);
                    toGo = myTile->next;
                } else {
                    toGo = recul->next;
                }
                blocageMulti++;
            } else {
                notYetBlocked = 1;
                TILE it = myTile;
                do {
                    if ( (it->next == NULL) && (it->value == '=')) {
                        it->locked = 1;
                    }
                    it = it->next;
                } while (it);
                speedY = 0;
                speedX = 0;
                pathInit(myTile, map,'a');
                if (blocage > 0) {
                    TILE recul = myTile->parent;
                    for (int j = 0; j < blocage; j++) {
                        recul = recul->parent;
                    }
                    pathInit(recul, map,'a');
                    if (recul != myTile->parent) {
                        vector v = algorithm(myTile,recul,map,true);
                        vector_clear(&v);
                        toGo = myTile->next;
                    } else {
                        toGo = recul->next;
                    }
                } else {
                    for (int i = 0; i < blocages.total; i++) {
                        if (sameTiles(vector_get(&blocages,i),myTile)) {
                            blocage++;
                            break;
                        }
                    }
                }
                vector_add(&blocages,myTile);
            }

        }
        movementY = (toGo->position[0] - myTile->position[0]);
        movementX = (toGo->position[1] - myTile->position[1]);
        accelerationY = movementY - speedY;
        accelerationX = movementX - speedX;
        speedY = movementY;
        speedX = movementX;

        sprintf(action, "%d %d", accelerationX, accelerationY);
        fprintf(stdout, "%s", action);
        fflush(stdout);
    }

    vector_clear(&blocages);
    free(map);
    return 0;
}

/**
do {
   it->value = '9';
   fprintf(stderr, "GOGOGO : %d %d \n", it->position[0], it->position[1]);
   it = it->next;
} while (it);
for (int i = 0; i < map->height; i++) {
   for (int j = 0; j < map->width; j++) {
       fprintf(stderr, "%c", map->tilemap[i][j]->value);
   }
   fputs("\n", stderr);
}
*/
