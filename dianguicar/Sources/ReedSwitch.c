/*
 * ReedSwitch.c
 *
 *  Created on: May 9, 2016
 *      Author: Quan
 */

#include "includes.h"
unsigned char StartFlag=0,StopFlag=0,running=0;

void StartDetect(void)
{
	if(ReedSwitch1==0&&ReedSwitch2==0)
	{
		StartFlag=1;
		running++;
	}
	if(running==2)
		StopFlag=1;
}
