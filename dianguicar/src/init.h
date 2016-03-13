/*
 * init.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#ifndef _INIT_H_
#define _INIT_H_

//*************************************************************************
//*	 *************************��ʼ������*************************	      *
//*************************************************************************
extern void initModesAndClock(void);			//ʱ�ӳ�ʼ��80M
extern void disableWatchdog(void);				//�ؿ��Ź�
extern void initEMIOS_0MotorAndSteer(void);	//������PWM��ʼ��   
extern void initPIT(void);						//PIT��ʱ��ʼ��10ms
extern void initEMIOS_0ModulusCounter(void);	//��������ʼ��(���)
extern void initLINFlex_0_UART (void);			//SCI��ʼ�� 57600
extern void initOLED(void);				        //��ʾ����ʼ��
extern void Pit0ISR(void)  ;                     //PIT0 2msec�жϿ���controlflag
extern void Pit1ISR(void);                       //PIT1 0.1msec�ж϶�ȡAD
extern void initKeys_Switchs(void);			    //����+���뿪��
extern void enableIrq(void);                    //�����ж�
extern void SAIC_INIT(void);                    //���벶׽
extern void initADC(void);
extern void initALL(void);


//*************************************************************************
//*		*************************�ӿں���***********************	      *
//*************************************************************************

//***********************�������ӿں���**********************************************************************//
void SET_steer(signed int ServoPWM);	//50Hz ռ�ձ�:7.5%+-5% ��λ��50000*7.5%=3750
void SET_motor(signed int speed); //5kHz ����ת,Ҫ��λ     
//*************************�����ӿں���************************************************************************//
void LINFlex_TX(unsigned char data);
void LINFlex_RX_Interrupt(void);

#endif /* INIT_H_ */
