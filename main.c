#include "mcc_generated_files/mcc.h"
#include "system.h"

#include "tick_cnt.h"
#include "snr_tcn75a.h"
#include <stdlib.h>
#include "main.h"

#pragma warning disable     520     // function "*" is never called
#pragma warning disable     2053    // function "*" is never called
#pragma warning disable     1498    // redundant pointer access : 포인터 선언 함수 사용하면 없어짐
#pragma warning disable     1510    // 인터럽트 내 함수 사용
/*------------------------------------------------------------------------------
 *  Define
 ------------------------------------------------------------------------------*/
#define DEBUG                       0
#define LOGGING_DELAY_ON            1
#define SENSOR_TYPE                 2   // 1 : IG Point Sensor, 2 : OG Point Sensor
#define AUTO_RESET_ALARM            1   // 0 : Manual Reset Alarm, 1 : Auto Reset Alarm
#define REFERENCE_CORRECTION_MAX    1
#define SIGNAL_ON                   1
#define DETECT_TIME_COEFF           100 // (10ms x 100 = 1sec) x detectTime
#define BASELINE_COR                900 // 9.00



/* <editor-fold defaultstate="collapsed" desc="Default 설정값"> */
#define REFERENCE_TEMP_DEFAULT          25
#define REFERENCE_BASELINE_DEFAULT      13913
#define OBSOLESCENCE_DEFAULT            -1000
#define RECOVERY_LIMIT_DEFAULT          3000
#define THRESHOLD_DEFAULT1              50
#define TH_MIN_DEFAULT1                 30
#define TH_MAX_DEFAULT1                 300
#define THRESHOLD_DEFAULT2              50
#define TH_MIN_DEFAULT2                 -400//30  //-255
#define TH_MAX_DEFAULT2                 -50//300   // -175
/* </editor-fold> */



/*------------------------------------------------------------------------------
 *  Variables
 ------------------------------------------------------------------------------*/
/* <editor-fold defaultstate="collapsed" desc="전역변수 설정"> */
Time_t TimmingCnt;
int16_t varRegistance;
uint8_t fRecoveryEnable;
int16_t varObsolescenceOut;
int16_t varRecoveryLimit;
int16_t sensorStatus = 0;   // 0 = normal, 1 = detected, 2 = break, 3 = worn
int16_t PreSensorStatus = 0;   // 0 = normal, 1 = detected, 2 = break, 3 = worn
int16_t temperature, temperature_10, temperature_indicator;
int16_t threMin, threMax, SensorThreshold;
uint8_t DetectLevel = 0;
char cmdBuffer[80];
int16_t cmdBufferIndex = 0;
int16_t bitMonitoring = 0;
mTouchData_t levelSensor;
int16_t referenceTemp = 0;
uint16_t referenceBaseline = 0;
uint16_t RecoveryBaseline = 0;
uint16_t ObsolecenceEnableTimeCnt = 0;
uint16_t referenceCorrection_time_cnt = 0;
uint8_t fObsolescenceOut_test = 0;
uint16_t varObsolescenceOut_test = 0;
uint8_t RecoveryCnt = 0;
uint8_t fObsolescence = 0;
uint16_t sensorStatusNormalCnt = 0;
uint16_t sensorStatusBreakCnt = 0;
AVGF_t RD_avg;
AVGF_t temp_avg;
uint8_t TimeCnt_2sec = 0;
uint16_t detectTime = 1;
uint16_t baseline_cor;
/* </editor-fold> */
uint16_t Init_time_cnt = 3; // 자동보정 시작 시 3sec, 수동보정 0 으로 설정
uint16_t Init_time_cnt2 = 5; // 자동보정 시작 시 3sec, 수동보정 0 으로 설정


/*------------------------------------------------------------------------------
 *  Main application
 ------------------------------------------------------------------------------*/
