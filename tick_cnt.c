//======================================================
// timer tick count
//======================================================

#include "tick_cnt.h"

void Tick_Count(Time_t *_time)
{
	_time->Flag._1ms	= 0;
	_time->Flag._10ms	= 0;
	_time->Flag._100ms	= 0;
	_time->Flag._250ms	= 0;
	_time->Flag._500ms	= 0;
	_time->Flag._1sec	= 0;
	
	if(_time->Tick.Flag_1ms)	{ _time->Flag._1ms		= 1; _time->Tick.Flag_1ms   	= 0; }
	if(_time->Tick.Flag_10ms)	{ _time->Flag._10ms		= 1; _time->Tick.Flag_10ms  	= 0; }
	if(_time->Tick.Flag_100ms)	{ _time->Flag._100ms	= 1; _time->Tick.Flag_100ms 	= 0; }
	if(_time->Tick.Flag_250ms)	{ _time->Flag._250ms	= 1; _time->Tick.Flag_250ms 	= 0; }
	if(_time->Tick.Flag_500ms)	{ _time->Flag._500ms	= 1; _time->Tick.Flag_500ms 	= 0; }
	if(_time->Tick.Flag_1sec)	{ _time->Flag._1sec		= 1; _time->Tick.Flag_1sec  	= 0; }
}

void Tick_Count_ISR(Time_t *_time)
{
//1ms	
	_time->Tick_Count_1ms = 0;
	_time->Tick_Count_10ms++;
	_time->Tick.Flag_1ms = 1;

//10ms	
	if(_time->Tick_Count_10ms >= 10)
	{
		_time->Tick_Count_10ms = 0;
		_time->Tick_Count_100ms++;
		_time->Tick_Count_250ms++;
		_time->Tick.Flag_10ms = 1;
	}

//100ms	
	if(_time->Tick_Count_100ms >= 10)
	{
		_time->Tick_Count_100ms = 0;
		_time->Tick_Count_500ms++;
		_time->Tick.Flag_100ms = 1;
	}
	
//250ms	
	if(_time->Tick_Count_250ms >= 25)
	{
		_time->Tick_Count_250ms = 0;
		_time->Tick.Flag_250ms = 1;
	}
	
//500ms	
	if(_time->Tick_Count_500ms >= 5)
	{
		_time->Tick_Count_500ms = 0;
		_time->Tick.Flag_500ms = 1;
		_time->Tick_Count_1sec++;
	}
	
//1000ms	
	if(_time->Tick_Count_1sec >= 2)
	{
		_time->Tick_Count_1sec = 0;
		_time->Tick.Flag_1sec = 1;
	}
}

void Tick_Count_Initialize(Time_t *_time)
{
	_time->Tick.Flag_1ms	= 0;
	_time->Tick.Flag_10ms 	= 0;
	_time->Tick.Flag_100ms	= 0;
	_time->Tick.Flag_250ms	= 0;
	_time->Tick.Flag_500ms	= 0;
	_time->Tick.Flag_1sec 	= 0;

	_time->Flag._1ms	= 0;
	_time->Flag._10ms	= 0;
	_time->Flag._100ms	= 0;
	_time->Flag._250ms	= 0;
	_time->Flag._500ms	= 0;
	_time->Flag._1sec	= 0;

	_time->Tick_Count_1ms	= 0;
	_time->Tick_Count_10ms	= 0;
	_time->Tick_Count_100ms	= 0;
	_time->Tick_Count_250ms	= 0;
	_time->Tick_Count_500ms	= 0;
	_time->Tick_Count_1sec	= 0;
}
