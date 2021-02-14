#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "delete.h"
#include "input.h"
#include "output.h"
#include "update.h"
#include "delete.h"
#include "calculate.h"
#include "structures.h"

int main()
{
    struct Department department;
    struct Staff staff;
    int department_id;
    int staff_id;
    int count;
    while(1)
    {
        int option;
        printf("Choose an option:\n"
               "1. get-m \n"
               "2. get-s \n"
               "3. del-m \n"
               "4. del-s \n"
               "5. update-m \n"
               "6. update-s \n"
               "7. insert-m \n"
               "8. insert-s \n"
               "9. calc-m \n"
               "10. calc-s \n"
               "11. ut-m \n"
               "12. ut-s \n"
               );
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(get_m(&department, department_id))
                {ut_m(department);}
                else
                {
                    printf("Fail to get department.\n");
                }
                break;
            case 2:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(get_m(&department, department_id))
                {
                    printf("Enter Staff ID: ");
                    scanf("%d", &staff_id);
                    if(get_s(department, &staff, staff_id))
                    {ut_s(staff);}
                    else
                    {
                        printf("Fail to get staff.\n");
                    }
                }
                break;
            case 3:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(del_m(department_id))
                {printf("Successfully deleted.\n");}
                else
                {
                    printf("Fail to delete.\n");
                }
                break;
            case 4:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(get_m(&department, department_id))
                {
                    printf("Enter Staff ID: ");
                    scanf("%d", &staff_id);
                    if(get_s(department, &staff, staff_id))
                    {
                        del_s(department, staff, staff_id);
                        printf("Successfully deleted.\n");
                    }
                }
                else
                {
                    printf("Fail to delete.\n");
                }
                break;
            case 5:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(get_m(&department, department_id))
                {printf("Enter Department Name: ");
                char department_name[15];
                scanf("%s", department_name);
                strcpy(department.department_name, department_name);
                printf("Enter Special Course: ");
                char special_courses[15];
                scanf("%s", special_courses);
                strcpy(department.special_courses, special_courses);
                if(update_m(department))
                {
                    printf("Successfully updated.\n");
                }
                else
                {
                    printf("Fail to update.\n");
                }}
                else
                {
                    printf("Fail to get department.\n");

                }
                break;
            case 6:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(get_m(&department, department_id))
                {
                    printf("Enter Staff ID: ");
                    scanf("%d", &staff_id);
                    if(get_s(department, &staff, staff_id))
                    {
                        inputStaff(&staff);
                        update_s(staff, staff_id);
                        printf("Successfully updated.\n");
                    }
                    else{
                         printf("Fail to update.\n");
                    }
                }
                else
                {
                    printf("Error.\n");
                }
            break;
            case 7:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                department.department_id = department_id;
                printf("Enter Department Name: ");
                char department_name[15];
                scanf("%s", department_name);
                strcpy(department.department_name, department_name);
                char special_courses[15];
                printf("Enter Special Course: ");
                scanf("%s", special_courses);
                strcpy(department.special_courses, special_courses);
                if(insert_m(department))
                {printf("Successfully inserted.\n");}
                else{
                    printf("Error.\n");}
                break;
            case 8:
                printf("Enter Department ID: ");
                scanf("%d", &department_id);
                if(get_m(&department, department_id))
                {
                    printf("Enter Staff ID: ");
                    scanf("%d", &staff_id);
                    staff.staff_id = staff_id;
                    inputStaff(&staff);
                    insert_s(department, staff);
                    printf("Successfully inserted.\n");
                }
                else{
                    printf("Error.\n");
                }
                break;
            case 9:
                count = calc_m();
                printf("Total amount of departments is: ");
                printf("%d\n", count);
                break;
            case 10:
                calc_s();
                break;
            case 11:
                ut2_m();
                break;
            case 12:
                ut2_s();
                break;
            default:
                printf("An error occured. Try again.\n");
            
        }
    printf("%s\n", "**************************************************");
    }
    return 0;
}