void main(void)
{
    /* Initialize the device */
    SYSTEM_Initialize();
    AllState_off();
    TEMP_Initialize();
    Tick_Count_Initialize(&TimmingCnt);
    TMR0_SetInterruptHandler(makeTimmingCallBack);
    
    __delay_ms(50);
     
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    __delay_ms(10);
    loadInitSetting();
    printf("[AMS] Point Sensor\n\r");
    DetctNormalOut();
    
    while (1)
    {
        Tick_Count(&TimmingCnt);
        MTOUCH_Service_Mainloop();
        tempProc();
        if(TimmingCnt.Flag._100ms) varRegistanceProc();
        serialProc();
        if(TimmingCnt.Flag._500ms)
        {
            if(ObsolecenceEnableTimeCnt < 20) ObsolecenceEnableTimeCnt++;
        }
        
#if DEBUG
        if(fObsolescenceOut_test)
        {
            if(TimmingCnt.Flag._1ms) varObsolescenceOut_test++;
            if(varObsolescenceOut_test >= 10000) varObsolescenceOut_test = 10000;
        }
#endif
        if(!Init_time_cnt && !referenceCorrection_time_cnt)
        {
        }
        else if(Init_time_cnt)
        {
            
            if(abs(levelSensor.DV_data) > 10000)
            {
        //        printf("DV overflow detected: %d, Calibrating...\n\r", levelSensor.DV_data);
                referenceCalibration();
         //       Init_time_cnt = 0;  // 초기화 완료 처리
            }
            if(TimmingCnt.Flag._1sec)
            {
                printf("INIT...(%dsec)\n\r", Init_time_cnt);
                Init_time_cnt--;
                Init_time_cnt2--;
                if(Init_time_cnt == 1) 
                {
                    referenceCalibration();
                }
                 if(Init_time_cnt2 == 1) 
                {
                    referenceCalibration();
                } 
                
            }
        }            
        else if(referenceCorrection_time_cnt)
        {
            if(TimmingCnt.Flag._1sec)
            {
                if(referenceCorrection_time_cnt==12) referenceCorrection(); // 12 sec 후
                referenceCorrection_time_cnt--;
                printf("Recovery...(%dsec)\n\r", referenceCorrection_time_cnt);
            }
        }
        
#if SIGNAL_ON
        if(Init_time_cnt==0) showstate();
        else if(TimmingCnt.Flag._500ms) NOR_LED_Toggle();
#endif
    }
}



/*------------------------------------------------------------------------------
 *  Function
 ------------------------------------------------------------------------------*/
