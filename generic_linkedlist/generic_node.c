#include <stdio.h>
#include "generic_node.h"

void generic_print_node(generic_node_t * node) {
    node->print(node->value);
}