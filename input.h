#ifndef input_h
#define input_h

#include <stdio.h>
#include "structures.h"

int get_m(struct Department* department, int department_id);
int get_s(struct Department department, struct Staff* staff, int staff_id);
int insert_m(struct Department department);
void inputStaff(struct Staff* staff);
void rewriteData(int garbageCount, FILE* garbageFile, struct Department* department);
int insert_s(struct Department department, struct Staff staff);
void linkAddresses(FILE* recordTable, struct Department department, struct Staff staff);
void rewriteAddress(int garbageCount, FILE* garbageFile, struct Staff* staff);



#endif /* input_h */
