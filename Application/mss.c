
/************************************************** Description *******************************************************/
/*
    File : mss.c
    Programmer : Mohammad Lotfi
    Used : manage safe storge
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
/************************************************** Defineds **********************************************************/
/*
    Nothing
*/
/************************************************** Names *************************************************************/
/*
    Nothing
*/
/************************************************** Variables *********************************************************/
volatile unsigned int MSS_Storage_Time_Open_Remind;

unsigned long MSS_Storage_Size;
void (*MSS_Storage_Encrypt)(unsigned char *Data, unsigned long Length);
void (*MSS_Storage_Decrypt)(unsigned char *Data, unsigned long Length);
void (*MSS_Storage_Open)(void);
void (*MSS_Storage_Close)(void);
void (*MSS_Storage_Write)(unsigned long Offset, unsigned char *Data, unsigned long Length);
void (*MSS_Storage_Read)(unsigned long Offset, unsigned char *Data, unsigned long Length);

struct {
    unsigned int Version;
    unsigned char *Key;
    struct {
        void* Name;
        unsigned long Offset;
        unsigned long Length;
        unsigned char Mode;
        unsigned long Time_Secound;
        unsigned long CRC;
    } Group[MSS_STORAGE_GROUP_SIZE];
} MSS_Storage_Map;

/* Table of CRC values for high–order byte  */
const unsigned char MSS_auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
} ;
    /* Table of CRC values for low–order byte  */
