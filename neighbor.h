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

/*
 * This is struct for neighbor. It is a linked list of neighbors.
 * The member pathListNext is pointer to the neighbor for which the member path is the last update sent.
 * The head of pathListNext is Path->neighborList
 */
typedef struct Neighbor {
    char address[ADDRESS_LEN];      // Address of the neighbor, max len is 16
    struct Path *path;              // The last path for which the updated has been sent by the neighbor
    struct Neighbor *next;          // Pointer to the next neigbhor in the neighbor list
    struct Neighbor *pathListNext;  // Pointer to the neighbor for which the update has been sent for path
} Neighbor;

/*
typedef struct NeighborList {
    Neighbor *neighbor;
    struct NeighborList *next;
} NeighborList;
*/

int addNeighbor(char *address);
void sendUpdates();
void routeRefresh();

#endif

