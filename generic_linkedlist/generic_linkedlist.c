#include <stdio.h>
#include <stdlib.h>

#include "generic_linkedlist.h"
#include "generic_node.h"

void generic_add_first(generic_linkedlist_t * l, PrintFunction print, void * value) {
    generic_node_t * newNode = calloc(1, sizeof(generic_node_t));
    newNode->value = value;
    newNode->print = print;
    newNode->next = NULL;
    if(!l->head) {
        l->head = newNode;
    } else {
        newNode->next = l->head;
        l->head = newNode;
    }
}

void generic_print_linkedlist_node(generic_node_t * head) {
    if(!head){
        printf("NULL");
        return;
    }
    generic_print_node(head);
    printf(" -> ");
    generic_print_linkedlist_node(head->next);
}

void generic_print_linkedlist(generic_linkedlist_t * l) {
    printf("***** Linkedlist Info ***** \n");
    generic_print_linkedlist_node(l->head);
    printf("\n***** Linkedlist Info ***** \n");
}