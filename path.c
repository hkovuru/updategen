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

int addPath(char *attr, uint16_t attrId) {
    int count = 0;
    int found = 0;
    Path *prevP = NULL, *currP = NULL;
    Path *path = NULL;
    Neighbor *neighborP = NULL;

    for(currP = pathListG;currP != NULL; currP = currP->next) {
        prevP = currP;
        if (!strncmp(currP->attr, attr, ATTR_LEN)) {
            break;
        }
    }

    if (currP == NULL) { // This is the first or last node in the path list
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
    } else {
        prevP->next = currP->next;
        if (prevP->neighborList == NULL) {
            prevP->neighborList = currP->neighborList;
        } else {
            neighborP = prevP->neighborList;
            while (neighborP->pathListNext != NULL) {
                neighborP = neighborP->pathListNext;
            }

            neighborP->pathListNext = currP->neighborList;
        }

        while(prevP->next != NULL) {
            prevP = prevP->next;
        }

        currP->attrId = attrId;
        currP->next = NULL;
        currP->neighborList = NULL;
        prevP->next = currP;
    }

    return 0;
}

void addNeighborToPath(Path *path, Neighbor *neighbor) {
    neighbor->pathListNext = path->neighborList;
    path->neighborList = neighbor;
    return;
}

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

