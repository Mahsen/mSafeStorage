
/************************************************** Description *******************************************************/
/*
    File : main.c
    Programmer : Mohammad Lotfi
    Used : useing mms
    Design Pattern : Nothing
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2025/5/2
*/
/************************************************** Warnings **********************************************************/
/*
    Nothing
*/
/************************************************** Wizards ***********************************************************/
/*
    Nothing
*/
/************************************************** Includes **********************************************************/
#include "mss.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string.h>
/************************************************** Defineds **********************************************************/
/*
    Nothing
*/
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
FILE* file;
const char* filename = "memory.db";
/************************************************** Opjects ***********************************************************/
/*
    Nothing
*/
/************************************************** Functions *********************************************************/
void MAIN_Encrypt(unsigned char *Data, unsigned long Length) {
    for (unsigned long i=0; i < Length; i++)
        Data[i] += 10;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void MAIN_Decrypt(unsigned char *Data, unsigned long Length) {
    for (unsigned long i=0; i < Length; i++)
        Data[i] -= 10;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void MAIN_Open(void) {
    file = fopen(filename, "r+b");  // Open file for read/write in binary mode
    if (!file) {
        // If file doesn't exist, create it
        file = fopen(filename, "w+b");
        if (!file) {
            perror("Failed to open file");
            return;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void MAIN_Close(void) {
    fclose(file);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void MAIN_Write(unsigned long Offset, unsigned char *Data, unsigned long Length) {
    // Seek to the desired offset
    if (fseek(file, Offset, SEEK_SET) != 0) {
        perror("Failed to seek");
        return;
    }

    // Write 'length' bytes of 'data' to the file
    size_t written = fwrite(Data, 1, Length, file);
    if (written != Length) {
        perror("Failed to write all bytes");
        return;
    }

    // Flush the buffer to ensure data is written to the file immediately
    if (fflush(file) != 0) {
        perror("Failed to flush the file buffer");
        return;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void MAIN_Read(unsigned long Offset, unsigned char *Data, unsigned long Length) {
    // Seek to the desired offset
    if (fseek(file, Offset, SEEK_SET) != 0) {
        perror("Failed to seek");
        return;
    }

    // Read 'length' bytes into the buffer
    size_t read = fread(Data, 1, Length, file);
    if (read != Length) {
        if (feof(file)) {
            printf("End of file reached before expected length.\n");
        } else {
            perror("Failed to read the required bytes");
        }
        return;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int main() {

    std::cout << "Start." << std::endl;

    struct {
        std::string Name;
        int Voltage;
        int Current;
    } Power;
    std::cout << "Size Power : " << sizeof(Power) << std::endl;

    int err = MSS_Config_Storage(4,
        10000,
        MAIN_Encrypt,
        MAIN_Decrypt,
        MAIN_Open, 
        MAIN_Close, 
        MAIN_Write, 
        MAIN_Read);
    std::cout << "MSS_Config_Storage err : " << err << std::endl;

    err = MSS_Config_Group(&Power, sizeof(Power), MSS_MODE_WRITE_SAFE | MSS_MODE_READ_SAFE | MSS_MODE_BACKUP | MSS_MODE_ENCRRYPT, 0);
    std::cout << "MSS_Config_Group err : " << err << std::endl;

    Power.Name = "Meter_101";
    Power.Voltage = 220;
    Power.Current = 10;
    std::cout << "Power.Name = " << Power.Name << " - Power.Voltage = " << Power.Voltage << " - Power.Current = " << Power.Current << std::endl;

    err = MSS_Update_Group(&Power);
    std::cout << "MSS_Update_Group err : " << err << std::endl;

    err = MSS_Refresh_Group(&Power);
    std::cout << "MSS_Refresh_Group err : " << err << std::endl;
    
    std::cout << "Power.Name = " << Power.Name << " - Power.Voltage = " << Power.Voltage << " - Power.Current = " << Power.Current << std::endl;

    std::cout << "----------------------------------------------------------------" << std::endl;

    struct {
        int Code;
        int Value;
    } Event;

    err = MSS_Config_Group(&Event, sizeof(Event), MSS_MODE_RECORDS, 1000);
    std::cout << "MSS_Config_Group err : " << err << std::endl;

    // for (int i=0; i < 1000; i++) {
    //     Event.Code = 1000-i;
    //     Event.Value = i;
    //     std::cout << "Event.Code = " << Event.Code << " - Power.Value = " << Event.Value << std::endl;
    //     err = MSS_Update_Group_Record(&Event, i);
    //     std::cout << "MSS_Update_Group_Record err : " << err << std::endl;
    // }
    
    // for (unsigned long i=0; i < 1000; i++) {
    //     err = MSS_Refresh_Group_Record(&Event, i);
    //     std::cout << "MSS_Refresh_Group_Record err : " << err << std::endl;
    //     std::cout << "Event.Code = " << Event.Code << " - Power.Value = " << Event.Value << std::endl;
    // }

    return 0;
}
/************************************************** Tasks *************************************************************/
/*
    Nothing
*/
/************************************************** Vectors ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
