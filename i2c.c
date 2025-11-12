/*  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/  */
/* (재)경북차량용임베디드기술연구원(GIVET)                                   */
/* # 파일명 : i2c.c                                                         */
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

//#include "system.h"
//#include "uart.h"
#include <xc.h>
#include<stdio.h>
#include "i2c.h"


/************************************************************************
* 함수 : void I2C_Initialize()
* -----------------------------------------------------------------------
* 설명: i2c is enabled in the Master Serial Synchronous Port
* 입력: 없음
* 반환: 없음
************************************************************************/

void I2C_Initialize (void)
{
    /* 참고 : I2C 사용시 해당 핀은 반드시 디지탈 입력핀으로 설정 할 것 */
    /* Step 1 : MSSP(Master Synchronous Serial Port Module) Control Register 1 */
    SSP1CON1 = 0;          /* 일단 클리어 */
#if(1)
    SSP1CON1 = 0x08;
#else
    SSP1CON1bits.WCOL = 0;         /* bit [7] : Write Collision Detect bit, (1) collision / (0) normal
                                    * must be cleared by software */
    SSP1CON1bits.SSPOV = 0;        /* bit [6] : Receive Overflow Indicator, (1) overflow / (0) normal
                                    * must be cleared by software */
    SSP1CON1bits.CKP = 0;          /* bit [4] : SCK Release Control, (1) High / (0) Low */

    SSP1CON1bits.SSPM = 0b1000;    /* bit [3:0] Master Synchronous Serial Port Mode Select
                                    * 0b1000 = I2C Master mode: Clock = FOSC/(4 * (SSP1ADD + 1)) */
#endif
    
    /* Step 2 : MSSP(Master Synchronous Serial Port Module) Control Register 2
     *          Control Register for I2C Operation Only 
     *          프로그램상 흐름 제어용으로 사용, 일단 클리어   */
#if(1)
    SSP1CON2 = 0x00;   /* I2C BIT RATE SETUP */
#else    
    SSP1CON2bits.GCEN = 0;      /* bit [7] : General Call Enable bit (Slave mode only) */
    SSP1CON2bits.ACKSTAT = 0;   /* bit [6] : Acknowledge Status bit (Master Transmit mode only)
                                 *  1 = Acknowledge was not received from slave
                                 *  0 = Acknowledge was received from slave  */
    SSP1CON2bits.ACKDT = 0;     /* bit [5] : Acknowledge Data bit (Master Receive mode only)
                                 *  1 = Not Acknowledge
                                 *  0 = Acknowledge */   
    SSP1CON2bits.ACKEN = 0;     /* bit [4] : Acknowledge Sequence Enable bit
                                 *  1 = Initiates Acknowledge sequence on SDA1 and SCL1 pins and transmits ACKDT data bit
                                 *  0 = Acknowledge sequence is Idle */ 

    SSP1CON2bits.RCEN = 0;      /* bit [3] : Receive Enable bit (Master Receive mode only)
                                 *  1 = Enables Receive mode for I2C
                                 *  0 = Receive is Idle */     
    SSP1CON2bits.PEN = 0;       /* bit [2] : Stop Condition Enable bit (Master mode only)
                                 *  1 = Initiates Stop condition on SDA1 and SCL1 pins
                                 *  0 = Stop condition is Idle */ 
    SSP1CON2bits.RSEN = 0;      /* bit [1] : Repeated Start Condition Enable bit (Master mode only)
                                 *  1 = Initiates Repeated Start condition on SDA1 and SCL1 pins
                                 *  0 = Repeated Start condition is Idle */     
    SSP1CON2bits.SEN = 0;       /* bit [0] : Start Condition Enable bit (Master mode only)
                                 *  1 = Initiates Start condition on SDA1 and SCL1 pins
                                 *  0 = Start condition is Idle */     
#endif    
    
    
    /* Step 3 : MSSP(Master Synchronous Serial Port Module) Control Register 3 
     * Hold Time = 300ns */
    SSP1CON3 = 0;          /* 일단 클리어 */
#if(1)
    SSP1CON3 = 0x08;
#else 
    SSP1CON3bits.ACKTIM = 0;    /* bit [7] : Acknowledge Time Status bit, Unused in Master mode */
    SSP1CON3bits.PCIE = 0;      /* bit [6] : Stop Condition Interrupt Enable bit
                                 *  1 = Enable interrupt on detection of Stop condition
                                 *  0 = Stop detection interrupts are disabled */    
    SSP1CON3bits.SCIE = 0;      /* bit [5] : Start Condition Interrupt Enable bit
                                 *  1 = Enable interrupt on detection of Start condition
                                 *  0 = Start detection interrupts are disabled */    
    SSP1CON3bits.BOEN = 0;      /* bit [4] : Buffer Overwrite Enable bit
                                 *  1 = SSP1BUF is updated every time a new data byte is available
                                 *  0 = SSP1BUF is only updated when SSPOV is clear */  
   
    SSP1CON3bits.SDAHT = 1;     /* bit [3] : SDA Hold Time Selection bit
                                 *  1 = Minimum of 300ns hold time on SDA after the falling edge of SCL
                                 *  0 = Minimum of 100ns hold time on SDA after the falling edge of SCL */
    SSP1CON3bits.SBCDE = 0;     /* bit [2] : Slave Mode Bus Collision Detect Enable bit, Unused in Master mode */
    SSP1CON3bits.AHEN = 0;      /* bit [1] : Address Hold Enable bit, Unused in Master mode */
    SSP1CON3bits.DHEN = 0;      /* bit [0] : Data Hold Enable bit, Unused in Master mode */
#endif

    /* Step 4 : SSP1ADD(MSSP Baud Rate Divider and Address) Register, 설정범위 : 3 ~ 255, 3이하 사용 금지
     * 적용식 : Clock = FOSC/(4 * (SSP1ADD + 1))
     * TCN75AVUA713의 경우 Clock = 100KHz_max, Fosc = 16MHz의 경우 
     * SSP1ADD = (Fosc - 4 * Clock) / (4 * Clock) = (16MHz - 4 * 100KHz) / (4 * 100KHz) = 39 = 0x27 */
    
    SSP1ADD = cI2C_CLOCK_100KHz; 

    /* <Step 5> SSP1STAT(MSSP Status Register)
     * Standard Speed mode로 동작 */
    SSP1STAT = 0;              /* 일단 클리어 */
    SSP1STATbits.SMP = 1;       /* bit [7] : Slew Rate Control bit
                                 *  1 = Slew rate control is disabled for Standard Speed mode (100 kHz and 1 MHz)
                                 *  0 = Slew rate control is enabled for High-Speed mode (400 kHz) */

    /* Step 6 : MSSP Data Buffer Register 클리어 */  
    SSP1BUF = 0x00;
    
    /* Step 7 : I2C 모듈 활성화 */  
    SSP1CON1bits.SSPEN = 1; /* bit [5] : Master Synchronous Serial Port Enable, (1) Enable / (0) Disable */
}

