#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structures.h"
#include "input.h"
#include "update.h"

int get_m(struct Department* department, int department_id)
{
    FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "rb");
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "rb");
    
    if (indexTable == NULL || recordTable == NULL)
    {
        printf("Department.ind or Department.fl do not exist.\n");
        return 0;
    }

    struct IndexRecord indexRecord;
    
    fseek(indexTable, 0, SEEK_END); // Moving pointer to the end
    long indexTableSize = ftell(indexTable); //size in bytes of indexTable
    
    if (indexTableSize == 0 || department_id * sizeof(struct IndexRecord) > indexTableSize)
    {
        printf("No such ID in table.\n");
        return 0;
    }
    
    fseek(indexTable, (department_id - 1) * sizeof(struct IndexRecord), SEEK_SET);    // get the indexer of the  record
    fread(&indexRecord, sizeof(struct IndexRecord), 1, indexTable);            // at the specified number Reads an array of count elements, each one with a size of size bytes, from the stream and stores them in the block of memory specified by ptr.
    
    if (!indexRecord.exists)
    {
        printf("The record you're looking for has been removed.\n");
        return 0;
    }

    fseek(recordTable, indexRecord.record,  SEEK_SET);// Move the pointer to the found address
    fread(department, sizeof(struct Department), 1, recordTable);// read structure department from binary file
 
    fclose(indexTable);
    fclose(recordTable);
    
    return 1;
}

int get_s(struct Department department, struct Staff* staff, int staff_id)
{
    if(department.quantity_staff == 0)
    {
        printf("Department has no staff.\n");
        return 0;
    }
    
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "rb");
    
    fseek(recordTable, department.firstStaffAddress, SEEK_SET); //use number of the first staff
    fread(staff, sizeof(struct Staff), 1, recordTable);
    
    for(int i = 0; i < department.quantity_staff; i++) //Consecutive search find the necessary record
    {   if(staff->staff_id == staff_id)
        {
            fclose(recordTable);
            return 1;
        }
        fseek(recordTable, staff->nextAddress, SEEK_SET);
        fread(staff, sizeof(struct Staff), 1, recordTable);
    }
    

    printf("There is no staff in this department.\n");
    fclose(recordTable);
    return 0;
}

void rewriteData(int garbageCount, FILE* garbageFile, struct Department* department)
{
    int* delIds = malloc(garbageCount * sizeof(int));  // garbage indexes
    
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageFile, "%d", delIds + i); //fill the array with garbage indexes
    }
    
    department->department_id = delIds[0]; //the last deleted id becomes an id of added department
    
    fclose(garbageFile);
    fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_collector.txt", "wb");           // clean garbage file
    fprintf(garbageFile, "%d", garbageCount - 1);
    
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageFile, " %d", delIds[i]);                // write rest of the rubbish indexes
    }
    
    free(delIds);                                            // free the memory allocated for the array
    fclose(garbageFile);
}

int insert_m(struct Department department)
{
    FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "a+b");
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "a+b"); // a - append binary Open file for output at the end of a file
    FILE* garbageCollector = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_collector.txt", "rb");  // "rb": r - read/update:
    if(garbageCollector == NULL)
    {
        printf("NULL garbage collector");
    }
    struct IndexRecord IndexRecord;
    int garbageCount;
    
    fscanf(garbageCollector, "%d", &garbageCount);
    
    if (garbageCount)                                        // there is garbage indexes rewrite first of them
    {
        rewriteData(garbageCount, garbageCollector, &department);
        
        fclose(indexTable);
        fclose(recordTable);
        
        indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "r+b");  // open binary files again and change mode for read and update
        recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.fl", "r+b");
        
        fseek(indexTable, (department.department_id - 1) * sizeof(struct IndexRecord), SEEK_SET);
        fread(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable);
        fseek(recordTable, IndexRecord.record, SEEK_SET);
    }
    else
    {
        long indexerSize =  sizeof(struct IndexRecord);
        fseek(indexTable, 0, SEEK_END);
        
        if (ftell(indexTable))                                // Size of indexTable is not null
        {
            fseek(indexTable, -indexerSize, SEEK_END);        // move pointer to the last indexer
            fread(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable);    // read last indexer
            
            department.department_id = IndexRecord.key + 1;                        // number the record with the following index
        }
        else                                                //indexTable is empty
        {
            department.department_id = 1;                               
        }
    }
    
    department.quantity_staff = 0;
    department.firstStaffAddress = -1;
    
    fwrite(&department, sizeof(struct Department), 1, recordTable);
    
    IndexRecord.key = department.department_id;
    IndexRecord.record = (department.department_id - 1) * sizeof(struct Department);
    IndexRecord.exists = 1;
    
    fseek(indexTable, (department.department_id - 1) * sizeof(struct IndexRecord), SEEK_SET);
    fwrite(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable);
    
    fclose(indexTable);
    fclose(recordTable);

    return 1;
}

