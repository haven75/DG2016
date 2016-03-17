#include "includes.h"
int Flag=0,wait=9;
signed int steer=0;





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
		//Senddata();
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


