#include "includes.h"
int Flag,wait=4;
signed int steer=0;
extern struct PID
{
	float SetPoint;
	float Proportion;
	float Integral;
	float Derivative;
	long SumError;
	int LastError;
	int PrevError;
}spPID, sePID;





void main(void)
{
	initALL();
	
	while(wait>0);
	Set_Middlepoint();
	for (;;) 
	{
		if(Flag==1)
		{
			sensor_display();
			position();
			GETservoPID();
			steer=STEER_HELM_CENTER+LocPIDCal();
			if(steer<522)
				steer=522;
			if(steer>855)
				steer=855;
			Dis_Num(64,3,(WORD)steer,5);
			SET_steer(steer);
		}
		Flag=0;
	}
}


void Pit0ISR()     
{
	Flag=1;
	PIT.CH[0].TFLG.B.TIF = 1;
	frequency_measure();
	if(wait>0)
		wait--;
}


