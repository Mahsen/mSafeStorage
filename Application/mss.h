#ifndef __BILLING_H
#define __BILLING_H
//----------------------------------------------------------
#define MSS_STORAGE_GROUP_SIZE               4

#define MSS_STORAGE_TIME_OPEN                5

#define MSS_VALID_ENCRYPT_LENGTH            16
#define MSS_VALID_CHECK_LENGTH              16

#define MSS_SUCCEED                          0
#define MSS_INVALID_PARAMETER                -1
#define MSS_STORAGE_TOO_SMALL                -2
#define MSS_GROUP_FULL                       -3
#define MSS_GROUP_NOTFIND                    -4
#define MSS_STORAGE_FULL                     -5
#define MSS_INVALID_ENCRYPT_LENGTH           -6
#define MSS_INVALID_TIME                     -7
#define MSS_INVALID_MODE                     -8
#define MSS_INVALID_ENCRYPT_FUNC             -9
#define MSS_WRITE_WRONG                      -10
#define MSS_INVALID_CHECK_LENGTH             -11
#define MSS_READ_WRONG                       -12
#define MSS_WRITE_BACKUP_WRONG               -13
#define MSS_READ_BACKUP_WRONG                -14

#define MSS_MODE_DEFAULT                     (1 << 0)
#define MSS_MODE_WRITE_SAFE                  (1 << 1)
#define MSS_MODE_READ_SAFE                   (1 << 2)
#define MSS_MODE_BACKUP                      (1 << 3)
#define MSS_MODE_ENCRRYPT                    (1 << 4)
#define MSS_MODE_UPDATE_WITH_CHANGE          (1 << 5)
#define MSS_MODE_UPDATE_WITH_TIME            (1 << 6)
//----------------------------------------------------------
void MSS_Refresh_Every_Second(void);
int MSS_Storage_Config(unsigned int Version,
    unsigned long Size,
    void (*Encrypt)(unsigned char *Data, unsigned long Length),
    void (*Decrypt)(unsigned char *Data, unsigned long Length),
    void (*Open)(void), 
    void (*Close)(void), 
    void (*Write)(unsigned long Offset, unsigned char *Data, unsigned long Length), 
    void (*Read)(unsigned long Offset, unsigned char *Data, unsigned long Length));
int MSS_Group_Config(void* Name, unsigned long Length, unsigned char Mode, unsigned int Time_Secound);
int MSS_Group_Update(void* Name);
int MSS_Group_Refresh(void* Name);
int MSS_Group_Sub_Update(void* Sub, unsigned long Sub_Length);
int MSS_Group_Sub_Refresh(void* Sub, unsigned long Sub_Length);
//----------------------------------------------------------
#endif
