/*
 * init.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#ifndef _INIT_H_
#define _INIT_H_

//*************************************************************************
//*	 *************************初始化函数*************************	      *
//*************************************************************************
extern void initModesAndClock(void);			//时钟初始化80M
extern void disableWatchdog(void);				//关看门狗
extern void initEMIOS_0MotorAndSteer(void);	//电机舵机PWM初始化   
extern void initPIT(void);						//PIT定时初始化10ms
extern void initEMIOS_0ModulusCounter(void);	//计数器初始化(光编)
extern void initLINFlex_0_UART (void);			//SCI初始化 57600
extern void initOLED(void);				        //显示屏初始化
extern void Pit0ISR(void)  ;                     //PIT0 2msec中断控制controlflag
extern void Pit1ISR(void);                       //PIT1 0.1msec中断读取AD
extern void initKeys_Switchs(void);			    //按键+拨码开关
extern void enableIrq(void);                    //开总中断
extern void SAIC_INIT(void);                    //输入捕捉
extern void initADC(void);
extern void initALL(void);


//*************************************************************************
//*		*************************接口函数***********************	      *
//*************************************************************************

//***********************电机舵机接口函数**********************************************************************//
void SET_steer(signed int ServoPWM);	//50Hz 占空比:7.5%+-5% 中位：50000*7.5%=3750
void SET_motor(signed int speed); //5kHz 正反转,要限位     
//*************************蓝牙接口函数************************************************************************//
void LINFlex_TX(unsigned char data);
void LINFlex_RX_Interrupt(void);

#endif /* INIT_H_ */
