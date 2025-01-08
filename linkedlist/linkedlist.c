#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "node.h"

void add_first(linkedlist_t * l, int value) {
    node_t * newNode = calloc(1, sizeof(node_t));
    newNode->value = value;
    newNode->next = NULL;
    if(!l->head) {
        l->head = newNode;
    } else {
        newNode->next = l->head;
        l->head = newNode;
    }
}

void print_linkedlist_node(node_t * head) {
    if(!head){
        printf("NULL");
        return;
    }
    print_node(head);
    printf(" -> ");
    print_linkedlist_node(head->next);
}

void print_linkedlist(linkedlist_t * l) {
    printf("***** Linkedlist Info ***** \n");
    print_linkedlist_node(l->head);
    printf("\n***** Linkedlist Info ***** \n");
}