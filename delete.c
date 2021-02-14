#include "delete.h"
#include "input.h"
#include  <stdlib.h>

int del_m(int department_id)
{
    FILE* indexTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Department.ind", "r+b");     // read/update binary file
    if (indexTable == NULL)
    {
        printf("Database files do not exist.");
        return 0;
    }
    fseek(indexTable, 0, SEEK_END);
    long indexTableSize = ftell(indexTable);
    if (indexTableSize == 0 || department_id * sizeof(struct IndexRecord) > indexTableSize)
    {
        printf("No such ID in table.");
        return 0;
    }
    
    struct Department department;
    get_m(&department, department_id);
    
    struct IndexRecord IndexRecord;
    
    fseek(indexTable, (department_id - 1) * sizeof(struct IndexRecord), SEEK_SET);
    fread(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable);
    
    IndexRecord.exists = 0; // denote as a record that logically does not exist
    
    fseek(indexTable, (department_id - 1) * sizeof(struct IndexRecord), SEEK_SET);
    fwrite(&IndexRecord, sizeof(struct IndexRecord), 1, indexTable); //write the field exists = 0 back to the table
    fclose(indexTable);
    
    //enter the index of the deleted record in the garbage area
    FILE* garbageCollector = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_collector.txt", "rb");
    
    int garbageCount;
    fscanf(garbageCollector, "%d", &garbageCount);
    
    int* delIds = malloc(garbageCount * sizeof(int));//allocate the place where the garbage indexes will be stored
    
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageCollector, "%d", delIds + i);//fill the array
    }
    
    fclose(garbageCollector);
    garbageCollector = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_collector.txt", "wb");//clean the file with garbage
    
    fprintf(garbageCollector, "%d", garbageCount + 1); //add 1 to number of indices
    
    for (int i = 0; i < garbageCount; i++)
    {
        fprintf(garbageCollector, " %d", delIds[i]);
    }

    fprintf(garbageCollector, " %d", department_id);
    free(delIds);     //deallocating the memory
    fclose(garbageCollector);
    
    if(department.quantity_staff) //del-s for all slaves
    {
        FILE* staffTable = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_s.txt", "r+b");
        
        struct Staff staff;
        fseek(staffTable, department.firstStaffAddress, SEEK_SET);
        for (int i = 0; i < department.quantity_staff; i++)
        {
            fread(&staff, sizeof(struct Staff), 1, staffTable);
            fclose(staffTable);
            del_s(department, staff, staff.staff_id);
            staffTable = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_s.txt", "r+b");
            
            fseek(staffTable, staff.nextAddress, SEEK_SET);
        }
        fclose(staffTable);
        
    }
    return 1;

}

int del_s(struct Department department, struct Staff staff, int staff_id)
{
    FILE* recordTable = fopen("/Users/andrej/Documents/lab_db/lab_db/Staff.fl", "r+b");
    struct Staff staff2;
    
    fseek(recordTable, department.firstStaffAddress, SEEK_SET);
    do                                                            //  Looking for a recording predecessor (it may not be,
    {                                                             // then the record will be entered in the predecessor)
        fread(&staff2, sizeof(struct Staff), 1, recordTable);
        fseek(recordTable, staff2.nextAddress, SEEK_SET);
    }
    while(staff2.nextAddress != staff.thisAddress && staff.thisAddress != department.firstStaffAddress);
    
    
    if (staff.thisAddress == department.firstStaffAddress)        // No predecessor (first record)
    {
        if (staff.thisAddress == staff.nextAddress)            // No successor (only one record)
        {
            department.firstStaffAddress = -1;
        }
        else                                                //if there is a successor
        {
            department.firstStaffAddress = staff.nextAddress;
        }
    }
    else                                                    //if there is a predecessor
    {
        if(staff.thisAddress == staff.nextAddress)            // No successor (the last record)
        {
            staff2.nextAddress = staff2.thisAddress;    // make the predecessor last
        }
        else
        {
            staff2.nextAddress = staff.nextAddress;
        }

        fseek(recordTable, staff2.thisAddress, SEEK_SET);    // write updated predecessor
        fwrite(&staff2, sizeof(struct Staff), 1, recordTable);            // back to the table
    }
    
    
    FILE* garbageCollector = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_s.txt", "rb");
    
    int garbageCount;
    fscanf(garbageCollector, "%d", &garbageCount);

    long* delAddresses = malloc(garbageCount * sizeof(long));
    
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageCollector, "%ld", delAddresses + i);
    }
    
    fclose(garbageCollector);
    garbageCollector = fopen("/Users/andrej/Documents/lab_db/lab_db/garbage_s.txt", "wb");
    fprintf(garbageCollector, "%d", garbageCount + 1);
    
    for (int i = 0; i < garbageCount; i++)
    {
        fprintf(garbageCollector, " %ld", delAddresses[i]);        // enter "garbage addresses "back
    }
    
    fprintf(garbageCollector, "%ld", staff.thisAddress);
    free(delAddresses);                                        // free the memory allocated for the array
    fclose(garbageCollector);
    
    staff.exists = 0;
    
    fseek(recordTable, staff.thisAddress, SEEK_SET);                // update staff
    fwrite(&staff, sizeof(struct Staff), 1, recordTable);
    fclose(recordTable);
    
    department.quantity_staff--;
    
    update_m(department);
    return 1;

}

