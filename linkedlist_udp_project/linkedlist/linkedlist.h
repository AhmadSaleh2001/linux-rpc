#pragma once
#include "node.h"

typedef struct linkedlist_ {
    node_t * head;
} linkedlist_t;

void add_first(linkedlist_t * l, int value);
void print_linkedlist(linkedlist_t * l);
