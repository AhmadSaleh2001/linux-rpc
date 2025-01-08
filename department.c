#include <stdio.h>
#include "department.h"

void print_department(department_t * dep) {
    printf("***** Department Info *****\n");
    printf("Department name: %s\n", dep->department);
    printf("Department code: %d\n", dep->dep_code);
    printf("***** Department Info *****\n");
}