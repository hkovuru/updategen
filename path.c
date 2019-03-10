/*
 * =====================================================================================
 *
 *       Filename:  path.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2019 03:07:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */

#include "path.h"
#include "neighbor.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

Path *pathListG = NULL;

Path *getPathList() {
    return pathListG;
}

// Add path to the global path list
// Param - attr: path attr
// Param - attrId: attr id for the path attr
int addPath(char *attr, uint16_t attrId) {
    int count = 0;
    int found = 0;
    Path *prevP = NULL, *currP = NULL;
    Path *path = NULL;
    Neighbor *neighborP = NULL;

    // Check if the path attr exists in the global path list
    for(currP = pathListG;currP != NULL; currP = currP->next) {
        if (!strncmp(currP->attr, attr, ATTR_LEN)) {
            break;
        }

        prevP = currP;
    }

    if (currP == NULL) { // This means either the global path list is empty or a new path is being added
        Path *path = (Path *)malloc(sizeof(Path));
        if (path == NULL) {
            printf("failed to allocate memory for path, attr=%s\n", attr);
            return -1;
        }

        strncpy(path->attr, attr, ATTR_LEN);
        path->attrId = attrId;
        path->next = NULL;
        path->neighborList = NULL;

        if (prevP == NULL) { // This path is added at the start of the list
            pathListG = path;
        } else { // This path is added at the end of the list
            prevP->next = path;
        }
    } else { // This is a existing path, move it to the end of the path list
        prevP->next = currP->next;
        // Update the neighbor list for this path
        if (prevP->neighborList == NULL) {
            prevP->neighborList = currP->neighborList;
        } else {
            neighborP = prevP->neighborList;
            while (neighborP->pathListNext != NULL) {
                neighborP = neighborP->pathListNext;
            }

            neighborP->pathListNext = currP->neighborList;
        }

        // Traverse to the end of the path list
        while(prevP->next != NULL) {
            prevP = prevP->next;
        }

        currP->attrId = attrId;
        currP->next = NULL;
        currP->neighborList = NULL;
        // Add the updated path at the end of the path list
        prevP->next = currP;
    }

    return 0;
}

// Add neighbor to the Path's neighbor list. This means that this is the last path update the neighbor sent out.
void addNeighborToPath(Path *path, Neighbor *neighbor) {
    neighbor->pathListNext = path->neighborList;
    path->neighborList = neighbor;
    return;
}

// Remove neighbor from the Path's neighbor list.
void removeNeighborFromPath(Path *path, Neighbor *neighbor) {
    Neighbor *neighborP;

    if (neighbor == NULL) {
        return;
    }

    if (path->neighborList == NULL) {
        return;
    }

    if (path->neighborList == neighbor) {
        path->neighborList = neighbor->pathListNext;
        neighbor->pathListNext = NULL;
    } else {
        for (neighborP = path->neighborList;neighborP->pathListNext != NULL;neighborP = neighborP->pathListNext) {
            if (neighborP->pathListNext == neighbor) {
                neighborP->pathListNext = neighbor->pathListNext;
                neighbor->pathListNext = NULL;
                break;
            }
        }
    }
}

