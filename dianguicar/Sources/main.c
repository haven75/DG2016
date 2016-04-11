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
			if(steer<503)
				steer=503;
			if(steer>851)
				steer=851;
			Dis_Num(64,3,(WORD)steer,5);
			SET_steer(steer);
			//speed_set();
		}
		//Senddata();
		//LINFlex_TX((unsigned char)steer);
		Flag=0;
	}
}


void Pit0ISR()     
{
	Flag=1;
	frequency_measure();
	Get_speed();
	if(wait>0)
		wait--;
	PIT.CH[0].TFLG.B.TIF = 1;

}