void loadInitSetting(void) // 설정값 읽기
{
    //if(0)
    if(DATAEE_ReadByte(EEP_ADDR_CONFIG)==0x3D)
    {
        SensorThreshold = (DATAEE_ReadByte(EEP_ADDR_THRESHOLD_H)<<8) | DATAEE_ReadByte(EEP_ADDR_THRESHOLD_L);
        threMin = (DATAEE_ReadByte(EEP_ADDR_THRE_MIN_H)<<8) | DATAEE_ReadByte(EEP_ADDR_THRE_MIN_L);
        threMax = (DATAEE_ReadByte(EEP_ADDR_THRE_MAX_H)<<8) | DATAEE_ReadByte(EEP_ADDR_THRE_MAX_L);
        varObsolescenceOut = (DATAEE_ReadByte(EEP_ADDR_OBSOLESCENCE_H)<<8) | DATAEE_ReadByte(EEP_ADDR_OBSOLESCENCE_L);
        referenceTemp = (DATAEE_ReadByte(EEP_ADDR_REFERENCE_TEMP_H)<<8) | DATAEE_ReadByte(EEP_ADDR_REFERENCE_TEMP_L);
        referenceBaseline = (DATAEE_ReadByte(EEP_ADDR_REFERENCE_BASELINE_H)<<8) | DATAEE_ReadByte(EEP_ADDR_REFERENCE_BASELINE_L);
        fRecoveryEnable = DATAEE_ReadByte(EEP_ADDR_RECOVERY_EN);
        varRecoveryLimit = (DATAEE_ReadByte(EEP_ADDR_RECOVERY_LIMIT_H)<<8) | DATAEE_ReadByte(EEP_ADDR_RECOVERY_LIMIT_L);
        RecoveryCnt = DATAEE_ReadByte(EEP_ADDR_RECOVERY_CNT);
        RecoveryBaseline = (DATAEE_ReadByte(EEP_ADDR_RECOVERY_BASELINE_H)<<8) | DATAEE_ReadByte(EEP_ADDR_RECOVERY_BASELINE_L);
        detectTime = (DATAEE_ReadByte(EEP_ADDR_DETECT_TIME_H)<<8) | DATAEE_ReadByte(EEP_ADDR_DETECT_TIME_L);
        baseline_cor = (DATAEE_ReadByte(EEP_ADDR_BASELINE_COR_H)<<8) | DATAEE_ReadByte(EEP_ADDR_BASELINE_COR_L);
    }
    else
    {
#if SENSOR_TYPE == 1
        SensorThreshold = THRESHOLD_DEFAULT1;
        threMin = TH_MIN_DEFAULT1;
        threMax = TH_MAX_DEFAULT1;
#elif SENSOR_TYPE == 2
        SensorThreshold = THRESHOLD_DEFAULT2;
        threMin = TH_MIN_DEFAULT2;
        threMax = TH_MAX_DEFAULT2;
#endif
        varObsolescenceOut = OBSOLESCENCE_DEFAULT;
        referenceTemp = REFERENCE_TEMP_DEFAULT;
        referenceBaseline = REFERENCE_BASELINE_DEFAULT;
        fRecoveryEnable = true;
        varRecoveryLimit = RECOVERY_LIMIT_DEFAULT;
        RecoveryCnt = 0;
        fObsolescence = 0;
        RecoveryBaseline = 0;
        detectTime = 2;//1;  //5
        baseline_cor = BASELINE_COR;
        Init_time_cnt = 3;  // init time 3 sec
    }

    printf("TH:%d\n\r", SensorThreshold);
    printf("TN:%d\n\r", threMin);
    printf("TM:%d\n\r", threMax);
    printf("OD:%d\n\r", varObsolescenceOut);
    printf("REEN:%d\n\r", fRecoveryEnable);
    printf("DT:%d\n\r", detectTime);
    printf("BC:%f\n\r", (baseline_cor*0.01));

    AllState_off();
}



void saveAllSetting(void) // 설정값 저장
{
    // 인터럽트 비활성화 (선택사항이지만 권장)
    uint8_t gie_backup = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    
    DATAEE_WriteByte(EEP_ADDR_CONFIG, 0x3D);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_THRESHOLD_H, SensorThreshold>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_THRESHOLD_L, SensorThreshold);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_THRE_MIN_H, threMin>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_THRE_MIN_L, threMin);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_THRE_MAX_H, threMax>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_THRE_MAX_L, threMax);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_EN, fRecoveryEnable);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_OBSOLESCENCE_H, varObsolescenceOut>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_OBSOLESCENCE_L, varObsolescenceOut);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_LIMIT_H, varRecoveryLimit>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_LIMIT_L, varRecoveryLimit);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_REFERENCE_TEMP_H, referenceTemp>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_REFERENCE_TEMP_L, referenceTemp);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_REFERENCE_BASELINE_H, referenceBaseline>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_REFERENCE_BASELINE_L, referenceBaseline);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_CNT, RecoveryCnt);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_H, RecoveryBaseline>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_L, RecoveryBaseline);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_DETECT_TIME_H, detectTime>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_DETECT_TIME_L, detectTime);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_BASELINE_COR_H, ((uint16_t)(baseline_cor))>>8);
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    DATAEE_WriteByte(EEP_ADDR_BASELINE_COR_L, (uint16_t)(baseline_cor));
    while(NVMCON1bits.WR);  // EEPROM 쓰기 완료 대기
    
    // 인터럽트 복원
    INTCONbits.GIE = gie_backup;
    
    // 추가 안정화 딜레이 (선택사항)
    __delay_ms(5);
    
    printf("Settings saved successfully\n\r");
}





