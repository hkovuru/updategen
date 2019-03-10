/*
 * =====================================================================================
 *
 *       Filename:  neighbor.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2019 02:22:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */

#include "neighbor.h"
#include "path.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Neighbor *neighborListG = NULL;

int addNeighbor(char *address) {
    int rv = 0;
    Neighbor *neighbor = (Neighbor *)malloc(sizeof(Neighbor));
    if (neighbor == NULL) {
        printf("Failed to allocate memory for neighbor %s\n", address);
        return -1;
    }

    strncpy(neighbor->address, address, ADDRESS_LEN);
    neighbor->next = NULL;
    neighbor->pathListNext = NULL;
    
    neighbor->next = neighborListG;
    neighborListG = neighbor;

    return 0;
}

void sendUpdatesToNeighbor(Neighbor *neighbor) {
    Path *path;
    uint8_t count;

    if (neighbor != NULL) {
        count = 0;
        path = NULL;
        
        if (neighbor->path == NULL) {
            path = getPathList();
        } else {
            path = neighbor->path;
            path = path->next;
        }

        count++;
        printf("Generated update with path %s attribute-id %d for neighbor %s\n", path->attr, path->attrId, neighbor->address);

        while (path->next != NULL) {
            path = path->next;
            count++;
            printf("Generated update with path %s attribute-id %d for neighbor %s\n", path->attr, path->attrId, neighbor->address);
            if (count == 10) {
                break;
            }
        }

        if (neighbor->path != NULL) {
            removeNeighborFromPath(neighbor->path, neighbor);
        }
        neighbor->path = path;
        addNeighborToPath(path, neighbor);
    }
}

void sendUpdates() {
    Neighbor *neighbor;
    Path *pathList = getPathList();

    if (pathList == NULL) {
        printf("updates list is empty\n");
        return;
    }

    for(neighbor = neighborListG;neighbor != NULL; neighbor = neighbor->next) {
        sendUpdatesToNeighbor(neighbor);
    }
}

void routeRefresh(char *address) {
    Neighbor *neighbor;
    Path *pathList = getPathList();

    if (pathList == NULL) {
        //printf("updates list is empty\n");
        return;
    }

    for(neighbor = neighborListG;neighbor != NULL; neighbor = neighbor->next) {
        if (!strncmp(neighbor->address, address, ADDRESS_LEN)) {
            if (neighbor->path != NULL) {
                removeNeighborFromPath(neighbor->path, neighbor);
                neighbor->path = NULL;
            }

            sendUpdatesToNeighbor(neighbor);
        }
    }
}
