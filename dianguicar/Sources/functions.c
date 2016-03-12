/*
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
float fre_diff,dis,LEFT,LEFT_old,LEFT_new=0,RIGHT,RIGHT_old,RIGHT_new=0,MIDDLE,MIDDLE_old,MIDDLE_new=0,temp_steer,middleflag=0;
float LEFT_Temp,RIGHT_Temp,MIDDLE_Temp,Lsum,Rsum,Msum;
float sensor[3][5]={0},avr[5]={0.025,0.025,0.05,0.1,0.8};
unsigned int left,right,middle,flag=0;//车子在赛道的位置标志
unsigned int count1=0,count2=0;
float  kp1=15,ki=0,kd1=15,   //大弯
		kp2=9,ki2=0,kd2=4,  //小弯
		kp3=4,ki3=0,kd3=2;//直道PID
float kp,ki,kd;
extern float Msetpoint=0,temp_middle=0,sensor_compensator=0;


/****************************************************************************************************************
* 函数名称：frequency_measure()	
* 函数功能：获取电感频率
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/03/6
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
* 函数名称：position()	
* 函数功能：计算车子的位置
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/03/6
*****************************************************************************************************************/
void position(void)
{
	fre_diff=LEFT-RIGHT+sensor_compensator;
	if(fre_diff==0)
		middle=1; 
	if(fre_diff<0/*LEFT<563&&RIGHT>571*/)
		left=1;
	if(fre_diff>0/*LEFT>563&&RIGHT<571*/)
		right=1;
}


/****************************************************************************************************************
* 函数名称：GETservoPID()	
* 函数功能：获取舵机的P、I、D
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/18
*****************************************************************************************************************/
void GETservoPID(void)
{
	if(middle==1)
		;
	else if(left==1)
	{
		if(LEFT<=563&&RIGHT>=569)
		{
			//se->Proportion=kp1;  传递不了值
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
* 函数名称：LocPIDCal()	
* 函数功能：计算舵机的PWM变化值
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/18
*****************************************************************************************************************/
signed int LocPIDCal(void)
{
	register float iError,dError;
	
//	if(((flag==1)&&(LEFT<572))||((flag==2)&&(RIGHT<580)))
	if(((flag==1)||(flag==2))&&(MIDDLE<=Msetpoint))    //左右打死保持
	{
		if(flag==1)
			return(165);
		else if(flag==2)
			return(-165);
	}
	else                                   
	{
		if(MIDDLE<=Msetpoint)      //中间线圈判定频率偏差大小
		{
			middleflag++;
			if(middleflag>=50)           //u形弯处理
			{
				if(fre_diff>=0)
					return(165);
				else
					return(-165);
			}
			if(fre_diff>=0) 
				fre_diff=21-fre_diff;
			else if(fre_diff>=-11)
				fre_diff=-27-fre_diff;
		}
		middleflag=0;
		
		iError=fre_diff; 
		se->SumError+=iError;
		dError=iError-se->LastError;
		se->LastError=iError;
		
		/*iError=se->SetPoint-Nextpoint; 
		se->SumError+=iError;
		dError=iError-se->LastError;
		se->LastError=iError;*/
			
		if(fre_diff>=-3&&fre_diff<=3)      //直道
		{
			flag=0;
			kp=kp3;
			kd=kd3;
		}
		else if(fre_diff>=-8&&fre_diff<=8)                                //小弯
		{
			if(fre_diff>=0)
			{
				kp=kp2+(kp2-kp3)/5*(fre_diff-3);
				kd=kd2;
			}
			else
			{
				kp=kp2+(kp2-kp3)/5*(-fre_diff-3);
				kd=kd2;
			}					
		}
		else                    //大弯
		{
			if(fre_diff>=0)
			{
				kp=kp2+(kp1-kp2)/15*(fre_diff-8);
				kd=kd3;
			}
			else
			{
				kp=kp2+(kp1-kp2)/15*(-fre_diff-8);
				kd=kd3;
			}								
		}
		temp_steer=kp*iError+kd*dError;
		if(temp_steer>=165)
			flag=1;               //左打死
		else if(temp_steer<=-168)
			flag=2;
		else 
			flag=0;
		return(temp_steer);
	}

}
/****************************************************************************************************************
* 函数名称：sensor_display()	
* 函数功能：显示传感器值
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/18
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
* 函数名称：SAIC1_inter()	
* 函数功能：光编输入脉冲捕捉
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/23
*****************************************************************************************************************/
void SAIC1_inter(void) 
{

    if(  EMIOS_0.CH[3].CSR.B.FLAG  == 1)
	{
		count1++;
		EMIOS_0.CH[3].CSR.B.FLAG=1;    //清除标志位
	}
}
/****************************************************************************************************************
* 函数名称：SAIC2_inter()	
* 函数功能：光编输入脉冲捕捉
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/23
*****************************************************************************************************************/
/*void SAIC2_inter(void) 
{

    if(  EMIOS_0.CH[7].CSR.B.FLAG  == 1)
	{
		count2++;
		EMIOS_0.CH[7].CSR.B.FLAG=1;    //清除标志位
	}
}
*/

/****************************************************************************************************************
* 函数名称：Get_speed()	
* 函数功能：10msec中断获取速度
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/23
*****************************************************************************************************************/
unsigned int Get_speed()  //定时2mse采速度
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

void Set_Middlepoint()
{
	temp_middle=MIDDLE-2;
	sensor_compensator=RIGHT-LEFT;
	Msetpoint=temp_middle;
	Dis_Num(64,4,(WORD)sensor_compensator,5);
	Dis_Num(64,5,(WORD)Msetpoint,5);
}
