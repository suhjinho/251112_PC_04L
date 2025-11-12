/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */
/* (재)경북차량용임베디드기술연구원(GIVET)                                  */
/* # 파일명 : snr_tcn75a.c                                                  */
/*                                                                          */
/* # 내역: I2C bus 관련 각종 함수                                           */
/* 		1. 관련 슬레이브 : TempSNR(TCN75AVUA713, Add7=0x48)                 */
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

//#include "system.h"
//#include "uart.h"
#include "snr_tcn75a.h"

/************************************************************************
* 함수 : void TEMP_Initialize()
* -----------------------------------------------------------------------
* 설명: 온도센서 TCN75 초기화 파일
* 입력: 없음
* 반환: 없음
************************************************************************/

uint8_t TEMP_Initialize (void)
{
    uint8_t configData = 0, returnData = 0;
    
    
    I2C_Initialize();

    /* 9-bit data mode = 30 ms,
     * 12-bit data mode 240 ms (typical) => 500 msec 주기로 읽기 */
    configData = cCONF_RSL_12BIT   |  cCONF_FLT_QUEUE_6 ; 
    
    /* 온도센서에 Config Reg.에 설정값 쓰기 */
    I2C_WriteR8D8 (cTCN75A_SLAVE_ADD, cTCN75A_REG_CONFIG_ADD, configData);
    returnData = I2C_ReadR8D8 (cTCN75A_SLAVE_ADD, cTCN75A_REG_CONFIG_ADD);
    
    return returnData;
}

/* 온도값 연산  = 최종 출력 값이 0x198 (즉 408) * 0.0625℃ = 25.5℃,
 * 최종 출력 값이 2047 (즉, 약 128℃) 이상의 경우 음의 온도값 즉 보수를 취함 */

uint16_t TEMP_Read (void)
{
    uint16_t tmpData = 0;
    
    tmpData = I2C_ReadR8D16 (cTCN75A_SLAVE_ADD, cTCN75A_REG_TEMP_ADD) >> 4; /* Ref. DataSheet EQUATION 5-2 => Ta = Code * 2^(-4) */
    //stSnrResult_PV[esnSNR5_I2C_TEMP].MeaData = (stSnrResult_PV[esnSNR5_I2C_TEMP].MeaData + tmpData) >> 1;   /* 평균값 */
    return tmpData;
}

/* Celsius (°C) / Fahrenheit (°F)
 * Celsius to Fahrenheit:   (°C × 9 / 5 ) + 32 = °F
 * Fahrenheit to Celsius:   (°F ? 32) ×  5 / 9  = °C  */
uint8_t TEMP_Calucation (int16_t rTemp)
{
    const uint8_t realScale[16] = { 0,   /* 0x0 -> 0        -> 0 ℃ */
                                    1,   /* 0x1 -> 0.625 ℃ -> 1 ℃ */
                                    1,   /* 0x2 -> 1.25 ℃  -> 1 ℃ */
                                    2,   /* 0x3 -> 1.875 ℃ -> 2 ℃ */
                                    2,   /* 0x4 -> 2.25 ℃ -> 2 ℃ */
                                    3,   /* 0x5 -> 3.125 ℃ -> 3 ℃ */
                                    4,   /* 0x6 -> 3.75 ℃ -> 4 ℃ */
                                    4,   /* 0x7 -> 4.375 ℃ -> 4 ℃ */
                                    5,   /* 0x8 -> 5 ℃ -> 5 ℃ */
                                    6,   /* 0x9 -> 5.625 ℃ -> 6 ℃ */
                                    6,   /* 0xA -> 6.25 ℃ -> 6 ℃ */
                                    7,   /* 0xB -> 6.875 ℃ -> 7 ℃ */
                                    8,   /* 0xC -> 7.5 ℃ -> 8 ℃ */
                                    8,   /* 0xD -> 8.125 ℃ -> 8 ℃ */
                                    9,   /* 0xE -> 8.75 ℃ -> 9 ℃ */
                                    9   /* 0xF -> 9.375 ℃ -> 9 ℃ */
                                };
    
    int8_t tempInt = 0;
    uint8_t tempReal = 0, index = 0;
    int16_t tempData = 0;
    
    /* 영하의 온도 이면 */
    if(rTemp > 2047) 
            tempData = (rTemp - 4096);
    else    tempData = (int16_t)rTemp;

    /* 정수부와 실수부 온도값 분리 */
    tempInt = (int8_t)(tempData >> 4);        /* MSB 8bit, 정수부 온도 */
    tempReal = realScale[(uint8_t)(tempData & 0x000F)];   /* LSB 4bit */
    return tempInt;
}

/* ************************************************************
*  End of snr_tcn75a.c
************************************************************ */