void DetctNormalOut(void)
{
    NOR_LED_SetLow();
    LEAK_LED_SetHigh();
    LEAK_OUT_SetLow();
}

void DetectWaterOut(void)
{
    NOR_LED_SetLow();
    LEAK_LED_SetLow();
    LEAK_OUT_SetLow();
}

void ObsolescenceOut(void)
{
    NOR_LED_Toggle();
    LEAK_LED_Toggle();
    LEAK_OUT_SetLow();
}

void DetectChemicalOut(void)
{
    NOR_LED_SetLow();
    LEAK_LED_SetLow(); 
    LEAK_OUT_SetLow();//LEAK_OUT_SetHigh();
}

void AllState_off(void)
{
    NOR_LED_SetHigh();
    LEAK_LED_SetHigh();
    LEAK_OUT_SetLow();
}
/* </editor-fold> */

void makeTimmingCallBack(void) // 타이머 인터럽트 콜백
{
    Tick_Count_ISR(&TimmingCnt);
}

void varRegistanceProc(void) // ADC : 가변저항 & MCU온도
{
    if(MTOUCH_Sensor_isSampling()==false)
    {
        ADCC_Initialize();
        __delay_us(500);
        AVG_Fitering(&temp_avg, ADCC_GetSingleConversion(channel_Temp));
        temperature_indicator = (int16_t)((0.659 - 1.25*(1-temp_avg.fitering_data/1023.0))/0.00132 - 40);
        
        MTOUCH_Sensor_Scan_Initialize();
    }
}

void tempProc(void) // 온도센서 값
{
    int tempData;

    tempData = TEMP_Read();
    if(tempData > 2047) tempData = (tempData - 4096);   // 영하온도
    
    temperature = tempData >> 4; // 25 ℃ = 25
    temperature_10 = 10*tempData >> 4; // 25.0 ℃ = 250
}

void eepromDump(int start, int count) // EEPROM 값 출력
{
    int i;
    
    for(i=0;i<count;i++)
    {
        printf("ED:%d,%d", start+i, DATAEE_ReadByte(start+i));
        printf("\n\r");
    }    
}

void Reading_Get_Proc(void) // Mtouch Sensor Data
{
    baseline_Correction();
    levelSensor.RD_data = MTOUCH_Button_Reading_Get(Button0);

#if DEBUG
    levelSensor.RD_data -= varObsolescenceOut_test;
#endif

    AVG_Fitering(&RD_avg, levelSensor.RD_data);
    levelSensor.DV_data = (int16_t)(levelSensor.RD_data-levelSensor.BS_data);

#if SENSOR_TYPE == 2
    if((levelSensor.DV_data > threMin)&&(levelSensor.DV_data < threMax))
    {
        if(levelSensor.detect_time_cnt++ >= (DETECT_TIME_COEFF*detectTime))
        {
            levelSensor.detect_time_cnt = (DETECT_TIME_COEFF*detectTime);
            //if(DetectLevel == DETECTED_NORMAL) DetectLevel = DETECTED_CHEMICAL; // 물 감지 후 천천히 마를 경우 대비
            DetectLevel = DETECTED_CHEMICAL;
        }
    }
    else if(levelSensor.DV_data >= threMax)
    {
#if AUTO_RESET_ALARM
        levelSensor.detect_time_cnt = 0;
        //DetectLevel = DETECTED_WATER;
        DetectLevel = DETECTED_NORMAL;
#else
        levelSensor.detect_time_cnt = 0;
        if(DetectLevel == DETECTED_NORMAL) DetectLevel = DETECTED_WATER;
#endif
    }
#else
    if(levelSensor.DV_data >= SensorThreshold)
    {
        if(levelSensor.detect_time_cnt++ >= (DETECT_TIME_COEFF*detectTime))
        {
            levelSensor.detect_time_cnt = (DETECT_TIME_COEFF*detectTime);
            DetectLevel = DETECTED_CHEMICAL;
        }
    }
#endif
    else
    {
#if AUTO_RESET_ALARM
        levelSensor.detect_time_cnt = 0;
        DetectLevel = DETECTED_NORMAL;
#endif
    }
}

