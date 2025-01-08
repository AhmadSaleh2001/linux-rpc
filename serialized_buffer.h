#pragma once
#include "student.h"
#include "department.h"
#include "linkedlist/node.h"
#include "linkedlist/linkedlist.h"

#define SERIALIZED_BUFFER_DEFAULT_SIZE 100

typedef struct serialized_buffer {
    char *b;
    int size;
    int next;
} serialized_buffer_t;

void init_serializd_buffer(serialized_buffer_t ** b);
void serialize_data(serialized_buffer_t *b, char * data, int nbytes);
void deserialize_data(serialized_buffer_t *b, char * data, int size);
void serialize_buffer_skip(serialized_buffer_t *b, int skip_size);
void free_serialize_buffer(serialized_buffer_t *b);


void serialize_student(serialized_buffer_t *b, student_t * std);
student_t * dserialize_student(serialized_buffer_t *b);

void serialize_department(serialized_buffer_t *b, department_t * dep);
department_t * dserialize_department(serialized_buffer_t *b);

void serialize_node(serialized_buffer_t *b, node_t * node);
node_t * dserialize_node(serialized_buffer_t *b);

void serialize_linkedlist(serialized_buffer_t *b, linkedlist_t * linkedlist);
linkedlist_t * dserialize_linkedlist(serialized_buffer_t *b);