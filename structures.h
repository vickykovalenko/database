#ifndef structures_h
#define structures_h


#include <stdio.h>
#include <stdbool.h>

struct Department //master
{
    int department_id;
    char department_name[15];
    char special_courses[15];
    int quantity_staff;
    long firstStaffAddress;
};

struct Staff //slave
{
    int staff_id;
    char staff_name[15];
    char staff_surname[15];
    double staff_salary;
    char staff_email[25];
    char staff_login[25];
    char staff_password[15];
    char stream[15];
    long nextAddress;
    long thisAddress;
    int exists;

};

struct IndexRecord
{
    int key;
    unsigned long record;
    int exists;
};
#endif /* structures_h */