void monitoringProc(void) // 모티터링 값 출력
{
    if((levelSensor.DV_data) > 10000)
    {
    //        printf("DV overflow detected: %d, Calibrating...\n\r", levelSensor.DV_data);
        referenceCalibration();
    //       Init_time_cnt = 0;  // 초기화 완료 처리
    }
    else
    {
        if((bitMonitoring & BIT_smr) == BIT_smr || (bitMonitoring & BIT_sma) == BIT_sma) printf("RD:%d ", levelSensor.RD_data);
        if((bitMonitoring & BIT_smb) == BIT_smb || (bitMonitoring & BIT_sma) == BIT_sma) printf("BS:%d ", levelSensor.BS_data);
        if((bitMonitoring & BIT_smd) == BIT_smd || (bitMonitoring & BIT_sma) == BIT_sma) printf("DV:%d ", levelSensor.DV_data);
        if((bitMonitoring & BIT_davg) == BIT_davg || (bitMonitoring & BIT_sma) == BIT_sma) printf("DAVG:%d ", RD_avg.fitering_data);
    #if SENSOR_TYPE == 2
        if((bitMonitoring & BIT_smt) == BIT_smt || (bitMonitoring & BIT_sma) == BIT_sma) printf("TH:%d ", threMin);
        if((bitMonitoring & BIT_smt) == BIT_smt || (bitMonitoring & BIT_sma) == BIT_sma) printf("TM:%d ", threMax);
    #else
        if((bitMonitoring & BIT_smt) == BIT_smt || (bitMonitoring & BIT_sma) == BIT_sma) printf("TH:%d ", SensorThreshold);
    #endif    
        if((bitMonitoring & BIT_smp) == BIT_smp || (bitMonitoring & BIT_sma) == BIT_sma) printf("SE:%d ", sensorStatus);
        if((bitMonitoring & BIT_smp) == BIT_smp || (bitMonitoring & BIT_sma) == BIT_sma) printf("DE:%d ", DetectLevel);
        if((bitMonitoring & BIT_tim) == BIT_tim || (bitMonitoring & BIT_sma) == BIT_sma) printf("TI:%d ", temperature_indicator);
        if((bitMonitoring & BIT_tem) == BIT_tem || (bitMonitoring & BIT_sma) == BIT_sma) printf("TE:%d ", temperature);
    //    if((bitMonitoring & BIT_sma) == BIT_sma || (bitMonitoring & BIT_sma) == BIT_sma) printf("DBG:%d ", fObsolescence);
        if(bitMonitoring) printf("\n\r");

        /* MPLAB Visualizer streaming */
    //    EUSART1_Write(0x03);
    //    EUSART1_Write(levelSensor.RD_data & 0x00FF);
    //    EUSART1_Write(levelSensor.RD_data>>8);
    //    EUSART1_Write(levelSensor.BS_data & 0x00FF);
    //    EUSART1_Write(levelSensor.BS_data>>8);
    //    EUSART1_Write(levelSensor.DV_data & 0x00FF);
    //    EUSART1_Write(levelSensor.DV_data>>8);
    //    EUSART1_Write(0xFC);
    }
}

