/*
 * =====================================================================================
 *
 *       Filename:  neighbor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2019 01:38:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __NEIGHBOR_H__

#define __NEIGHBOR_H__

#include "path.h"

#define ADDRESS_LEN 16

typedef struct Neighbor {
    char address[ADDRESS_LEN];
    struct Path *path;
    struct Neighbor *next;
    struct Neighbor *pathListNext;
} Neighbor;

typedef struct NeighborList {
    Neighbor *neighbor;
    struct NeighborList *next;
} NeighborList;

int addNeighbor(char *address);
void sendUpdates();
void routeRefresh();

#endif