/* ---------------------------------------------------------------------------------
 * 내부 사용 함수
 * --------------------------------------------------------------------------------- */
/* 버스가 아이들 상태가 될때까지 대기 */
void I2C_Wait(void)
{
    while ((SSP1STAT & cSTAT_FLAG_RW) || (SSP1CON2 & 0x1F)) continue;
}

/* I2C 동작 개시를 슬레이브에게 통보 */
void I2C_Start(void)
{
    SSP1CON2bits.SEN = 1;		                /* Generate Start COndition */
    
  	while (SSP1CON2 & cCON2_FLAG_SEN) continue;	/* Wait for Start condition Idle */
}

/* 읽기 동작시 읽기 모드용 슬래이브 어드레스 전송시 사용 */
void I2C_Restart(void)
{
    SSP1CON2bits.RSEN = 1;		/* Generate Restart	*/
    /* Wait for Repeated Start condition is Idle */	
	while (SSP1CON2 & cCON2_FLAG_RSEN) continue;	
}

/* 읽기/쓰기 동작 완료 */
void I2C_Stop(void)
{
	SSP1CON2bits.PEN = 1;		/* Generate Stop Condition */
	while (SSP1CON2 & cCON2_FLAG_PEN);	/* Wait for Stop condition is Idle */
}

/* NOT ACKNOWLEDGE EVENT : 연속 데이터 읽기 동작에서 최종 데이터 수신을 슬레이브에게 통보 */
void I2C_Nack(void)
{
	SSP1CON2bits.ACKDT = 1;			/* Set for NotACk */
	SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2 & cCON2_FLAG_ACKEN); /* wait for ACK to complete */
	SSP1CON2bits.ACKDT = 0;			/* Clear for NotACk */
}

/* ACKNOWLEDGE EVENT : 연속 데이터 읽기 동작에서 한 바이트 데이터 수신 완료시 슬레이브에게 Ack 신호를
   전송하여 다음 데이터 수신 준비 됨을 통보 */
void I2C_Ack(void)
{
    SSP1CON2bits.ACKDT = 0;      /* Send ACK */
    SSP1CON2bits.ACKEN = 1;      /* Initiate Acknowledge and transmit ACKDT */
    while(SSP1CON2 & cCON2_FLAG_ACKEN);
}