void cmdProc(void) // 시리얼 통신 CMD 응답
{
    int idx;
    int p1, p2;
    
    idx = splitCmd(cmdBuffer, &p1, &p2);
    if(idx == -1) return;
    
    switch(idx)
    {
        case 0: bitMonitoring |= BIT_smr; break; // Sensor monitoring reading
        case 1: bitMonitoring |= BIT_smb; break; // Sensor monitoring baseline
        case 2: bitMonitoring |= BIT_smd; break; // Deviation
        case 3: bitMonitoring |= BIT_smt;  break;   // threshold
        case 4: bitMonitoring |= BIT_smp; break;   // button push
        case 5: bitMonitoring |= BIT_sma; break;  // all
        case 6: printf("RD:%d\n\r", levelSensor.RD_data); break;
        case 7: printf("RB:%d\n\r", referenceBaseline); printf("RT:%d\n\r", referenceTemp); break;    // "srb"
        case 8: printf("DV%d\n\r", levelSensor.DV_data); break;
        case 9: bitMonitoring = 0; break;
        case 10: printf("SE:%d\n\r", sensorStatus); break;
        case 11: printf("TN:%d\n\r", threMin); break;
        case 12: printf("TM:%d\n\r", threMax); break;
        case 13: printf("RG:%d\n\r", varRegistance); break;
        case 14: bitMonitoring |= BIT_tim;  break;   // AD            
        case 15: printf("TH:%d\n\r", (DATAEE_ReadByte(EEP_ADDR_THRESHOLD_H)<<8) | DATAEE_ReadByte(EEP_ADDR_THRESHOLD_L)); break;
        case 16: printf("OD:%d\n\r", varObsolescenceOut); break;   // "sor" oldering read
        case 17: varObsolescenceOut = p1;
                DATAEE_WriteByte(EEP_ADDR_OBSOLESCENCE_H, varObsolescenceOut>>8);
                DATAEE_WriteByte(EEP_ADDR_OBSOLESCENCE_L, varObsolescenceOut);
                break;  // oldering write
        case 18: printf("EE:%d,%d\n\r", p1, DATAEE_ReadByte(p1)); break; // eeprom read
        case 19: DATAEE_WriteByte(p1, p2); printf("eew %d %d\n\r", p1, p2); break;
        case 20: eepromDump(p1, p2); break;      // "eed"
        case 21: loadInitSetting(); break;
        case 22: saveAllSetting(); break;   // "ees" : eepsave
        case 23: SensorThreshold = p1;
                DATAEE_WriteByte(EEP_ADDR_THRESHOLD_H, SensorThreshold>>8);
                DATAEE_WriteByte(EEP_ADDR_THRESHOLD_L, SensorThreshold);
                break;
        case 24: fRecoveryEnable = p1;
                DATAEE_WriteByte(EEP_ADDR_RECOVERY_EN, fRecoveryEnable);    
                break;
        case 25: printf("REB:%d\n\r", RecoveryBaseline);printf("REEN:%d\n\r", fRecoveryEnable); break;   // rer
        case 26: printf("TE:%d\n\r", temperature); break;
        case 27: bitMonitoring |= BIT_tem; break;
        case 28: threMin = p1;
                DATAEE_WriteByte(EEP_ADDR_THRE_MIN_H, threMin>>8);
                DATAEE_WriteByte(EEP_ADDR_THRE_MIN_L, threMin);
                break;
        case 29: threMax = p1;
                DATAEE_WriteByte(EEP_ADDR_THRE_MAX_H, threMax>>8);
                DATAEE_WriteByte(EEP_ADDR_THRE_MAX_L, threMax);
                break;
        case 30: RecoveryBaseline = p1;
                DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_H, RecoveryBaseline>>8);
                DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_L, RecoveryBaseline);
                break; //"rew" : set baseline
        case 31: AllState_off(); Init_time_cnt = 3;printf("Reference Correction\n\r");break;    // atc
        case 32: //MTOUCH_Proximity_Baseline_Set(0, p1);printf("Proximity Baseline Update\n\r");
                break;  // sfb 000000
        case 33: fObsolescenceOut_test = 1;printf("fWorn_test start(%d)\n\r", RecoveryCnt);break;   //wrs
        case 34: fObsolescenceOut_test = 0, varObsolescenceOut_test = 0; RecoveryCnt = 0; RecoveryBaseline = 0;
                ObsolecenceEnableTimeCnt = 0;
                sensorStatus = STATUS_NORMAL;
                DATAEE_WriteByte(EEP_ADDR_RECOVERY_CNT, RecoveryCnt);
                DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_H, RecoveryBaseline>>8);
                DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_L, RecoveryBaseline);
                printf("Recovery Test INIT\n\r");break;                         //wri
        case 35: referenceBaseline = p1; referenceTemp = p2;
            DATAEE_WriteByte(EEP_ADDR_REFERENCE_TEMP_H, referenceTemp>>8);
            DATAEE_WriteByte(EEP_ADDR_REFERENCE_TEMP_L, referenceTemp);
            DATAEE_WriteByte(EEP_ADDR_REFERENCE_BASELINE_H, referenceBaseline>>8);
            DATAEE_WriteByte(EEP_ADDR_REFERENCE_BASELINE_L, referenceBaseline);
            break;            //ssb
        case 36: printf("DT:%d\n\r", detectTime); break;   // "rdt"
        case 37: detectTime = p1;
            DATAEE_WriteByte(EEP_ADDR_DETECT_TIME_H, detectTime>>8);
            DATAEE_WriteByte(EEP_ADDR_DETECT_TIME_L, detectTime);
        case 38: printf("BC:%d\n\r", (uint16_t)(baseline_cor)); // "rdt"
            break;            //rbc
        case 39: baseline_cor = p1;
            DATAEE_WriteByte(EEP_ADDR_BASELINE_COR_H, baseline_cor>>8);
            DATAEE_WriteByte(EEP_ADDR_BASELINE_COR_L, baseline_cor);
            break;            //sbc
    }
}

