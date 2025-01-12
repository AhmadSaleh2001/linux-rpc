#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialized_buffer.h"
#include "student.h"
#include "department.h"
#include "linkedlist/linkedlist.h"
#include "generic_linkedlist/generic_node.h"
#include "generic_linkedlist/generic_linkedlist.h"

void test_serialize_student() {
    serialized_buffer_t * b = NULL;
    init_serializd_buffer(&b);

    student_t * std = malloc(sizeof(student_t));
    memcpy(std->name, "ahmad", 5);
    std->age = 20;
    std->mark = 99;
    memcpy(std->dep.department, "computer", 8);
    std->dep.dep_code = 11;

    serialize_student(b, std);
    printf("ptr: %d\n", b->next);
    serialize_buffer_skip(b, -b->next);

    student_t * std_deserialize = dserialize_student(b);
    print_student(std_deserialize);
}

void test_serialize_linkedlist() {
    linkedlist_t * l = malloc(sizeof(linkedlist_t));
    add_first(l, 1);
    add_first(l, 2);
    add_first(l, 3);
    print_linkedlist(l);

    printf("\n");

    serialized_buffer_t * b = NULL;
    init_serializd_buffer(&b);

    serialize_linkedlist(b, l);
    serialize_buffer_skip(b, -b->next);

    linkedlist_t * linkedlist2 = dserialize_linkedlist(b);
    print_linkedlist(linkedlist2);
}

void printStudentNode(void * data) {
    student_t * std = (student_t *)data;
    print_student(std);
}

void printDepartmentNode(void * data) {
    department_t * dep = (department_t *)data;
    print_department(dep);
}

void serialize_department_main(void *buffer, void * data) {

    serialized_buffer_t *b = (serialized_buffer_t *)buffer;
    department_t * dep = (department_t *)data;

    SENTINEL_CHECK_SERIALIZATION(b, dep)

    serialize_data(b, &dep->department, sizeof(char) * 30);
    serialize_data(b, &dep->dep_code, sizeof(int));
}

void * dserialize_department_main(void *buffer) {

    serialized_buffer_t *b = (serialized_buffer_t *)buffer;

    SENTINEL_CHECK_DESERIALIZATION(b)

    department_t * dep = calloc(1, sizeof(department_t));
    deserialize_data(b, &dep->department, sizeof(char)*30);
    deserialize_data(b, &dep->dep_code, sizeof(int));

    return dep;
}

void serialize_student_main(void * buffer, void * data) {

    serialized_buffer_t *b = (serialized_buffer_t *)buffer;
    student_t * std = (student_t *)data;

    SENTINEL_CHECK_SERIALIZATION(b, std)

    serialize_data(b, &std->name, sizeof(char) * 30);
    serialize_data(b, &std->age, sizeof(int));
    serialize_data(b, &std->mark, sizeof(int));
    serialize_department(b, &std->dep);

}

void * dserialize_student_main(void *buffer) {

    serialized_buffer_t *b = (serialized_buffer_t *)buffer;

    SENTINEL_CHECK_DESERIALIZATION(b)
    
    student_t * std = calloc(1, sizeof(student_t));
    deserialize_data(b, &std->name, sizeof(char)*30);
    deserialize_data(b, &std->age, sizeof(int));
    deserialize_data(b, &std->mark, sizeof(int));
    std->dep = *dserialize_department(b);

    return std;
}

int main() {

    generic_linkedlist_t * l = malloc(sizeof(generic_linkedlist_t));
    student_t * std = malloc(sizeof(student_t));
    memcpy(std->name, "ahmad", 5);
    std->age = 20;
    std->mark = 99;
    memcpy(std->dep.department, "computer", 8);
    std->dep.dep_code = 11;

    department_t * dep = malloc(sizeof(department_t));
    dep->dep_code = 99;
    memcpy(dep->department, "math", 4);

    generic_add_first(l, printStudentNode, serialize_student_main, dserialize_student_main, std);
    generic_add_first(l, printDepartmentNode, serialize_department_main, dserialize_department_main, dep);

    generic_print_linkedlist(l);
    
    return 0;
}