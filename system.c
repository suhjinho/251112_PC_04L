#include<string.h>
#include<stdlib.h>
#include "mcc_generated_files/mcc.h"

#define CMD_CNT     40

char cmdTable[CMD_CNT][4] = 
{
    "smr",  // 0
    "smb",  // 1
    "smd",
    "smt",
    "smp",
    "sma",  // 5
    "srr",
    "srb",
    "srd",
    "mts",
    "srp",  // 10
    "snr",
    "sxr",
    "adr",
    "adm",
    "thr",  // 15
    "sor",
    "sow",  // set oldering
    "eer",
    "eew",
    "eed",  // 20
    "eel",      // eepload
    "ees",      // eepsave
    "thw",       // threshold write
    "ren",      // recovery enable/disable
    "rer",  // 25 : read recovery
    "ter",
    "tem",
    "snw",
    "sxw",
    "rew",   // 30 : set recovery
    "atc",   // 31 : auto calibration
    "sfb",   // 32 : set proximity baseline
    "wrs",   // 33 : worn test start
    "wri",   // 34 : worn test init
    "ssb",   // 35 : setting reference
    "rdt",   // 36 : read detect time
    "sdt",   // 37 : setting detect time
    "rbc",   // 38 : read baseline cor
    "sbc"    // 39 : setting baseline cor
};

int strCmp(char *src, char *find)
{
    int i;
    
    for(i=0;i<3;i++)
    {
        if(src[i] != find[i]) return 0;
    }
    return 1;
}

int chkCmd(char *buffer)
{
    int i;

    for(i = 0; i<CMD_CNT;i++)
    {
        if(strCmp(buffer, cmdTable[i])) return i;
    }
    return -1;
}

int splitCmd(char *src, int *param1, int *param2)
{
    int ret = -1, i;
    char *cmd, *p1, *p2;
    
    cmd = strtok(src, " "); // skip space
    ret = chkCmd(cmd);
    
    if(ret < 0) return ret;
    
    p1 = strtok(NULL, " ");
    p2 = strtok(NULL, " ");
    for(i=0;src[i] != '\0';i++)
    {
        if(src[i] == ' ') src[i] = '\0';
    }
    (*param1) = atoi(p1);
    (*param2) = atoi(p2);
    return ret;
}

int eepReadWord(int addr)
{
    int tmp;
    
    tmp = DATAEE_ReadByte(addr * 2 + 1);
    tmp <<= 8;
    tmp |= (0xff & DATAEE_ReadByte(addr * 2));
    
    return tmp;
}

void eepWriteWord(int addr, int data)
{
    DATAEE_WriteByte(addr * 2, (uint8_t)data);
    DATAEE_WriteByte(addr * 2 + 1, (uint8_t)(data >> 8));
}
