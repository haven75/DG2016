/*
 * init.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#ifndef INIT_H_
#define INIT_H_

//*************************************************************************
//*	 *************************��ʼ������*************************	      *
//*************************************************************************
void initModesAndClock(void);			//ʱ�ӳ�ʼ��80M
void disableWatchdog(void);				//�ؿ��Ź�
void initEMIOS_0MotorAndSteer(void);	//������PWM��ʼ��   
void initPIT(void);						//PIT��ʱ��ʼ��10ms
void initEMIOS_0ModulusCounter(void);	//��������ʼ��(���)
void initLINFlex_0_UART (void);			//SCI��ʼ�� 57600
void initOLED(void);				        //��ʾ����ʼ��
void Pit0ISR(void)  ;                     //PIT0 2msec�жϿ���controlflag
void Pit1ISR(void);                       //PIT1 0.1msec�ж϶�ȡAD
void initKeys_Switchs(void);			    //����+���뿪��
void enableIrq(void);                    //�����ж�
//void SAIC_INIT(void);                    //���벶׽
void initADC(void);
void initALL(void);


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
