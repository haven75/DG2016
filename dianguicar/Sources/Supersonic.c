/*
 * Supersonic.c
 *
 *  Created on: May 7, 2016
 *      Author: Quan
 */

#include "includes.h"



unsigned char Supersonic_flag=0,f;
unsigned int t1,t2,Supersonic_Time;
void Supersonic_Trig(void)
{
	int t=0;
	Trig=ON;
	while(t<200)
		t++;
	Trig=OFF;
}



void Supersonic_Echo(void)
{
	Supersonic_flag=1;
	f++;
	t1=EMIOS_0.CH[7].CADR.R;
	t2=EMIOS_0.CH[7].CBDR.R;
	Beep_ON();
	if(t1>t2)
		Supersonic_Time=t1-t2;
	else
		Supersonic_Time=0xffff+t2-t1;
	Dis_Num(64,3,(WORD) Supersonic_Time,5);
	Dis_Num(20,4,(WORD) t1,5);
	Dis_Num(20,5,(WORD) t2,5);
	Dis_Num(20,0,(WORD) f,5);
	EMIOS_0.CH[7].CSR.B.FLAG=1; 
}


float Supersonic_Distance(void)
{
	float distance;
	distance=Supersonic_Time*340/2;
	return distance;
}
