# 1 "tick_cnt.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/suhji/.mchp_packs/Microchip/PIC18F-K_DFP/1.4.87/xc8\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "tick_cnt.c" 2




# 1 "./tick_cnt.h" 1



typedef struct BaseTimerCount
{
 unsigned int Tick_Count_1ms;
 unsigned char Tick_Count_10ms;
 unsigned char Tick_Count_100ms;
 unsigned char Tick_Count_250ms;
 unsigned char Tick_Count_500ms;
 unsigned char Tick_Count_1sec;

 struct
 {
  unsigned char Flag_1ms : 1;
  unsigned char Flag_10ms : 1;
  unsigned char Flag_100ms : 1;
  unsigned char Flag_250ms : 1;
  unsigned char Flag_500ms : 1;
  unsigned char Flag_1sec : 1;
 }Tick;

 struct
 {
  unsigned char _1ms : 1;
  unsigned char _10ms : 1;
  unsigned char _20ms : 1;
  unsigned char _100ms : 1;
  unsigned char _250ms : 1;
  unsigned char _500ms : 1;
  unsigned char _1sec : 1;
 }Flag;
} Time_t;

void Tick_Count(Time_t *_time);
void Tick_Count_ISR(Time_t *_time);
void Tick_Count_Initialize(Time_t *_time);
# 6 "tick_cnt.c" 2

void Tick_Count(Time_t *_time)
{
 _time->Flag._1ms = 0;
 _time->Flag._10ms = 0;
 _time->Flag._100ms = 0;
 _time->Flag._250ms = 0;
 _time->Flag._500ms = 0;
 _time->Flag._1sec = 0;

 if(_time->Tick.Flag_1ms) { _time->Flag._1ms = 1; _time->Tick.Flag_1ms = 0; }
 if(_time->Tick.Flag_10ms) { _time->Flag._10ms = 1; _time->Tick.Flag_10ms = 0; }
 if(_time->Tick.Flag_100ms) { _time->Flag._100ms = 1; _time->Tick.Flag_100ms = 0; }
 if(_time->Tick.Flag_250ms) { _time->Flag._250ms = 1; _time->Tick.Flag_250ms = 0; }
 if(_time->Tick.Flag_500ms) { _time->Flag._500ms = 1; _time->Tick.Flag_500ms = 0; }
 if(_time->Tick.Flag_1sec) { _time->Flag._1sec = 1; _time->Tick.Flag_1sec = 0; }
}

void Tick_Count_ISR(Time_t *_time)
{

 _time->Tick_Count_1ms = 0;
 _time->Tick_Count_10ms++;
 _time->Tick.Flag_1ms = 1;


 if(_time->Tick_Count_10ms >= 10)
 {
  _time->Tick_Count_10ms = 0;
  _time->Tick_Count_100ms++;
  _time->Tick_Count_250ms++;
  _time->Tick.Flag_10ms = 1;
 }


 if(_time->Tick_Count_100ms >= 10)
 {
  _time->Tick_Count_100ms = 0;
  _time->Tick_Count_500ms++;
  _time->Tick.Flag_100ms = 1;
 }


 if(_time->Tick_Count_250ms >= 25)
 {
  _time->Tick_Count_250ms = 0;
  _time->Tick.Flag_250ms = 1;
 }


 if(_time->Tick_Count_500ms >= 5)
 {
  _time->Tick_Count_500ms = 0;
  _time->Tick.Flag_500ms = 1;
  _time->Tick_Count_1sec++;
 }


 if(_time->Tick_Count_1sec >= 2)
 {
  _time->Tick_Count_1sec = 0;
  _time->Tick.Flag_1sec = 1;
 }
}

void Tick_Count_Initialize(Time_t *_time)
{
 _time->Tick.Flag_1ms = 0;
 _time->Tick.Flag_10ms = 0;
 _time->Tick.Flag_100ms = 0;
 _time->Tick.Flag_250ms = 0;
 _time->Tick.Flag_500ms = 0;
 _time->Tick.Flag_1sec = 0;

 _time->Flag._1ms = 0;
 _time->Flag._10ms = 0;
 _time->Flag._100ms = 0;
 _time->Flag._250ms = 0;
 _time->Flag._500ms = 0;
 _time->Flag._1sec = 0;

 _time->Tick_Count_1ms = 0;
 _time->Tick_Count_10ms = 0;
 _time->Tick_Count_100ms = 0;
 _time->Tick_Count_250ms = 0;
 _time->Tick_Count_500ms = 0;
 _time->Tick_Count_1sec = 0;
}
