/*
 * =====================================================================================
 *
 *       Filename:  path.c
 *
 *    Description:  Path structure and path list implementation
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

Path *pathHeadG = NULL;
Path *pathTailG = NULL;

Path *getPathHead() {
    return pathHeadG;
}

Path *getPathTail() {
    return pathTailG;
}

// Create a Path struct and set the members using the arguments
Path *createPath(char *attr, uint16_t attrId) {
    char *newAttr;
    Path *path = malloc(sizeof(Path));
    if (path == NULL) {
        printf("failed to allocate memory for path %s id %d\n", attr, attrId);
    }

    newAttr = (char *)malloc(ATTR_LEN);
    if (newAttr == NULL) {
        printf("failed to allocate memory for attr %s\n", attr);
        return NULL;
    }

    strncpy(newAttr, attr, ATTR_LEN);
    path->attr = newAttr;
    path->attrId = attrId;
    path->numNeighbors = 0;
    path->next = NULL;
    path->prev = NULL;
    return path;
}

// Create a Path from an existing path and populate the path attributes attr and attr id
Path *clonePath(Path *path) {
    Path *newPath = malloc(sizeof(Path));
    if (newPath == NULL) {
        printf("failed to allocate memory for path %s id %d\n", path->attr, path->attrId);
        return NULL;
    }

    newPath->attr = path->attr;
    newPath->attrId = path->attrId;
    newPath->numNeighbors = 0;
    newPath->next = NULL;
    newPath->prev = NULL;
    return newPath;
}

// Delete the path from the path list
void deletePath(Path *path) {
    if (path->prev != NULL) {
        path->prev->next = path->next;

        if (pathTailG == path) {
            pathTailG = path->prev;
        }
    }

    if (path->next != NULL) {
        path->next->prev = path->prev;
        if (pathHeadG == path) {
            pathHeadG = path->next;
        }
    }
}

// Add a path at the end of the list
Path *appendPath(char *attr, uint16_t attrId) {
    Path *path = createPath(attr, attrId);
    if (path == NULL) {
        return NULL;
    }

    if (pathHeadG == NULL) {
        pathHeadG = path;
        pathTailG = path;
    } else {
        path->prev = pathTailG;
        pathTailG->next = path;
        pathTailG = path;
    }

    return path;
}

// Update the path with the new attr id
Path *updatePath(Path *path, uint16_t attrId) {
    Path *newPath;

    if (path != NULL) {
        if (path->numNeighbors > 0) {
            // If this the last update sent by any of the neighbors, keep this path in the
            // paths list and add a new one at the end of the list. Set the path attr member
            // to NULL so that we know this path is invalid and need to be removed when there
            // is no neighbor pointing to this path.
            path->attrId = attrId;
            newPath = clonePath(path);
            path->attr = NULL;
            path->attrId = 0;
        } else {
            // Remove the path from the paths list and add it at the end
            deletePath(path);
            newPath = path;
            newPath->attrId = attrId;
        }

        // Add the new path at the end of the paths list
        if (pathTailG != newPath) {
            pathTailG->next = newPath;
            newPath->prev = pathTailG;
            pathTailG = newPath;
            pathTailG->next = NULL;
        }

        return newPath;
    }

    return NULL;
}

// Increment neighbor count for path
void incrNeighborCount(Path *path) {
    if (path != NULL) {
        (path->numNeighbors)++;
    }
    return;
}

// Decrement neighbor count for path
void decrNeighborCount(Path *path) {
    if (path != NULL) {
        (path->numNeighbors)--;
        // If the neighbor count is 0 and the path does not have attr set, remove the path from the paths list
        if ((path->attr == NULL) && (path->numNeighbors == 0)) {
            deletePath(path);
        }
    }
    return;
}