void serialProc(void) // 시리얼 통신
{
    char ch;
    
    if(EUSART1_is_rx_ready() == true)
    {
        ch = getch();
        putch(ch);
        switch(ch)
        {
            case '\r':
                cmdBuffer[cmdBufferIndex] = '\0';
                cmdProc();                
                cmdBufferIndex = 0;
                break;
            case '\n':
                break;
            default:
                if(cmdBufferIndex < 80)
                {
                    cmdBuffer[cmdBufferIndex] = ch;
                    cmdBufferIndex++;
                }
                else
                {
                    cmdBufferIndex = 0;
                }
                break;
        }
    }
    else
    {
        if(TimmingCnt.Flag._10ms) Reading_Get_Proc();

#if LOGGING_DELAY_ON == 0
        if(TimmingCnt.Flag._100ms) monitoringProc();
#endif
#if LOGGING_DELAY_ON
        if(TimmingCnt.Flag._1sec) monitoringProc();
#endif
    }
}

void setThresholdLevel(void) // 가변저항 값에 따른 Thresh Hold 값
{
    double a, b, y;

    a = (double)(threMax - threMin) / (double)(AD_MAX - AD_MIN);
    b = (double)threMin - a * (double)AD_MIN;

    y = a * (double)varRegistance + b;

    SensorThreshold = (int)y;
}

void sensorStatus_refresh(void) // 센서상태(노후화) 판단
{
//    if((levelSensor.DV_data <= varObsolescenceOut)&&(ObsolecenceEnableTimeCnt>= 20))
//    {
//        if(fRecoveryEnable) // 리커버리 허용
//        {
//            if((RecoveryCnt >= REFERENCE_CORRECTION_MAX) || (levelSensor.RD_data <= varRecoveryLimit))
//            {
//                /* 리커버리 한계값보다 작으면 리커버리 진행안하고 노후화 경보 */
//                fObsolescenceOut_test = 0;
//                sensorStatus = STATUS_OBSOLESCENCE;
//            }
//            else
//            {
//                /* 12초 동안 노후화 경보 후 리커버리 진행하고, 센서상태 복귀 */
//                sensorStatus = STATUS_OBSOLESCENCE_RECOVER;
//                fObsolescenceOut_test = 0;
//                referenceCorrection_time_cnt = 12;
//                ObsolecenceEnableTimeCnt = 0;
//            }
//        }
//        else
//        {
//            sensorStatus = STATUS_OBSOLESCENCE;
//            fObsolescenceOut_test = 0;
//        }
//    }
}

