/*
 * functions.c
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */


/*
 * functions.c
 *
 *  Created on: Feb 20, 2016
 *      Author: Administrator
 */
#include"includes.h"
float LEFT,LEFT_old,LEFT_new=0,RIGHT,RIGHT_old,RIGHT_new=0,MIDDLE,MIDDLE_old,MIDDLE_new=0;
float LEFT_Temp,RIGHT_Temp,MIDDLE_Temp,Lsum,Rsum,Msum;
float sensor[3][5]={0},avr[5]={0.025,0.025,0.05,0.1,0.8};
unsigned int left,right,middle;//������������λ�ñ�־
unsigned int count1=0,count2=0;
float  kp1=8.2,ki=0,kd1=0,   //�ֶ�PID����
		kp2=0.2,ki2=0,kd2=0.2,
		kp3=0.5,ki3=0,kd3=0.25;
float kp,ki,kd;


/****************************************************************************************************************
* �������ƣ�frequency_measure()	
* �������ܣ���ȡ���Ƶ��
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/03/6
*****************************************************************************************************************/
void frequency_measure(void)
{
	unsigned int i,j;
	
	LEFT_new=(WORD)EMIOS_0.CH[24].CCNTR.R;
	if (LEFT_new >= LEFT_old)
		{
			LEFT_Temp = LEFT_new - LEFT_old;
		}
	else
		{
			LEFT_Temp = 0xffff - (-LEFT_new + LEFT_old);
		}
	LEFT_old=LEFT_new;
	
	
	RIGHT_new=(WORD)EMIOS_0.CH[23].CCNTR.R;
	if (RIGHT_new >= RIGHT_old)
		{
			RIGHT_Temp = RIGHT_new - RIGHT_old;
		}
	else
		{
			RIGHT_Temp = 0xffff - (-RIGHT_new + RIGHT_old);
		}
	RIGHT_old=RIGHT_new;
	

	MIDDLE_new=(WORD)EMIOS_0.CH[16].CCNTR.R;
	if (MIDDLE_new >= MIDDLE_old)
		{
			MIDDLE_Temp = MIDDLE_new - MIDDLE_old;
		}
		else
		{
			MIDDLE_Temp = 0xffff - (-MIDDLE_new + MIDDLE_old);
		}
	MIDDLE_old=MIDDLE_new;
	

	for(i=0;i<4;i++)
	{
		sensor[0][i]=sensor[0][i+1];
		sensor[1][i]=sensor[1][i+1];
		sensor[2][i]=sensor[2][i+1];
	}
	sensor[0][4]=LEFT_Temp;
	sensor[1][4]=MIDDLE_Temp;
	sensor[2][4]=RIGHT_Temp;
	Lsum=0;
	Msum=0;
	Rsum=0;
	for( j=0;j<5;j++)
	{
		Lsum+=sensor[0][j]*avr[j];
		Msum+=sensor[1][j]*avr[j];
		Rsum+=sensor[2][j]*avr[j];
	}
	LEFT=Lsum;
	MIDDLE=Msum;
	RIGHT=Rsum;
}

/****************************************************************************************************************
* �������ƣ�position()	
* �������ܣ����㳵�ӵ�λ��
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/03/6
*****************************************************************************************************************/
void position(void)
{
	if(LEFT<=563&&RIGHT<=571)
		middle=1;
	if(LEFT<563&&RIGHT>571)
		left=1;
	if(LEFT>563&&RIGHT<571)
		right=1;
}


/****************************************************************************************************************
* �������ƣ�GETservoPID()	
* �������ܣ���ȡ�����P��I��D
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/18
*****************************************************************************************************************/
void GETservoPID(void)
{
	if(middle==1)
		;
	else if(left==1)
	{
		if(LEFT<=563&&RIGHT>=569)
		{
			//se->Proportion=kp1;  ���ݲ���ֵ
			//se->Derivative=kd1;
			kp=kp1;
			kd=kd1;
		}
	/*	if(RIGHT>2610&&RIGHT<=2620)
		{
			se->Proportion=kp2;
			se->Derivative=kd2;
		}
		if(RIGHT>2620)
		{
			se->Proportion=kp3;
			se->Derivative=kd3;
		}*/
	}
	else if(right==1)
	{
		if(LEFT>=563&&RIGHT<=569)
		{
			//se->Proportion=kp1;
			//se->Derivative=kd1;
			kp=kp1;
			kd=kd1;
		}
	/*	if(LEFT>2610&&LEFT<=2620)
		{
			se->Proportion=kp2;
			se->Derivative=kd2;
		}
		if(LEFT>2620)
		{
			se->Proportion=kp3;
			se->Derivative=kd3;
		}*/
	}
}


