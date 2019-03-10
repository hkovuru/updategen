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

/*
 * This is the structure for paths. It is a linked list of paths.
 * The member neighborList is to maintain a reference to the neighbors for which this path is the last update sent.
 */
typedef struct Path {
    char attr[ATTR_LEN];            // Path attr (IP prefix), max len is 19
    uint16_t attrId;                // Attribute id to check if the attribute was updated
    uint8_t numNeighbors;           // The number of neighbors that sent this path as their last update
    struct Path *next;              // Pointer to next Path element in the list
} Path;

int addPath(char *prefix, uint16_t attrId);
Path *getPathList();
void incrNeighborCount(Path *path);
void decrNeighborCount(Path *path);

#endif

