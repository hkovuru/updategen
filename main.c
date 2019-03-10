/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2019 08:42:15 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Harsha Kovuru
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "path.h"
#include "neighbor.h"

#define LINE_LEN 250

int processPath(char *prefix, uint16_t attrId);
int processUpdate();
int processRouteRefresh(char *neighbor);

int main() {
    FILE *f;
    char line[LINE_LEN];
    char delims[3] = {' ', '\n', '\0'};
    char *token, *prefix, *neighbor;
    uint16_t attrId;

    addNeighbor("1.1.1.1");
    addNeighbor("2.2.2.2");

    f = fopen("./input.txt", "r");
    if (f == NULL) {
        printf("Opening file failed\n");
        return -1;
    }

    while(fgets(line, LINE_LEN, f) != NULL) {
        //printf("line = %s", line, strlen(line));
        token = strtok(line, delims);
        if (token == NULL) {
            continue;
        }

        if (!strcmp(token, "path")) {
            prefix = strtok(NULL, delims);
            token = strtok(NULL, delims);
            attrId = atoi(token);
            processPath(prefix, attrId);
        } else if (!strcmp(token, "update")) {
            processUpdate();
        } else if (!strcmp(token, "route-refresh")) {
            strtok(NULL, delims);
            neighbor = strtok(NULL, delims);
            processRouteRefresh(neighbor);
        } else {
            printf("no valid command in the line\n");
        }
    }

    fclose(f);
}

int processPath(char *prefix, uint16_t attrId) {
    //printf("processPath: prefix=%s, attrId=%d\n", prefix, attrId);
    addPath(prefix, attrId);
    return 0;
}

int processUpdate() {
    //printf("processUpdate\n");
    sendUpdates();
    return 0;
}

int processRouteRefresh(char *neighbor) {
    //printf("processRouteRefresh: neighbor=%s\n", neighbor);
    routeRefresh(neighbor);
    return 0;
}