void rewriteAddress(int garbageCount, FILE* garbageFile, struct Staff* staff)
{
    long* delIds = malloc(garbageCount * sizeof(long));  // garbage addresses
    
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageFile, "%ld", delIds + i); //fill garbage indexes in the array
    }
    
    staff->thisAddress = delIds[0];
    staff->nextAddress = delIds[0];  //staff is single, so pointer points to itself

    fclose(garbageFile);
    fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_s.txt", "wb"); //clean garbage file
    fprintf(garbageFile, "%d", garbageCount - 1); //new number of garbage addresses
    
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageFile, " %ld", delIds[i]);                //write down the rest of the "garbage" indexes
    }
    
    free(delIds);                                            // аree the memory allocated for the array
    fclose(garbageFile);
}

void linkAddresses(FILE* recordTable, struct Department department, struct Staff staff)
{
    fclose(recordTable);
    recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "r+b");  //  chanhe mode for read/update
    struct Staff previous;

    fseek(recordTable, department.firstStaffAddress, SEEK_SET);

    for (int i = 0; i < department.quantity_staff; i++)            // run the linked list to the last staff
    {
        fread(&previous, sizeof(struct Staff), 1, recordTable);
        fseek(recordTable, previous.nextAddress, SEEK_SET);
    }

    previous.nextAddress = staff.thisAddress;                // link addresses
    fwrite(&previous,  sizeof(struct Staff), 1, recordTable);                // write updated record back to the file
        
}

int insert_s(struct Department department, struct Staff staff)
{
    staff.exists = 1;
    
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "a+b");
    FILE* garbageZone = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_s.txt", "rb");
    
    int garbageCount; //the number of deleted indexes
    
    fscanf(garbageZone, "%d", &garbageCount);
    
    if(garbageCount)
    {
        rewriteAddress(garbageCount, garbageZone, &staff);
        fclose(recordTable);
        recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "r+b");
        fseek(recordTable, staff.thisAddress, SEEK_SET);    // Move pointer to the rubbish for next rewrite
                                                            
    }
   else
   {
       fseek(recordTable, 0, SEEK_END);
       long dbSize = ftell(recordTable);
       staff.thisAddress = dbSize;
       staff.nextAddress = dbSize;
   }
    
    fwrite(&staff, sizeof(struct Staff), 1, recordTable); //write staff to the file
    
    if (!department.quantity_staff)                                    // no staff yet
    {
        department.firstStaffAddress = staff.thisAddress;
    }
    else                                                        // Department has already staff, update last staff
    {
        linkAddresses(recordTable, department, staff);
    }
    fclose(recordTable);                                            // Close file
    department.quantity_staff++;

    update_m(department);
    return 1;
}

void inputStaff(struct Staff* staff)
{
    char staff_name[15];
    printf("Enter staff's name: ");
    scanf("%s", staff_name);
    strcpy(staff->staff_name, staff_name);
    
    char staff_surname[15];
    printf("Enter staff's surname: ");
    scanf("%s", staff_surname);
    strcpy(staff->staff_surname, staff_surname);

    double staff_salary;
    printf("Enter staff's salary: ");
    scanf("%lf", &staff_salary);
    staff->staff_salary = staff_salary;
    
    char staff_email[25];
    printf("Enter staff's email: ");
    scanf("%s", staff_email);
    strcpy(staff->staff_email, staff_email);
    
    char staff_login[25];
    printf("Enter staff's login: ");
    scanf("%s", staff_login);
    strcpy(staff->staff_login, staff_login);
    
    char staff_password[25];
    printf("Enter staff's password: ");
    scanf("%s", staff_password);
    strcpy(staff->staff_password, staff_password);
    
    char stream[15];
    printf("Enter staff's stream: ");
    scanf("%s", stream);
    strcpy(staff->stream, stream);
    
}

