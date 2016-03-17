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
float fre_diff,dis,LEFT,LEFT_old,LEFT_new,RIGHT,RIGHT_old,RIGHT_new=0,MIDDLE,MIDDLE_old,MIDDLE_new,temp_steer;
float LEFT_Temp,RIGHT_Temp,MIDDLE_Temp,Lsum,Rsum,Msum;
float sensor[3][10]={0},avr[10]={0.005,0.01,0.01,0.0125,0.0125,0.025,0.025,0.05,0.15,0.7};
unsigned int left,right,middle,flag=0;//车子在赛道的位置标志
unsigned int count1=0,count2=0,currentspeed;
float  kp1=16,ki=0,kd1=4,   // 分段PID
		kp2=9,ki2=0,kd2=2.5,  
		kp3=5.5,ki3=0,kd3=1,
		kp4=2.5,ki4=0,kd4=0.1;    
float kp,ki,kd;
int temp_fre[2];
float sumerror,lasterror,Msetpoint,temp_middle,sensor_compensator,middleflag,start_left,start_right;
int Set_speed,temp_speed;
float speed_kp,speed_ki,speed_kd,speed_iError,speed_lastError,speed_prevError;


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
	

	for(i=0;i<9;i++)
	{
		sensor[0][i]=sensor[0][i+1];
		sensor[1][i]=sensor[1][i+1];
		sensor[2][i]=sensor[2][i+1];
	}
	sensor[0][9]=LEFT_Temp;
	sensor[1][9]=MIDDLE_Temp;
	sensor[2][9]=RIGHT_Temp;
	Lsum=0;
	Msum=0;
	Rsum=0;
	for( j=0;j<10;j++)
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
* 函数名称：InitsePID()	
* 函数功能：初始化舵机的PID参数
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/18
*****************************************************************************************************************/

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
	
//	if((LEFT>=start_left+8)&&(RIGHT>=start_right+8))  // 过十字
	//	return(0);
//	if(((flag==1)&&(LEFT<572))||((flag==2)&&(RIGHT<580)))
	

	Dis_Num(64,6,(WORD)Msetpoint,5);	
	if(((flag==1)||(flag==2))&&(MIDDLE<=Msetpoint)) //左右打死保持
	{
		middleflag++;
		if(flag==1)
			return(165);
		if(flag==2)
			return(-165);
	}
		
	else
	{
		if(MIDDLE<=Msetpoint)      //中间线圈判定频率偏差大小
		{
			middleflag++;
			if(middleflag>=28)           //u形弯处理  middleflag计数
			{
				if(fre_diff>=0)
				{
					flag=1;
					return(165);
				}
				else
				{
					flag=2;
					return(-165);
				}
			}         
			if(fre_diff>=0) 
				fre_diff=18-fre_diff;
			else if(fre_diff>=-11)
				fre_diff=-21-fre_diff;
		
		}   
		else
			middleflag=0;	
		
		if(fre_diff>=0)
			fre_diff+=1;
		
		
		iError=fre_diff; 
		sumerror+=iError;
		dError=iError-lasterror;
		lasterror=iError;
		
		/*iError=se->SetPoint-Nextpoint; 
		se->SumError+=iError;
		dError=iError-se->LastError;
		se->LastError=iError;*/
			
		
		if(fre_diff>=-2&&fre_diff<=2)      //直道
		{
			flag=0;
			kp=kp4;
			kd=kd4;
		}
		else if(fre_diff>=-5&&fre_diff<=5)                                //小弯
		{
			if(fre_diff>=0)
			{
				kp=kp4+(kp3-kp4)/3*(fre_diff-2);
				kd=kd3;
			}
			else
			{
				kp=kp4+(kp3-kp4)/3*(-fre_diff-2);
				kd=kd3;
			}					
		}
		else if(fre_diff>=-7&&fre_diff<=7)                                //小弯
		{
			if(fre_diff>=0)
			{
				kp=kp3+(kp2-kp3)/2*(fre_diff-5);
				kd=kd2;
			}
			else
			{
				kp=kp3+(kp2-kp3)/2*(-fre_diff-5);
				kd=kd2;
			}					
		}
		else                    //大弯
		{
			if(fre_diff>=0)
			{
				kp=kp2+(kp1-kp2)/13*(fre_diff-7);
				kd=kd3;
			}
			else
			{
				kp=kp2+(kp1-kp2)/13*(-fre_diff-7);
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
	Dis_Num(64,0,(WORD)LEFT,5);
	Dis_Num(64,1,(WORD)MIDDLE,5);
	Dis_Num(64,2,(WORD)RIGHT,5);
/*	Dis_Num(64,4,(WORD)fre_diff,5);
	Dis_Num(64,5,(WORD)(-fre_diff),5);*/

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
	temp_middle=MIDDLE-11;
	start_left=LEFT-14;
	start_right=RIGHT-14;
	sensor_compensator=RIGHT-LEFT;
	Msetpoint=temp_middle;
//	Dis_Num(64,6,(WORD)Msetpoint,5);
}

/****************************************************************************************************************
* 函数名称：speed_set( )	
* 函数功能：速度设定
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/03/16
*****************************************************************************************************************/
void speed_set()
{
	if(fre_diff>-2&&fre_diff<2)
		Set_speed=Strait;
	if(fre_diff>=-5&&fre_diff<=5)
		Set_speed=Littleround;
	if(fre_diff>=-7&&fre_diff<=7)
		Set_speed=LittleSround;
	if(middleflag>28)
		Set_speed=Uround;
	if(((flag==1)||(flag==2))&&(MIDDLE<=Msetpoint))     //判定不严谨
		Set_speed=Biground;
}
/****************************************************************************************************************
* 函数名称：speed_control( )	
* 函数功能：速度控制
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/03/16
*****************************************************************************************************************/
void speed_control()
{
	speed_iError=Set_speed-currentspeed;
	
	temp_speed=speed_kp*speed_iError-speed_ki*speed_lastError+speed_kd*speed_prevError;
	SET_motor(temp_speed);
	
	speed_prevError=speed_lastError;
	speed_lastError=speed_iError;
}
/****************************************************************************************************************
* 函数名称：Set_Middlepoint()	
* 函数功能：中间线圈频率标定
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/23
*****************************************************************************************************************/
void Set_Middlepoint()
{
	temp_middle=MIDDLE-11;
	start_left=LEFT-14;
	start_right=RIGHT-14;
	sensor_compensator=RIGHT-LEFT;
	Msetpoint=temp_middle;
	Dis_Num(64,6,(WORD)Msetpoint,5);
}


void SendHex(unsigned char hex) 
{
    unsigned char temp;
    temp = hex & 0x0F;
    if(temp < 10) 
    {
    	LINFlex_TX(temp + '0');
     }   
    else 
    {
    	LINFlex_TX(temp - 10 + 'A');
     }
     temp = hex >> 4;
     if(temp < 10) 
     {
    	 LINFlex_TX(temp + '0');
      } 
     else 
     {
    	 LINFlex_TX(temp - 10 + 'A');
      }
}

void Senddata()
{
	LINFlex_TX('=');
	LINFlex_TX('=');
	temp_fre[0]=(int)fre_diff;
	temp_fre[1]=((int)fre_diff)>>8;
	SendHex((unsigned char)temp_fre[0]);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex((unsigned char)temp_fre[1]);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
	SendHex(0x00);
}

