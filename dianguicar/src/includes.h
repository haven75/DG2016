/*
 * includes.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#include "MPC5604B_M27V.h"
#include "init.h" 
#include "IntcInterrupts.h"
#include "define.h"
#include "functions.h"
#include "oleddriver.h"




#define byte unsigned char
#define word unsigned int


/***************foreward*****************/
#define forward SIU.GPDI[44].R	//����⳵ʵ�ʷ���:1ǰ�� 0����


/***************switchs*****************/
#define switch1 SIU.GPDI[56].R
#define switch2 SIU.GPDI[54].R
#define switch3 SIU.GPDI[52].R
#define switch4 SIU.GPDI[50].R
#define switch5 SIU.GPDI[48].R
#define switch6 SIU.GPDI[24].R


/***************��������*****************/
#define STEER_HELM_CENTER 690

