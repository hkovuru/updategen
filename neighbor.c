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

Neighbor *getNeighborsList() {
    return neighborListG;
}

// Add a neighbor at the front of the neighbor list
int addNeighbor(char *address) {
    int rv = 0;
    Neighbor *neighbor = (Neighbor *)malloc(sizeof(Neighbor));
    if (neighbor == NULL) {
        printf("Failed to allocate memory for neighbor %s\n", address);
        return -1;
    }

    strncpy(neighbor->address, address, ADDRESS_LEN);
    neighbor->next = neighborListG;
    neighborListG = neighbor;

    return 0;
}

// Send update message for a neighbor
// Param neighbor - pointer to Neighbor struct
// Param all - 1 means send all path updates, 0 means send max of 10 updates
void sendUpdatesToNeighbor(Neighbor *neighbor, int all) {
    Path *path, *prevPath;
    uint8_t count;

    if (neighbor != NULL) {
        count = 0;
        path = NULL;
        
        if (neighbor->path == NULL) { // Updates have not been sent for this neighbor yet
            path = getPathHead();
        } else { // The last update for this neighbor is for neighbor->path
            prevPath = neighbor->path;
            path = prevPath->next;
        }

        // Send path update until path is NULL
        while (path != NULL) {
            // The attr is NULL, this node does not have valid data. It will be remove when none of
            // the neighbors are pointing to this path.
            if (path->attr == NULL) {
                path = path->next;
                continue;
            }

            prevPath = path;
            count++;
            printf("Generated update with path %s attribute-id %d for neighbor %s\n", path->attr, path->attrId, neighbor->address);
            // Break if sent 10 updates and all is set to 0
            if ((!all) && (count == 10)) {
                break;
            }

            path = path->next;
        }

        // Remove the neighbor from the old neighbor->path's neighbor list
        if (neighbor->path != NULL) {
            decrNeighborCount(neighbor->path);
        }
        // Update the neighbor to reflect the last path update that was sent
        neighbor->path = prevPath;
        // Add neighbor to the new path's neighbor list
        incrNeighborCount(prevPath);
    }
}

// Send updates to all neighbors
// Param - all: 1 - send all path updates, 0 - send a max of 10 updates
void sendUpdates(int all) {
    Neighbor *neighbor;
    Path *pathList = getPathHead();

    if (pathList == NULL) {
        printf("updates list is empty\n");
        return;
    }

    for(neighbor = neighborListG;neighbor != NULL; neighbor = neighbor->next) {
        sendUpdatesToNeighbor(neighbor, all);
    }
}

// Route refresh
// Param address - Neighbor address
void routeRefresh(char *address) {
    Neighbor *neighbor;
    Path *pathList = getPathHead();

    if (pathList == NULL) {
        //printf("updates list is empty\n");
        return;
    }

    for(neighbor = neighborListG;neighbor != NULL; neighbor = neighbor->next) {
        if (!strncmp(neighbor->address, address, ADDRESS_LEN)) {
            // Reset the previous path. this will lead resending all updates
            if (neighbor->path != NULL) {
                decrNeighborCount(neighbor->path);
                neighbor->path = NULL;
            }

            sendUpdatesToNeighbor(neighbor, 0);
        }
    }
}
