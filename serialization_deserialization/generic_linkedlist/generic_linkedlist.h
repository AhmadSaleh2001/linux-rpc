#pragma once
#include "generic_node.h"

typedef struct generic_linkedlist_ {
    generic_node_t * head;
} generic_linkedlist_t;

void generic_add_first(generic_linkedlist_t * l, PrintFunction print, SerializeFunction serialize, DeSerializeFunction deserialize, void * value);
void generic_print_linkedlist(generic_linkedlist_t * l);
