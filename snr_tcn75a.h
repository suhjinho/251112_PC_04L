/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */
/* (재)경북차량용임베디드기술연구원(GIVET)                                  */
/* # 파일명 : snr_tcn75a.h                                                  */
/*                                                                          */
/* # 내역: 온도센서 TCN75AVUA713 관련 함수 정의                             */
/* 		1. 슬레이브 주소 (7bit) = 0x48                                      */
/*		2.                                                                  */
/* # Chip type :  PIC18F26K40, Device ID Revision = 0xa044                  */
/* # Core Clock frequency : HFINTOSC = 64MHz, Fosc = 16MHz                  */
/* # Environment : MPLAB X IDE v5.35,  Microchip XC8 v2.10 (C99)            */ 
/* # Encoding    : x-windows-949                                            */
/* # Modification History                                                   */
/*   -------------------------------------------------------------------    */
/*   No. Ver     Date        by who      Trouble No. (Change Request No.)   */
/*   -------------------------------------------------------------------    */
/*   01  0.0.1   2020/03/26  IC.Park                                        */   
/*   02                                                                     */
/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */

#ifndef TCN75A_H
#define TCN75A_H

/* ----------------------------------------------------------------------
 * <Include File>
 * --------------------------------------------------------------------- */
#include "i2c.h"

/* ----------------------------------------------------------------------
 * < 함수 관련 상수 정의 >
 * --------------------------------------------------------------------- */

/* Slave Device Address */
#define cTCN75A_SLAVE_ADD    	 (0x48)    /* Temperature Sensor TCN75 Slave Address */

/* Register Address 정의 */
#define cTCN75A_REG_TEMP_ADD     (0x00)    /* Temperature Register */
#define cTCN75A_REG_CONFIG_ADD   (0x01)    /* Configuration Register */
#define cTCN75A_REG_TEMP_HY_ADD  (0x02)    /* Temperature Hysteresis register */
#define cTCN75A_REG_TEMP_SV_ADD  (0x03)    /* Temperature Limit-set register */

/* Configuration Register Define */
/* bit[7] : ONE-SHOT bit : shutdown시 1회 변환, 0 = Disabled (Power-up default) */
#define cCONF_ONE_SHOT_ENABLE  (0b10000000)    /*  ONE-SHOT Enabled */

/* bit[6:5] : Resolution 정의
 * 00 = 9 bit or 0.5°C (Power-up default) */ 
#define cCONF_RSL_9BIT         (0b00000000)    /*    0.5℃,  40msec, (Power-up default) */
#define cCONF_RSL_10BIT        (0b00100000)    /*   0.25℃,  80msec */
#define cCONF_RSL_11BIT        (0b01000000)    /*  0.125℃, 160msec */
#define cCONF_RSL_12BIT        (0b01100000)    /* 0.0625℃, 320msec */

/* bit[4:3] : FAULT QUEUE : 설정 횟수만큼 오류 발생 후, ALERT 출력, 
 * 00 = 1 (Power-up default) */ 
#define cCONF_FLT_QUEUE_1      (0b00000000)
#define cCONF_FLT_QUEUE_2      (0b00001000)
#define cCONF_FLT_QUEUE_4      (0b00010000)
#define cCONF_FLT_QUEUE_6      (0b00011000)

/* bit[2] : ALERT 출력 극성, 0 = Active-low (Power-up default)*/ 
#define cCONF_ALTER_POL_HIGH   (0b00000100)

/* bit[1] : ALERT 출력 모드, 0 = Comparator mode (Power-up default) */ 
#define cCONF_ALTER_OUT_MODE_INT  (0b00000010)

/* bit[0] : SHUTDOWN 모드, 0 = Disable (Power-up default) */ 
#define cCONF_SHUT_DOWN_ENABLE    (0b00000001)

/* ----------------------------------------------------------------------
 * < 관련 메크로 정의 >
 * --------------------------------------------------------------------- */


/* ----------------------------------------------------------------------
 * < 함수 프로토타입 선언 >
 * --------------------------------------------------------------------- */
uint8_t TEMP_Initialize (void);
uint16_t TEMP_Read (void);
uint8_t TEMP_Calucation (int16_t rTemp);

#endif  /* *** End of Header File *** */

/* ************************************************************
*  End of snr_tcn75a.h
************************************************************ */