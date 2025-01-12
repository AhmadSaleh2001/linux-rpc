#pragma once

typedef void (*PrintFunction)(void *);
typedef void (*SerializeFunction)(void *, void *);
typedef void * (*DeSerializeFunction)(void *);

typedef struct generic_node_ {

    SerializeFunction serialize;
    DeSerializeFunction deserialize;
    PrintFunction print;
    void * value;
    struct node_ * next;
    
} generic_node_t;

void generic_print_node(generic_node_t * node);