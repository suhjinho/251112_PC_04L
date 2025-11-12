#ifndef	MAIN_H
#define MAIN_H

//#pragma warning disable 520
//#pragma warning disable 2053

#include <stdint.h>

#define AVG_DATA_BUFSIZE    5

/* Sensor Status */
#define STATUS_NORMAL                   0
#define STATUS_OBSOLESCENCE_RECOVER     1
#define STATUS_OBSOLESCENCE             2

/* Detect Level */
#define DETECTED_NORMAL             0
#define DETECTED_CHEMICAL           1
#define DETECTED_WATER              2

/*------------------------------------------------------------------------------
 * typedef
 ------------------------------------------------------------------------------*/
typedef struct AVG_Filter
{
	int16_t input_data;
	int32_t data_sum;
	int16_t avg_filtering_cnt;
	int16_t fitering_data;
    int16_t data_buf[AVG_DATA_BUFSIZE];
    int16_t min;
    int16_t max;
} AVGF_t;

typedef struct mTouchSensing
{
	uint16_t RD_data;
    int16_t BS_data;
    int16_t DV_data;
    int16_t detect_time_cnt;
} mTouchData_t;

/*------------------------------------------------------------------------------
 * function prototypes
 ------------------------------------------------------------------------------*/
void loadInitSetting(void);
void saveAllSetting(void);
void sensorStatusPrint(void);

void NormalStatusToggle(void);
void DetctNormalOut(void);
void DetectWaterOut(void);
void ObsolescenceOut(void);
void DetectChemicalOut(void);
void AllState_off(void);

void makeTimmingCallBack(void);
void varRegistanceProc(void);
void tempProc(void);
void eepromDump(int start, int count);
void monitoringProc(void);
void cmdProc(void);
void serialProc(void);
void setThresholdLevel(void);
void sensorStatus_refresh(void);
void showstate(void);
void AVG_Fitering(AVGF_t *_data, int16_t _inputData);
void baseline_Correction(void);
void referenceCorrection(void);
void referenceInit(void);
void referenceCalibration(void);
void Reading_Get_Proc(void);

#endif
