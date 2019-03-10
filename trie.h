/*
 * =====================================================================================
 *
 *       Filename:  trie.h
 *
 *    Description:  Trie implementation
 *
 *        Version:  1.0
 *        Created:  03/10/2019 06:17:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __TRIE_H__

#define __TRIE_H__

#include <stdint.h>

#define NUM_CHILDREN 12

// Trie data structure
// The key to the children array is a byte in the path attr. The valid values are (0-9,.,/)
typedef struct Node {
    struct Path *data;                      // Pointer to the data
    struct Node *children[NUM_CHILDREN];    // Pointer to the child nodes
} Node;

int addPath(char *prefix, uint16_t attrId);

#endif

