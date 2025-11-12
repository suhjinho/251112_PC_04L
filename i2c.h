/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */
/* (재)경북차량용임베디드기술연구원(GIVET)                                  */
/* # 파일명 : system.h                                                      */
/*                                                                          */
/* # 내역: I2C bus 관련 각종 함수                                           */
/* 		1. 관련 슬레이브 : TempSNR(TCN75AVUA713, Add7=0x48)                 */
/*		2. 참고 :  AN735 (DS00735A)                                         */
/* # Chip type :  PIC18F26K40, Device ID Revision = 0xa044                   */
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

#ifndef I2C_H
#define I2C_H
#include<stdint.h>
/* ----------------------------------------------------------------------
 * < 함수 관련 상수 정의 >
 * --------------------------------------------------------------------- */


/* ----------------------------------------------------------------------
 * < 관련 메크로 정의 >
 * --------------------------------------------------------------------- */
/* 클럭 설정 : Clock = Fosc/(4 * (SSP1ADD + 1))
 *            => SSP1ADD = ( ( (Fosc / Clock) / 4 ) - 1 )
 * TCN75AVUA713의 경우 Clock = 100KHz_max, Fosc = 16MHz의 경우 
 * SSP1ADD = (Fosc - 4 * Clock) / (4 * Clock) = (16MHz - 4 * 100KHz) / (4 * 100KHz) = 39 = 0x27 */
#define  cI2C_CLOCK_100KHz    39U  /* Clock = 100KHz, Fosc = 16MHz */  
#define  cI2C_CLOCK_400KHz     9U  /* Clock = 400KHz, Fosc = 16MHz */  

/* Read/Write Control */
#define cWRITE_MODE          0x00 
#define cREAD_MODE           0x01
#define cI2C_TIME_OUT        200  /* 최소 150 이상 사용 할것 */

/* SSP1CON2 플래그 설정 */
#define cCON2_FLAG_SEN         (1 << 0)   /* Start Condition Enable bit (Master mode only) */
#define cCON2_FLAG_RSEN        (1 << 1)   /* Repeated Start Condition Enable bit (Master mode only) */
#define cCON2_FLAG_PEN         (1 << 2)   /* Stop Condition Enable bit (Master mode only) */
#define cCON2_FLAG_RCEN        (1 << 3)   /* Receive Enable bit (Master Receive mode only) */
#define cCON2_FLAG_ACKEN       (1 << 4)   /* Acknowledge Sequence Enable bit */
#define cCON2_FLAG_ACKDT       (1 << 5)   /* Acknowledge Data bit (Master Receive mode only) */
#define cCON2_FLAG_ACKSTAT     (1 << 6)   /* Acknowledge Status bit (Master Transmit mode only) */

/* SSP1STAT 플래그 설정 */
#define cSTAT_FLAG_BF       (1 << 0)  /* Buffer Full Status */
#define cSTAT_FLAG_RW       (1 << 2)  /* Read/Write Information */

/* ----------------------------------------------------------------------
 * < 함수 프로토타입 선언 >
 * --------------------------------------------------------------------- */
/* I2C 버스 초기화 함수 */
    void I2C_Initialize (void);

/* 8bit Register 주소에 한 바이트 데이타 쓰기 */
    void I2C_WriteR8D8 (uint8_t slaveAddr, uint8_t regAddr, uint8_t data);  

/* 8bit Register 주소에서 한바이트 데이타 읽기 */     
    uint8_t I2C_ReadR8D8 (uint8_t slaveAddr, uint8_t regAddr);              

/* 8bit 레지스터 주소에 16bit 데이터 쓰기 */    
    void I2C_WriteR8D16 (uint8_t slaveAddr, uint8_t regAddr, uint16_t value); 

/* 8bit 레지스터 주소에 16bit 데이터 읽기 */    
    uint16_t I2C_ReadR8D16 (uint8_t slaveAddr, uint8_t regAddr);  

#endif  /* *** End of Header File *** */

/* ************************************************************
*  End of i2c.h
************************************************************ */