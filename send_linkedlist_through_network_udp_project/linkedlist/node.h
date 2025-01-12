#pragma once

typedef struct node_ {
    int value;
    struct node_ * next;
} node_t;

void print_node(node_t * node);