#include "includes.h"
int Flag=0,wait=9,Supersonic_t=0,i;
signed int steer=0;
float Ramp_distance=0;




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
			if(steer<=1449)
				steer=1404;
			if(steer>=1814)
				steer=1860;
			while(Supersonic_t>6)
			{
				//if(steer>1580&&steer<1700)
					Supersonic_Trig();      //´¥·¢·¢ËÍ³¬Éù²¨
				Supersonic_t=0;
			}
			if(Supersonic_flag)
			{
			/*	Ramp_distance=Supersonic_Distance();    
				if(Ramp_distance<10) */
				//if( Supersonic_Time<50)
					Beep_ON();
				Supersonic_flag=0;	
			}
			Dis_Num(64,3,(WORD) steer,5);
			SET_steer(steer);
			SpeedSet();
			speed_control();
		}
		Flag=0;
		Senddata();
	}
}


void Pit0ISR()     
{
	Flag=1;
	frequency_measure();
	Get_speed();
	if(wait>0)
		wait--;
	Supersonic_t++;
	PIT.CH[0].TFLG.B.TIF = 1;
}


