/*
 * =====================================================================================
 *
 *       Filename:  trie.c
 *
 *    Description:  Trie implementation
 *
 *        Version:  1.0
 *        Created:  03/10/2019 06:28:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */

#include "trie.h"
#include "path.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

Node *root;

// Initialize the root of the trie
int initTrie() {
    root = (Node *)malloc(sizeof(Node));
    if (root == NULL) {
        printf("failed to allocate memory for root\n");
        return 1;
    }

    return 0;
}

// Insert a node int the trie
Node *insertNode(char *attr) {
    int index = 0, len = 0;
    Node *node = root, *newNode = NULL;

    len = strlen(attr);
    // Iterate until there is a byte match in the trie
    while ((index < len) && ((node->children[attr[index] - '.']) != NULL)) {
        node = node->children[attr[index] - '.'];
        index++;
    }
    
    // When there is no match, starting adding the path attr into the trie, one byte at a time
    while (index < len) {
        newNode = (Node *)malloc(sizeof(Node));
        if (newNode ==  NULL) {
            printf("failed to allocate memory for trie node, attr = %s\n", attr);
            return NULL;
        }

        node->children[attr[index] - '.'] = newNode;
        node = newNode;
        index++;
    }

    return node;
}

// Get a node from the trie for attr
Node *getNode(char *attr) {
    int index = 0, len = 0;
    Node *node = root;

    len = strlen(attr);
    while ((index < len) && ((node->children[attr[index] - '.']) != NULL)) {
        node = node->children[attr[index] - '.'];
        index++;
    }

    if (index == len) {
        return node;
    }

    return NULL;
}

// Set the data in the trie node
int setData(Node *node, char *attr, uint16_t attrId) {
    Path *data = appendPath(attr, attrId);
    node->data = data;
}

// Update the data in the trie node
int updateData(Node *node, uint16_t attrId) {
    Path *data = updatePath(node->data, attrId);
    node->data = data;
}

// Add a path into the trie
int addPath(char *attr, uint16_t attrId) {
    Node *node = insertNode(attr);
    if (node == NULL) {
        printf("failed to add trie node for attr %s attr id %d\n", attr, attrId);
        return 1;
    }

    if (node->data == NULL) {
        setData(node, attr, attrId);
    } else {
        updateData(node, attrId);
    }
}
