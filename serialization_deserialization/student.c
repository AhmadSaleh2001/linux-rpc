#include <stdio.h>
#include "student.h"
#include "department.h"

void print_student(student_t * std) {
    if(!std)return;
    printf("***** Student Info *****\n");
    printf("Student name: %s\n", std->name);
    printf("Student age: %d\n", std->age);
    printf("Student mark: %d\n", std->mark);
    print_department(&std->dep);
    printf("***** Student Info *****\n");
}
