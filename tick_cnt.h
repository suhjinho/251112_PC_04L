#ifndef	TICK_CNT_H
#define TICK_CNT_H

typedef struct BaseTimerCount
{
	unsigned int	Tick_Count_1ms;
	unsigned char	Tick_Count_10ms;
	unsigned char	Tick_Count_100ms;
	unsigned char	Tick_Count_250ms;
	unsigned char	Tick_Count_500ms;
	unsigned char	Tick_Count_1sec;

	struct
	{
		unsigned char Flag_1ms	: 1;
		unsigned char Flag_10ms 	: 1;
		unsigned char Flag_100ms 	: 1;
		unsigned char Flag_250ms 	: 1;
		unsigned char Flag_500ms 	: 1;
		unsigned char Flag_1sec 	: 1;
	}Tick;

	struct
	{
		unsigned char _1ms	: 1;
		unsigned char _10ms	: 1;
		unsigned char _20ms	: 1;
		unsigned char _100ms 	: 1;
		unsigned char _250ms 	: 1;
		unsigned char _500ms 	: 1;
		unsigned char _1sec 	: 1;
	}Flag;
} Time_t;

void Tick_Count(Time_t *_time);
void Tick_Count_ISR(Time_t *_time);
void Tick_Count_Initialize(Time_t *_time); 

#endif
