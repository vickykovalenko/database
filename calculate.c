#include "calculate.h"
#include "structures.h"

int calc_m(void)
{
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "rb");
    FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "rb");
    
    struct IndexRecord IndexRecord;

    fseek(recordTable, 0 , SEEK_END);
    long size = ftell(recordTable)/sizeof(struct Department);
    int count = 0;
    for(size_t i = 0; i < size; i++)
    {
        fseek(indexTable, i * sizeof(struct IndexRecord), SEEK_SET);
        fread(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable);
        
        if(IndexRecord.exists)
        {
            count++;
        }
        else{
            continue;
        }
    }

    fclose(indexTable);
    fclose(recordTable);
    return count;
}

int calc2_s(void)
{
    FILE* recordTable2 = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "rb");

    struct Staff staff;

    fseek(recordTable2, 0 , SEEK_END);
    long size = ftell(recordTable2)/sizeof(struct Staff);
    int count = 0;
    for(size_t i = 0; i < size; i++)
    {
        fseek(recordTable2, i * sizeof(struct Staff), SEEK_SET);
        fread(&staff, sizeof(struct Staff), 1, recordTable2);
            
        if(staff.exists)
        {
            count++;
        }
        else
        {
            continue;
        }
    }
    fclose(recordTable2);
    return count;
    
}

void calc_s(void)
{
    int count = 0;
    count = calc2_s();
    printf("Total amount of staff is: ");
    printf("%d\n", count);
    printf("Do you want to count staff in the specific department(y/n): ");
    
    char answer[1];
    scanf("%s", answer);
    if(answer[0] == 'y')
    {
        FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "rb");
        FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "rb");
        
        struct IndexRecord IndexRecord;
        struct Department department;
        
        fseek(recordTable, 0 , SEEK_END);
        long size2 = ftell(recordTable)/sizeof(struct Department);
        for(size_t i = 0; i < size2; i++)
        {
            fseek(indexTable, i * sizeof(struct IndexRecord), SEEK_SET);
            fread(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable);
            
            if(IndexRecord.exists)
            {
               fseek(recordTable, IndexRecord.record,  SEEK_SET);// move pointer to the found address
               fread(&department, sizeof(struct Department), 1, recordTable);// read structure department from binary file
               {
                   printf("Number of staff in department # %d is %d\n", department.department_id, department.quantity_staff);
               }
            }
            else{
                continue;
            }
        }
        fclose(recordTable);
        fclose(indexTable);
    }
    else {
        return;
    }
    return;
}
   
