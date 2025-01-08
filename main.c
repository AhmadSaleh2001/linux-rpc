#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialized_buffer.h"
#include "student.h"
#include "department.h"

int main() {

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
    
    return 0;
}