/* 슬레이브로 부터 수신된 데이터 획득 */
void I2C_Read(void)
{
    SSP1CON2bits.RCEN = 1;
    NOP();
    while(SSP1CON2 & cCON2_FLAG_RCEN);
}

/* 슬레이브에게 어드레스 또는 데이터를 전송 */
void I2C_Write(uint8_t data)
{
    SSP1BUF = (uint16_t)data;          /* I2C1  전송버퍼에 데이타 격납 */
    while(SSP1STAT & cSTAT_FLAG_BF);   /* 데이타 전송 완료 까지 대기 */
}

/* ---------------------------------------------------------------------------------
 * 전역 함수
 * --------------------------------------------------------------------------------- */

/* 8bit Register 주소에 한 바이트 데이타 쓰기 */
void I2C_WriteR8D8 (uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
    I2C_Wait(); 
    I2C_Start();
    I2C_Write((slaveAddr << 1) & 0xFE);  /* Slave 주소 전송 */
    I2C_Wait();

    I2C_Write(regAddr);             /* 레지스터 주소 전송 */
    I2C_Wait();                     /* ACK 대기 */

    I2C_Write(data);                /* 쓰기할 데이타 전송 */
    I2C_Wait(); 
    I2C_Stop();

}

/* 8bit Register 주소에서 한바이트 데이타 읽기 */ 
uint8_t I2C_ReadR8D8 (uint8_t slaveAddr, uint8_t regAddr)
{
	I2C_Wait();
	I2C_Start(); 
	I2C_Write((slaveAddr << 1) & 0xFE);   /* EEPROM 쓰기 주소 Addr */
    I2C_Wait();

    I2C_Write(regAddr);                 /* 레지스터 주소 전송 */
    I2C_Wait();                         /* ACK 대기 */    

    I2C_Restart();                      /* 재시작 조건 전송 */

    I2C_Write((slaveAddr << 1) | 0x01);   /* 읽기 슬레이브 주소 전송 */
    I2C_Wait();  

	I2C_Read();                         /* 데이터 읽기 시퀀스 */
    I2C_Nack();                         /* 읽기 종료 */
    I2C_Stop();                         /* Stop 조건 설정 */

    return (uint8_t)SSP1BUF;
}

/* 8bit 레지스터 주소에 16bit 데이터 쓰기 */
void I2C_WriteR8D16 (uint8_t slaveAddr, uint8_t regAddr, uint16_t value)
{
    uint8_t  highdata, lowdata;
    
    /* 워드 데이타를 바이트 단위로 분리 */
    highdata = (uint8_t)(value >> 8);  
    lowdata  = (uint8_t)(value & 0x00FF);
    
    /* 시작 조건 전송 */
    I2C_Start();                     /* Start By Master */

    /* Frame 1: Slave Address Byte 전송 */
    I2C_Write((slaveAddr << 1) & 0xFE);
    I2C_Wait();                      /* ACK By Slave */

    /* Frame 2: Register Address Byte 전송 */
    I2C_Write(regAddr);
    I2C_Wait();                     /* ACK By Slave */

    /* Frame 3: Data High Byte 전송 */
    I2C_Write(highdata);
    I2C_Wait();                      /* ACK By Slave */

    /* Frame 4: Data Low Byte 전송 */
    I2C_Write(lowdata);
    I2C_Wait();                      /* ACK By Slave */
    
    I2C_Stop();                      /* Stop By Master */
}

/* 8bit 레지스터 주소에 16bit 데이터 읽기 */
uint16_t I2C_ReadR8D16 (uint8_t slaveAddr, uint8_t regAddr)
{
    uint8_t  highdata, lowdata;
    
    /* 시작 조건 전송 */
    I2C_Start();                     /* Start By Master */

    /* Frame 1: Slave Address Byte 전송, Write Mode */
    I2C_Write((slaveAddr << 1) & 0xFE);
    I2C_Wait();                      /* ACK By Slave */

    /* Frame 2: Register Address Byte 전송 */
    I2C_Write(regAddr);
    I2C_Wait();                      /* ACK By Slave */

    I2C_Restart();                   /* Restart By Master */

    /* Frame 3: Slave Address Byte 전송, Read Mode */
    I2C_Write((slaveAddr << 1) | 0x01);
    I2C_Wait();                      /* ACK By Slave */  

    I2C_Read();
    highdata = SSP1BUF;  
    I2C_Ack();                      /* ACK By Master */  

    I2C_Read();
    lowdata = SSP1BUF;  
    I2C_Nack();                     /* ACK By Master */  

    I2C_Stop();                     /* Stop By Master */
    
    return ( ((uint16_t)highdata << 8) | ((uint16_t)lowdata) );
    
}

/* ************************************************************
*  End of i2c.c
************************************************************ */