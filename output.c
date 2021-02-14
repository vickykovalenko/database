#include "output.h"
#include "structures.h"
#include "input.h"
#include "calculate.h"
#include <stdbool.h>

void ut_m(struct Department department)
{
    printf("Department's id: %d\n", department.department_id);
    printf("Department's name: %s\n", department.department_name);
    printf("Department's special courses: %s\n", department.special_courses);
    printf("Department's quantity staff: %d\n", department.quantity_staff);
    printf("Department's firstStaffAddress: %ld\n", department.firstStaffAddress);
}

void ut_s(struct Staff staff)
{
    printf("Staff's id:  %d\n", staff.staff_id);
    printf("Staff's name: %s\n", staff.staff_name);
    printf("Staff's surname: %s\n", staff.staff_surname);
    printf("Staff's salary: %f\n", staff.staff_salary);
    printf("Staff's email: %s\n", staff.staff_email);
    printf("Staff's login: %s\n", staff.staff_login);
    printf("Staff's password: %s\n", staff.staff_password);
    printf("Staff's stream: %s\n", staff.stream);
    printf("Staff's nextAddress: %ld\n", staff.nextAddress);
    printf("Staff's thisAddress: %ld\n", staff.thisAddress);
    printf("Staff's exists: %d\n", staff.exists);

}

void ut2_m(void)
{
    if(calc_m() == 0)
    {
        printf("No departments in the table.\n");
        return;
    }
    FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "rb");
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "rb");
    
    
    if (indexTable == NULL || recordTable == NULL)
    {
        printf("Department.ind or Department.fl do not exist.\n");
        return;
    }
   
    struct Department department;
    struct IndexRecord indexRecord;
   
    fseek(recordTable, 0 , SEEK_END);
    long size = ftell(recordTable)/sizeof(struct Department);
    
    for(size_t i = 0; i < size; i++)
    {
        fseek(indexTable, i * sizeof(struct IndexRecord), SEEK_SET);
        fread(&indexRecord, sizeof(struct IndexRecord), 1, indexTable);
        
        if(indexRecord.exists)
        {

            fseek(recordTable, indexRecord.record,  SEEK_SET);// move the pointer to the found address
            fread(&department, sizeof(struct Department), 1, recordTable);// read structure department from the binary file
            ut_m(department);
            printf("%s\n", "--------------------------------------------------");
            
        }
        else{
            continue;
        }
    }
    
    fclose(indexTable);
    fclose(recordTable);

    return;
}
   
void ut2_s(void)
{
    if(calc2_s() == 0)
    {
        printf("No staff in the table.\n");
        return;
    }
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "rb");
    
    struct Staff staff;

    fseek(recordTable, 0 , SEEK_END);

    long size = ftell(recordTable)/sizeof(struct Staff);
 
       for(size_t i = 0; i < size; i++)
       {
           fseek(recordTable, i * sizeof(struct Staff), SEEK_SET);
           fread(&staff, sizeof(struct Staff), 1, recordTable);
           
           if(staff.exists)
           {
               ut_s(staff);
               printf("%s\n", "--------------------------------------------------");
               
           }
           else{
               continue;
           }
       }
    
       fclose(recordTable);
}