const unsigned char MSS_auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
} ;
/************************************************** Opjects ***********************************************************/
/*
    Nothing
*/
/************************************************** Functions *********************************************************/
unsigned int MSS_GetCRC(unsigned char *Data, unsigned long Length) {

	unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
	unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
	unsigned uIndex ; /* will index into CRC lookup table */

    /* pass through message buffer */
	while (Length--) {
		uIndex = uchCRCHi ^ *Data++ ; /* calculate the CRC */
		uchCRCHi = uchCRCLo ^ MSS_auchCRCHi[uIndex];
		uchCRCLo = MSS_auchCRCLo[uIndex];
	}

	return (uchCRCHi << 8 | uchCRCLo) ;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void MSS_Storage_Prepere(void) {

    MSS_Storage_Time_Open_Remind = MSS_STORAGE_TIME_OPEN;
    if(MSS_Storage_Open)
        MSS_Storage_Open();
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Storage_Map_Config(unsigned int Version) {

    MSS_Storage_Prepere();
    MSS_Storage_Read(0, (unsigned char *)&MSS_Storage_Map, sizeof(MSS_Storage_Map));
    if(MSS_Storage_Map.Version != Version) {
        for (int i = 0; i < MSS_STORAGE_GROUP_SIZE; i++) {
            MSS_Storage_Map.Group[i].Name = 0;            // Set Name to NULL
            MSS_Storage_Map.Group[i].Offset = 0;          // Default Offset
            MSS_Storage_Map.Group[i].Length = 0;          // Default Length
        }
        MSS_Storage_Map.Version = Version;
        MSS_Storage_Write(0, (unsigned char *)&MSS_Storage_Map, sizeof(MSS_Storage_Map));
    }

    return MSS_SUCCEED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Storage_Config(unsigned int Version,
                        unsigned long Size,
                        void (*Encrypt)(unsigned char *Data, unsigned long Length),
                        void (*Decrypt)(unsigned char *Data, unsigned long Length),
                        void (*Open)(void), 
                        void (*Close)(void), 
                        void (*Write)(unsigned long Offset, unsigned char *Data, unsigned long Length), 
                        void (*Read)(unsigned long Offset, unsigned char *Data, unsigned long Length)) {
    
    if((!Version) || (!Size) || (!Write) || (!Read))
        return MSS_INVALID_PARAMETER;

    MSS_Storage_Time_Open_Remind = 0;
    MSS_Storage_Size = Size;
    MSS_Storage_Encrypt = Encrypt;
    MSS_Storage_Decrypt = Decrypt;
    MSS_Storage_Open = Open;
    MSS_Storage_Close = Close;
    MSS_Storage_Write = Write;
    MSS_Storage_Read = Read;

    return MSS_Storage_Map_Config(Version);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Group_Config(void* Name, unsigned long Length, unsigned char Mode, unsigned int Time_Secound) {

    if((!Name) || (!Length))
        return MSS_INVALID_PARAMETER;
    else if(Mode && (!((Mode & MSS_MODE_WRITE_SAFE) || (Mode & MSS_MODE_READ_SAFE) || (Mode & MSS_MODE_ENCRRYPT) || (Mode & MSS_MODE_UPDATE_WITH_CHANGE) || (Mode & MSS_MODE_UPDATE_WITH_TIME))))
        return MSS_INVALID_MODE;
    else if((Mode & MSS_MODE_ENCRRYPT) && (Length%MSS_VALID_ENCRYPT_LENGTH))
        return MSS_INVALID_ENCRYPT_LENGTH;
    else if((Mode & MSS_MODE_WRITE_SAFE) && (Length%MSS_VALID_CHECK_LENGTH))
        return MSS_INVALID_CHECK_LENGTH;
    else if((Mode & MSS_MODE_READ_SAFE) && (Length%MSS_VALID_CHECK_LENGTH))
        return MSS_INVALID_CHECK_LENGTH;
    else if((Mode & MSS_MODE_ENCRRYPT) && ((!MSS_Storage_Encrypt) || (!MSS_Storage_Decrypt)))
        return MSS_INVALID_ENCRYPT_FUNC;
    else if((Mode & MSS_MODE_UPDATE_WITH_TIME) && (!Time_Secound))
        return MSS_INVALID_TIME;

    unsigned long Offset = sizeof(MSS_Storage_Map);

    int i = 0;
    for (; i < MSS_STORAGE_GROUP_SIZE; i++) {
        if (Name == MSS_Storage_Map.Group[i].Name)
            return MSS_SUCCEED;
        if (!MSS_Storage_Map.Group[i].Name)
            break;
        Offset += MSS_Storage_Map.Group[i].Length;
    }

    if(i == MSS_STORAGE_GROUP_SIZE)
        return MSS_GROUP_FULL;
    else if((MSS_Storage_Map.Group[i].Offset + Length) > MSS_Storage_Size)
        return MSS_STORAGE_FULL;

    MSS_Storage_Map.Group[i].Name = Name;
    MSS_Storage_Map.Group[i].Offset = Offset;
    MSS_Storage_Map.Group[i].Length = Length;
    MSS_Storage_Map.Group[i].Mode = Mode;
    MSS_Storage_Map.Group[i].Time_Secound = Time_Secound;
    MSS_Storage_Map.Group[i].CRC = 0;

    MSS_Storage_Prepere();
    MSS_Storage_Write(0, (unsigned char *)&MSS_Storage_Map, sizeof(MSS_Storage_Map));

    return MSS_SUCCEED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Group_Update(void* Name) {

    unsigned long CRC, CRC_Feed, Div;
    unsigned char Data_Feed[MSS_VALID_CHECK_LENGTH];

    if(!Name)
        return MSS_INVALID_PARAMETER;

    int i = 0;
    for (; i < MSS_STORAGE_GROUP_SIZE; i++)
        if (Name == MSS_Storage_Map.Group[i].Name)
            break;

    if(i == MSS_STORAGE_GROUP_SIZE)
        return MSS_GROUP_NOTFIND;

    Div = MSS_Storage_Map.Group[i].Length / MSS_VALID_CHECK_LENGTH;

    if(MSS_Storage_Map.Group[i].Mode & MSS_MODE_ENCRRYPT)
        MSS_Storage_Encrypt((unsigned char *)MSS_Storage_Map.Group[i].Name, MSS_Storage_Map.Group[i].Length);
    if((MSS_Storage_Map.Group[i].Mode & MSS_MODE_WRITE_SAFE) || (MSS_Storage_Map.Group[i].Mode & MSS_MODE_READ_SAFE)) {
        CRC = 0;
        for (unsigned long a = 0; a<Div; a++) 
            CRC += MSS_GetCRC(&((unsigned char *)MSS_Storage_Map.Group[i].Name)[(a*MSS_VALID_CHECK_LENGTH)], MSS_VALID_CHECK_LENGTH);
        MSS_Storage_Map.Group[i].CRC = CRC;
    }
    MSS_Storage_Prepere();
    MSS_Storage_Write(MSS_Storage_Map.Group[i].Offset, (unsigned char *)MSS_Storage_Map.Group[i].Name, MSS_Storage_Map.Group[i].Length);
    if(MSS_Storage_Map.Group[i].Mode & MSS_MODE_WRITE_SAFE) {
        CRC_Feed = 0;
        for (unsigned long a = 0; a<Div; a++) {
            MSS_Storage_Prepere();
            MSS_Storage_Read((MSS_Storage_Map.Group[i].Offset+(a*MSS_VALID_CHECK_LENGTH)), Data_Feed, MSS_VALID_CHECK_LENGTH);
            CRC_Feed += MSS_GetCRC(Data_Feed, MSS_VALID_CHECK_LENGTH);
        }
        if(CRC != CRC_Feed) 
            return MSS_WRITE_WRONG;
    }
    
    return MSS_SUCCEED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Group_Refresh(void* Name) {

    unsigned long Div, CRC_Feed;

    if(!Name)
        return MSS_INVALID_PARAMETER;

    int i = 0;
    for (; i < MSS_STORAGE_GROUP_SIZE; i++)
        if (Name == MSS_Storage_Map.Group[i].Name)
            break;

    if(i == MSS_STORAGE_GROUP_SIZE)
        return MSS_GROUP_NOTFIND;

    Div = MSS_Storage_Map.Group[i].Length / MSS_VALID_CHECK_LENGTH;

    MSS_Storage_Prepere();
    MSS_Storage_Read(MSS_Storage_Map.Group[i].Offset, (unsigned char *)MSS_Storage_Map.Group[i].Name, MSS_Storage_Map.Group[i].Length);
    if(MSS_Storage_Map.Group[i].Mode & MSS_MODE_READ_SAFE) {
        CRC_Feed = 0;
        for (unsigned long a = 0; a<Div; a++) 
            CRC_Feed += MSS_GetCRC(&((unsigned char *)MSS_Storage_Map.Group[i].Name)[(a*MSS_VALID_CHECK_LENGTH)], MSS_VALID_CHECK_LENGTH);
        if(MSS_Storage_Map.Group[i].CRC != CRC_Feed)
            return MSS_READ_WRONG;
    }
    if(MSS_Storage_Map.Group[i].Mode & MSS_MODE_ENCRRYPT)
        MSS_Storage_Decrypt((unsigned char *)MSS_Storage_Map.Group[i].Name, MSS_Storage_Map.Group[i].Length);

    return MSS_SUCCEED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Group_Sub_Update(void* Sub, unsigned long Sub_Length) {

    if((!Sub) || (!Sub_Length))
        return MSS_INVALID_PARAMETER;

    unsigned long Offset_Sub;

    int i = 0;
    for (; i < MSS_STORAGE_GROUP_SIZE; i++) {
        Offset_Sub = ((unsigned long)MSS_Storage_Map.Group[i].Name) - ((unsigned long)Sub);
        if ((Sub >= MSS_Storage_Map.Group[i].Name) && ((Offset_Sub + Sub_Length) <= MSS_Storage_Map.Group[i].Length))
            break;
    }

    if(i == MSS_STORAGE_GROUP_SIZE)
        return MSS_GROUP_NOTFIND;

    MSS_Storage_Prepere();
    MSS_Storage_Write(MSS_Storage_Map.Group[i].Offset + Offset_Sub, (unsigned char *)Sub, Sub_Length);

    return MSS_SUCCEED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int MSS_Group_Sub_Refresh(void* Sub, unsigned long Sub_Length) {

    if((!Sub) || (!Sub_Length))
        return MSS_INVALID_PARAMETER;

    unsigned long Offset_Sub;

    int i = 0;
    for (; i < MSS_STORAGE_GROUP_SIZE; i++) {
        Offset_Sub = ((unsigned long)MSS_Storage_Map.Group[i].Name) - ((unsigned long)Sub);
        if ((Sub >= MSS_Storage_Map.Group[i].Name) && ((Offset_Sub + Sub_Length) <= MSS_Storage_Map.Group[i].Length))
            break;
    }

    if(i == MSS_STORAGE_GROUP_SIZE)
        return MSS_GROUP_NOTFIND;
    
    MSS_Storage_Prepere();
    MSS_Storage_Read(MSS_Storage_Map.Group[i].Offset + Offset_Sub, (unsigned char *)Sub, Sub_Length);

    return MSS_SUCCEED;
}
/************************************************** Tasks *************************************************************/
/*
    Nothing
*/
/************************************************** Vectors ***********************************************************/
void MSS_Refresh_Every_Second(void) {
    if(MSS_Storage_Time_Open_Remind) {
        MSS_Storage_Time_Open_Remind--;
        if(!MSS_Storage_Time_Open_Remind)
            if(MSS_Storage_Close)
                MSS_Storage_Close();
    }
}
/**********************************************************************************************************************/
