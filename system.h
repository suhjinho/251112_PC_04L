/* 
 * File:   system.h
 * Author: ??
 *
 * Created on May 19, 2020, 3:52 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H


// eeprom map

#define EEP_ADDR_MAGIC                      0
#define EEP_ADDR_CONFIG                     1
#define EEP_ADDR_THRESHOLD_H                2
#define EEP_ADDR_THRESHOLD_L                3
#define EEP_ADDR_THRE_MIN_H                 4
#define EEP_ADDR_THRE_MIN_L                 5
#define EEP_ADDR_THRE_MAX_H                 6
#define EEP_ADDR_THRE_MAX_L                 7
#define EEP_ADDR_OBSOLESCENCE_H             8
#define EEP_ADDR_OBSOLESCENCE_L             9
#define EEP_ADDR_REFERENCE_TEMP_H           10
#define EEP_ADDR_REFERENCE_TEMP_L           11
#define EEP_ADDR_REFERENCE_BASELINE_H       12
#define EEP_ADDR_REFERENCE_BASELINE_L       13
#define EEP_ADDR_RECOVERY_EN                14
#define EEP_ADDR_RECOVERY_LIMIT_H           15
#define EEP_ADDR_RECOVERY_LIMIT_L           16
#define EEP_ADDR_RECOVERY_CNT               17
#define EEP_ADDR_RECOVERY_BASELINE_H        18
#define EEP_ADDR_RECOVERY_BASELINE_L        19
#define EEP_ADDR_DETECT_TIME_H              20
#define EEP_ADDR_DETECT_TIME_L              21
#define EEP_ADDR_BASELINE_COR_H             22
#define EEP_ADDR_BASELINE_COR_L             23

#define AD_MIN  0
#define AD_MAX  1023


int eepReadWord(int addr);
void eepWriteWord(int addr, int data);

//

int splitCmd(char *src, int *param1, int *param2);
// return 
// -1 : not found
//

#define BIT_smr     0x01
#define BIT_smb     0x02
#define BIT_smd     0x04
#define BIT_davg    0x08
#define BIT_smt     0x10
#define BIT_smp     0x20
#define BIT_tim     0x40
#define BIT_tem     0x80
#define BIT_sma     0x100

#endif	/* SYSTEM_H */
