#include "includes.h"
int Flag,wait=4;
int steer;





void main(void)
{
	initALL();
	if (wait <= 0)
	{
		Set_Middlepoint();
		while (1)
		{
			if(Flag)
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
				Senddata();
			}
			Flag=0;
		}
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


