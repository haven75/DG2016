#include "includes.h"
int Flag;
signed int steer=0;
extern struct PID{
float SetPoint;
float Proportion;
float Integral;
float Derivative;
long SumError;
int LastError;
int PrevError; }spPID,sePID;




void main(void)
{
	initALL();
	
  for (;;) 
  {
    if(Flag)
    	{
    	sensor_display();
    	position();
    	GETservoPID();
    	steer=STEER_HELM_CENTER+LocPIDCal();
    	if(steer<540)
    		steer=540;
    	if(steer>840)
    		steer=840;
    	Dis_Num(64,3,(WORD)steer,5);
    	SET_steer(steer);

		Dis_Num(64, 5, (WORD)sePID.Proportion, 5);

    	}
    Flag=0;
  }
}


void Pit0ISR()     
{
	Flag=1;
	PIT.CH[0].TFLG.B.TIF = 1;
	frequency_measure();
}


