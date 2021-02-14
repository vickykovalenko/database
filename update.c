#include <stdbool.h>
#include "update.h"
#include "structures.h"

int update_m(struct Department department)
{
   FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "r+b");
   FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "r+b");
    
    if (indexTable == NULL || recordTable == NULL)
    {
        printf("Department.ind or Department.fl are empty.\n");
        return 0;
    }

    struct IndexRecord indexRecord;
    int id = department.department_id;
    
    fseek(indexTable, 0, SEEK_END);
    long indexTableSize = ftell(indexTable);
    
    if (indexTableSize == 0 || id * sizeof(struct IndexRecord) > indexTableSize)
    {
        printf("No such ID in table.\n");
        return 0;
    }
    
    fseek(indexTable, (id - 1) * sizeof(struct IndexRecord), SEEK_SET);
    fread(&indexRecord, sizeof(struct IndexRecord), 1, indexTable);
    
    if (indexRecord.exists == 0)
    {
        printf("The record you're looking for has been removed.\n");
        return 0;
    }

    fseek(recordTable, indexRecord.record, SEEK_SET);                // Position at the address of the record in the database
    fwrite(&department, sizeof(struct Department), 1, recordTable);                // update the record
    fclose(indexTable);
    fclose(recordTable);

    return 1;
}

int update_s(struct Staff staff, int staff_id)
{
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "r+b");
    
    fseek(recordTable, staff.thisAddress, SEEK_SET);
    fwrite(&staff, sizeof(struct Staff), 1, recordTable);
    fclose(recordTable);
    
    return 1;
}





