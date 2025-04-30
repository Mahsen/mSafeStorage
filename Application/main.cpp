
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
    Last Update : 2025/4/30
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
        int Voltage;
        int Current;
    } Power;

    int err = MSS_Storage_Config(1,
        10000,
        0,
        0,
        MAIN_Open, 
        MAIN_Close, 
        MAIN_Write, 
        MAIN_Read);
    std::cout << "MSS_Storage_Config err : " << err << std::endl;

    err = MSS_Group_Config(&Power, sizeof(Power), MSS_MODE_DEFAULT, 0);
    std::cout << "MSS_Group_Config err : " << err << std::endl;

    Power.Voltage = 222;
    Power.Current = 10;
    std::cout << "Power.Voltage = " << Power.Voltage << " - Power.Current = " << Power.Current << std::endl;

    //err = MSS_Group_Update(&Power);
    //std::cout << "MSS_Group_Update err : " << err << std::endl;

    err = MSS_Group_Refresh(&Power);
    std::cout << "MSS_Group_Refresh err : " << err << std::endl;

    std::cout << "Power.Voltage = " << Power.Voltage << " - Power.Current = " << Power.Current << std::endl;

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
