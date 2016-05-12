/*
 * AD_Scan.c
 *
 *  Created on: May 8, 2016
 *      Author: Quan
 */
#include "includes.h"


unsigned int tempAD[2], ADFilter[10]={0.005,0.01,0.01,0.0125,0.0125,0.025,0.025,0.05,0.15,0.7};

/**********************************读取AD值**********************************/
unsigned int ADread(unsigned int channel)
{
	unsigned int value;
	while(ADC.CDR[channel].B.VALID != 1);
	value=ADC.CDR[channel].B.CDATA;
}

/*********************************读取角速度和陀螺仪******************************/
void GET_AD_Channel(void)
{
	unsigned int i;
	tempAD[2]=0;
	 for(i=0;i<10;i++)        //PB4
	 {    
		 tempAD[0]+=ADFilter[i]*ADread(0);
	  } 
	 for(i=0;i<10;i++)        //PB4
	 {    
		 tempAD[1]+=ADFilter[i]*ADread(1);
	  } 
}
