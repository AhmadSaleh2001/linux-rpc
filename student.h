#pragma once
#include "department.h"

typedef struct student_ {
    char name[30];
    int age;
    int mark;
    department_t dep;
} student_t ;

void print_student(student_t * std);