void showstate(void) // 센서상태 표시
{
    /* 검출 결과 표시 */
    switch(DetectLevel)
    {
        case DETECTED_NORMAL:
            DetctNormalOut();
            break;        
        case DETECTED_CHEMICAL:
            if(TimmingCnt.Flag._500ms) DetectChemicalOut();
            break;
        case DETECTED_WATER:
            DetectWaterOut();
            break;
    }
}

void AVG_Fitering(AVGF_t *_data, int16_t _inputData) // 평균필터
{
	unsigned int i;

    _data->input_data = _inputData;
	_data->avg_filtering_cnt++;
	if(_data->avg_filtering_cnt >= AVG_DATA_BUFSIZE) _data->avg_filtering_cnt = AVG_DATA_BUFSIZE;

	_data->data_sum = 0;
    _data->min = 0x7FFF;
    _data->max = 0;

	/* data shift */
	for(i=1 ; i< AVG_DATA_BUFSIZE ; i++)
	{
		*(_data->data_buf+i-1) = *(_data->data_buf+i);
		_data->data_sum = _data->data_sum + *(_data->data_buf+i);
        
        if(*(_data->data_buf+i) < _data->min) _data->min = *(_data->data_buf+i);
        if(*(_data->data_buf+i) > _data->max) _data->max = *(_data->data_buf+i);
	}

	_data->data_sum += _data->input_data;
    if(_data->avg_filtering_cnt >= 3) _data->data_sum -= _data->min;
    if(_data->avg_filtering_cnt >= 3) _data->data_sum -= _data->max;
	*(_data->data_buf+AVG_DATA_BUFSIZE-1) = _data->input_data;

	if(_data->avg_filtering_cnt >= AVG_DATA_BUFSIZE)
	{
		_data->fitering_data = (int16_t)(_data->data_sum / (AVG_DATA_BUFSIZE-2));
	}
    else if(_data->avg_filtering_cnt >= 3) _data->fitering_data = (uint16_t)(_data->data_sum / (_data->avg_filtering_cnt-2));
    else _data->fitering_data = _data->input_data;
}

void baseline_Correction(void) // Baseline 보정 함수
{
    if(RecoveryCnt && fRecoveryEnable) levelSensor.BS_data = (int16_t)(RecoveryBaseline + (temperature - referenceTemp)*(baseline_cor*0.01));
    else levelSensor.BS_data = (int16_t)(referenceBaseline + (temperature - referenceTemp)*(baseline_cor*0.01));
}

void referenceInit(void) // 참조값 초기화
{
    referenceTemp = (DATAEE_ReadByte(EEP_ADDR_REFERENCE_TEMP_H)<<8) | DATAEE_ReadByte(EEP_ADDR_REFERENCE_TEMP_L);
    referenceBaseline = (DATAEE_ReadByte(EEP_ADDR_REFERENCE_BASELINE_H)<<8) | DATAEE_ReadByte(EEP_ADDR_REFERENCE_BASELINE_L);
    printf("RT:%d\n\r", referenceTemp);
    printf("RB:%d\n\r", referenceBaseline);  
}

void referenceCalibration(void) // 참조값 저장
{
    referenceTemp = temperature;
    referenceBaseline = levelSensor.RD_data;;
    RecoveryCnt=0;
    fObsolescence = 0;
    RecoveryBaseline=0;

    saveAllSetting();
    
    printf("RB:%d\n\r", referenceBaseline);  
    printf("RT:%d\n\r", referenceTemp);
}

void referenceCorrection(void) // 참조값 보정
{
    if(!RecoveryCnt) RecoveryBaseline = referenceBaseline - abs(levelSensor.DV_data);
    else RecoveryBaseline -= abs(levelSensor.DV_data);
    RecoveryCnt++;
    fObsolescence = 0;
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_CNT, RecoveryCnt);
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_H, RecoveryBaseline>>8);
    DATAEE_WriteByte(EEP_ADDR_RECOVERY_BASELINE_L, RecoveryBaseline);
}
/**
 End of File
*/