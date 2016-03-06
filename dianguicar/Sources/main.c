#include "includes.h"
int Flag=0;
signed int steer=0;




void main(void)
{
	initALL();
	InitsePID();
  for (;;) 
  {
    if(Flag==1)
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


