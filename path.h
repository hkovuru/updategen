/*
 * =====================================================================================
 *
 *       Filename:  path.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2019 01:41:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PATH_H__

#define __PATH_H__

#include "neighbor.h"

#include <stdint.h>

#define ATTR_LEN 19


typedef struct Path {
    char attr[ATTR_LEN];
    int attrId;
    struct Neighbor *neighborList;
    struct Path *next;
} Path;

int addPath(char *prefix, uint16_t attrId);
Path *getPathList();
void addNeighborToPath(Path *path, struct Neighbor *neighbor);
void removeNeighborFromPath(Path *path, struct Neighbor *neighbor);

#endif

