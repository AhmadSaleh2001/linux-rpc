#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialized_buffer.h"
#include "student.h"
#include "department.h"

void init_serializd_buffer(serialized_buffer_t ** b) {
    (*b) = calloc(1, sizeof(serialized_buffer_t));
    (*b)->b = calloc(SERIALIZED_BUFFER_DEFAULT_SIZE, sizeof(char));
    (*b)->size = SERIALIZED_BUFFER_DEFAULT_SIZE;
    (*b)->next = 0;
}

void serialize_data(serialized_buffer_t *b, char * data, int nbytes) {
    int available_size = b->size - b->next;
    bool should_resize = false;
    while(available_size < nbytes) {
        b->size<<=1;
        available_size = b->size - b->next;
        should_resize = true;
    }

    if(should_resize) {
        b->b = realloc(b->b, b->size);
    }

    
    memcpy((char*)b->b + b->next, data, nbytes);
    b->next+=nbytes;
}

void deserialize_data(serialized_buffer_t *b, char * data, int size) {
    memcpy(data, (char*)b->b + b->next, size);
    b->next+=size;
}

void serialize_buffer_skip(serialized_buffer_t *b, int skip_size) {
    if(b->next + skip_size >= 0 && b->next + skip_size < b->size) {
        b->next+=skip_size;
    }
}

void free_serialize_buffer(serialized_buffer_t *b) {
    free(b->b);
    free(b);
}

void serialize_student(serialized_buffer_t *b, student_t * std) {

    SENTINEL_CHECK_SERIALIZATION(b, std)

    serialize_data(b, &std->name, sizeof(char) * 30);
    serialize_data(b, &std->age, sizeof(int));
    serialize_data(b, &std->mark, sizeof(int));
    serialize_department(b, &std->dep);

}

student_t * dserialize_student(serialized_buffer_t *b) {
    SENTINEL_CHECK_DESERIALIZATION(b)

    student_t * std = calloc(1, sizeof(student_t));
    deserialize_data(b, &std->name, sizeof(char)*30);
    deserialize_data(b, &std->age, sizeof(int));
    deserialize_data(b, &std->mark, sizeof(int));
    std->dep = *dserialize_department(b);

    return std;
}

void serialize_department(serialized_buffer_t *b, department_t * dep) {
    SENTINEL_CHECK_SERIALIZATION(b, dep)

    serialize_data(b, &dep->department, sizeof(char) * 30);
    serialize_data(b, &dep->dep_code, sizeof(int));
}

department_t * dserialize_department(serialized_buffer_t *b) {
    SENTINEL_CHECK_DESERIALIZATION(b)

    department_t * dep = calloc(1, sizeof(department_t));
    deserialize_data(b, &dep->department, sizeof(char)*30);
    deserialize_data(b, &dep->dep_code, sizeof(int));

    return dep;
}

void serialize_node(serialized_buffer_t *b, node_t * node) {

    SENTINEL_CHECK_SERIALIZATION(b, node)

    serialize_data(b, &node->value, sizeof(int));
    serialize_data(b, &node->next, sizeof(node_t*));
    serialize_node(b, node->next);
}

node_t * dserialize_node(serialized_buffer_t *b) {

    SENTINEL_CHECK_DESERIALIZATION(b)

    node_t * node = calloc(1, sizeof(node_t));
    deserialize_data(b, &node->value, sizeof(int));
    deserialize_data(b, &node->next, sizeof(node_t*));
    node->next = dserialize_node(b);

    return node;
}


void serialize_linkedlist(serialized_buffer_t *b, linkedlist_t * linkedlist) {
    SENTINEL_CHECK_SERIALIZATION(b, linkedlist)

    serialize_data(b, &linkedlist->head, sizeof(node_t*));
    serialize_node(b, linkedlist->head);
}

linkedlist_t * dserialize_linkedlist(serialized_buffer_t *b) {
    SENTINEL_CHECK_DESERIALIZATION(b)

    linkedlist_t * linkedlist = calloc(1, sizeof(linkedlist_t));
    deserialize_data(b, &linkedlist->head, sizeof(node_t*));
    linkedlist->head = dserialize_node(b);

    return linkedlist;
}