/****************************************************************************************************************
* �������ƣ�InitsePID()	
* �������ܣ���ʼ�������PID����
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/18
*****************************************************************************************************************/
void InitsePID(void)
{
	se->SumError=0;
	se->LastError=0;
	se->PrevError=0;
	
	se->Proportion=0;
	se->Integral=0;
	se->Derivative=0;
	se->SetPoint=0;     
}
/****************************************************************************************************************
* �������ƣ�InitspPID()	
* �������ܣ���ʼ�������PID����
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/18
*****************************************************************************************************************/
void InitspPID(void)
{
	sp->SumError=0;
	sp->PrevError=0;
	sp->LastError=0;
	
	sp->Proportion=0;
	sp->Integral=0;
	sp->Derivative=0;
	sp->SetPoint=0;
}
/****************************************************************************************************************
* �������ƣ�LocPIDCal()	
* �������ܣ���������PWM�仯ֵ
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/18
*****************************************************************************************************************/
signed int LocPIDCal(void)
{
	register int iError,dError;
	unsigned int Nextpoint;
	
	se->SetPoint=LEFT+9;
	Nextpoint=RIGHT;
	
	iError=se->SetPoint-Nextpoint; 
	se->SumError+=iError;
	dError=iError-se->LastError;
	se->LastError=iError;
			
	if(MIDDLE>=522/*||((LEFT+9-RIGHT)<1&&(LEFT+9-RIGHT)>-1)*/)  //�м���Ȧ�ж�����ʱƫ������7������
	{
		return(kp1*iError+kd1*dError);
	}
	else            //��ʱƫ��������7����
	{
		if((LEFT+9-RIGHT)<0)
			return(-150);
		else
			return(150);
		
	} 
	
	//return(kp1*iError+kd1*dError);
	//return(se->Proportion*iError+se->Integral*se->SumError+se->Derivative*dError);
	//return(kp*iError+kd*dError);
}
/****************************************************************************************************************
* �������ƣ�sensor_display()	
* �������ܣ���ʾ������ֵ
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/18
*****************************************************************************************************************/
void sensor_display(void)
{
	//LCD_Print(8,0,"Left:");
	Dis_Num(64,0,(WORD)LEFT,5);
	//LCD_Print(8,1,"Middle:");
	Dis_Num(64,1,(WORD)MIDDLE,5);
	//LCD_Print(8,2,"Right:");
	Dis_Num(64,2,(WORD)RIGHT,5);
}

/****************************************************************************************************************
* �������ƣ�SAIC1_inter()	
* �������ܣ�����������岶׽
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/23
*****************************************************************************************************************/
void SAIC1_inter(void) 
{

    if(  EMIOS_0.CH[3].CSR.B.FLAG  == 1)
	{
		count1++;
		EMIOS_0.CH[3].CSR.B.FLAG=1;    //�����־λ
	}
}
/****************************************************************************************************************
* �������ƣ�SAIC2_inter()	
* �������ܣ�����������岶׽
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/23
*****************************************************************************************************************/
/*void SAIC2_inter(void) 
{

    if(  EMIOS_0.CH[7].CSR.B.FLAG  == 1)
	{
		count2++;
		EMIOS_0.CH[7].CSR.B.FLAG=1;    //�����־λ
	}
}
*/

/****************************************************************************************************************
* �������ƣ�Get_speed()	
* �������ܣ�10msec�жϻ�ȡ�ٶ�
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/23
*****************************************************************************************************************/
unsigned int Get_speed()  //��ʱ2mse���ٶ�
{
	unsigned int speed;
	if(forward)
		speed=count1;
	else
		speed=-count1;
	count1=0;
	PIT.CH[1].TFLG.B.TIF=1;
	return(speed);
}
