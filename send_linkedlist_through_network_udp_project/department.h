#pragma once

typedef struct department_ {
    char department[30];
    int dep_code;
} department_t ;

void print_department(department_t * dep);