#include "includes.h"
int Flag=0,wait=9;
signed int steer=0;



<<<<<<< HEAD


void main(void)
{
=======
void main(void)
  {
>>>>>>> master
	initALL();
	while(wait>0);
	Set_Middlepoint();
	for (;;) 
	{
		if(Flag==1)
		{
			sensor_display();
			position();
<<<<<<< HEAD
			GETservoPID();
			steer=STEER_HELM_CENTER+LocPIDCal();
			if(steer<700)
				steer=692;
			if(steer>1050)
				steer=1058;
			Dis_Num(64,3,(WORD)steer,5);
			SET_steer(steer);
			SpeedSet();
			speed_control();
		}
		Flag=0;
		Senddata();

=======
			//GETservoPID();
			steer=STEER_HELM_CENTER+LocPIDCal();
			if(steer<700)
				steer=692;
			if(steer>1020)
				steer=1025;
			Dis_Num(64,3,(WORD)steer,5);
			SET_steer(steer);
		}
		//Senddata();
		//LINFlex_TX((unsigned char)steer);
		Flag=0;
>>>>>>> master
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
<<<<<<< HEAD
=======

>>>>>>> master
}


