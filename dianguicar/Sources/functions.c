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
float fre_diff,dis,LEFT_old,LEFT_new=0,RIGHT_old,RIGHT_new=0,MIDDLE_old,MIDDLE_new=0,temp_steer;
float LEFT_Temp,RIGHT_Temp,MIDDLE_Temp,Lsum,Rsum,Msum;
float sensor[3][10]={0},avr[10]={0.005,0.01,0.01,0.0125,0.0125,0.025,0.025,0.05,0.15,0.7};
unsigned int left,right,middle,flag=0,zd_flag=0; //车子在赛道的位置标志
unsigned int count1,count2,currentspeed,speed_target,speed[10],tempspeed; 
unsigned int presteer,currentsteer,dsteer;
unsigned int speed1=55,
			 speed2=48,
			 speed3=46,
			 speed4=44,
			 speed5=42;

float  /*	kp0=16.5,ki0=0,kd0=4.2,
		kp1=12,ki=0,kd1=3.3,// 分段PID
		kp2=7.8,ki2=0,kd2=2.15,  
		kp3=5.7,ki3=0,kd3=1.4,
		kp4=2.3,ki4=0,kd4=0.6;    */ //  空转83
		
	/*	kp0=16.7,ki0=0,kd0=4.2,
		kp1=12,ki=0,kd1=3.2,// 分段PID
		kp2=7.8,ki2=0,kd2=2.3,  
		kp3=5.7,ki3=0,kd3=1.6,
		kp4=2.3,ki4=0,kd4=0.65; //空转86*/


	/*	kp0=15.3,ki0=0,kd0=4.05,
		kp1=11.3,ki=0,kd1=3.2,// 分段PID
		kp2=8.3,ki2=0,kd2=2.45,//2.48,  
		kp3=5,ki3=0,kd3=1.5,
		kp4=2,ki4=0,kd4=0.5; */
	/*	kp0=15.7,ki0=0,kd0=3.9,
		kp1=11.6,ki=0,kd1=3.425,// 分段PID
		kp2=8.2,ki2=0,kd2=2.4,//2.48,  
		kp3=5.2,ki3=0,kd3=1.5,
		kp4=3,ki4=0,kd4=0.65;*/
		kp0=15.7,ki0=0,kd0=3.9,
		kp1=11.7,ki=0,kd1=3.4,// 分段PID
		kp2=8.2,ki2=0,kd2=2.38,//2.48,  
		kp3=5.2,ki3=0,kd3=1.4,
		kp4=2,ki4=0,kd4=0.65;




float kp,ki,kd;
int RIGHT,LEFT,MIDDLE,temp_fre[2],temp_steer_old,dtemp_steer;
unsigned char Outdata[8];
float sumerror,lasterror,Msetpoint=0,temp_middle=0,sensor_compensator=0,middleflag=0,start_left=0,start_right=0;
int Set_speed,temp_speed,pwm;
int speed_iError,speed_lastError,speed_prevError,Error[3],Sumerror;
float  	  speed_kp=5.4/2.5,
		  speed_ki=1.1/2.5,
		  speed_kd=0.258/2.5;
/* speed_kp=5.4/2.5,
	  speed_ki=1.1/2.5,
	  speed_kd=0.258/2.5;*/



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
		if(LEFT<=567&&RIGHT>=575)
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
		if(LEFT>=567&&RIGHT<=75)
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
	
	if(((flag==1)||(flag==2))&&(MIDDLE<=Msetpoint)) //左右打死保持
	{
		middleflag++;
		if(flag==1)
		{
			temp_steer=181;
			return(temp_steer);
		}
		if(flag==2)
		{
			temp_steer=-186;
			return(temp_steer);
		}
	}
		
	else
	{
		if(MIDDLE<=Msetpoint)      //中间线圈判定频率偏差大小
		{
			middleflag++;
//			if(middleflag>=2)           //u形弯处理  middleflag计数
//			{
				if(fre_diff>=0)
				{
					temp_steer=181;
					flag=1;
					return(temp_steer);
				}
				else
				{
					temp_steer=-186;
					flag=2;
					return(temp_steer);
				}
//			}
			
			if(fre_diff>=0) 
				fre_diff=20-fre_diff;
			else
				fre_diff=-21-fre_diff;
		
		}   
		else
			middleflag=0;	
		if(fre_diff>=0)
			fre_diff-=0;

		
		
		iError=fre_diff; 
		sumerror+=iError;
		dError=iError-lasterror;
		lasterror=iError;
		
		/*iError=se->SetPoint-Nextpoint; 
		se->SumError+=iError;
		dError=iError-se->LastError;
		se->LastError=iError;*/
			
		
		if(fre_diff>=-5&&fre_diff<=5)      //直道
		{
			flag=0;
			kp=kp4;
			kd=kd4;
		}
		else if(fre_diff>=-10&&fre_diff<=10)                                //小弯
		{
			if(fre_diff>=0)
			{
				kp=kp4+(kp3-kp4)/5*(fre_diff-5);
				kd=kd3;
			}
			else
			{
				kp=kp4+(kp3-kp4)/5*(-fre_diff-5);
				kd=kd3;
			}					
		}
		else if(fre_diff>=-15&&fre_diff<=15)                                //小弯
		{
			if(fre_diff>=0)
			{
				kp=kp3+(kp2-kp3)/5*(fre_diff-10);
				kd=kd2;
			}
			else
			{
				kp=kp3+(kp2-kp3)/5*(-fre_diff-10);
				kd=kd2;
			}					
		}
		else if(fre_diff>=-20&&fre_diff<=20)                                //小弯
		{
			if(fre_diff>=0)
			{
				kp=kp2+(kp1-kp2)/5*(fre_diff-20);
				kd=kd1;
			}
			else
			{
				kp=kp2+(kp1-kp2)/5*(-fre_diff-20);
				kd=kd1;
			}					
		}
		else                    //大弯
		{
			if(fre_diff>=0)
			{
				kp=kp1+(kp0-kp1)/10*(fre_diff-20);
				kd=kd0;
			}
			else
			{
				kp=kp1+(kp0-kp1)/10*(-fre_diff-20);
				kd=kd0;
			}								
		}
		
		temp_steer=(kp*iError+kd*dError)/2.5;
		if(temp_steer>=181)
			flag=1;               //左打死
		else if(temp_steer<=-186)
			flag=2;
		else 
			flag=0;
		return(temp_steer);
	}

}

unsigned int abs(signed int x)
{
	if(x>=0)
		return x;
	else 
		return -x;
}

void SpeedSet(void)
{
	dtemp_steer=temp_steer-temp_steer_old;
	temp_steer_old=temp_steer;
//	if(dtemp_steer>100&&dtemp_steer<-100)
//		speed_target=40;
	 if(temp_steer<30&&temp_steer>-30)  
    {
    	zd_flag++;
    	if(zd_flag>100)
        	speed_target = speed1;
    } 
    else if(temp_steer>-60 && temp_steer<60)
    {
    	if(zd_flag>25)
    	{
    		if(currentspeed>=speed1)
    			speed_target=speed4;
    		else if(currentspeed>=speed1-2)
    			speed_target=speed3;
    		else 
    			speed_target=speed3+1;	
    	}
    	else
    		speed_target = speed1-(abs(temp_steer)-30)/30*(speed1-speed2);
    	zd_flag=0;
    } 
    else if(temp_steer>-100 && temp_steer<100)
    {
    	zd_flag=0; 
        speed_target = speed3;//speed2-(abs(temp_steer)-60)/40*(speed2-speed3);
    } 
    else if(temp_steer>=-140 && temp_steer<140)
    {
    	zd_flag=0;
        speed_target = speed4;//speed3-(abs(temp_steer)-100)/40*(speed3-speed4);
    }  
    else 
    {
    	zd_flag=0;
        speed_target = speed5;//speed4-(abs(temp_steer)-140)/40*(speed4-speed5);
    }  
    
//    if(middleflag>100)
 //   	speed_target+=2;

    
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
	if(StopFlag)
		speed_target=0;
	speed_iError=speed_target*2.5-currentspeed;
	Error[2]=Error[1];
	Error[1]=Error[0];
	Error[0]=speed_iError;
	Sumerror+=speed_iError;
	
	
	
	//temp_speed=speed_kp*speed_iError+speed_ki*Sumerror+speed_kd*(speed_iError-speed_prevError);
	temp_speed+=speed_kp*(Error[0]-Error[1])+speed_ki*Error[0]+speed_kd*(Error[0]-Error[1]-(Error[1]-Error[2]));
	if(temp_speed>105)
		temp_speed=105;
	if(temp_speed<-50)
			temp_speed=-50;
	SET_motor(temp_speed);
	speed_prevError=speed_iError;
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
	Dis_Num(64,4,(WORD)currentspeed,5);
	Dis_Num(64,5,(WORD)speed_target,5);
	//Dis_Num(64,6,(WORD)fre_diff,5);

}

/****************************************************************************************************************
* 函数名称：SAIC1_inter()	
* 函数功能：光编输入脉冲捕捉
* 入口参数：无
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/02/23
*****************************************************************************************************************/
/*void SAIC1_inter(void) 
{

    if(  EMIOS_0.CH[3].CSR.B.FLAG  == 1)
	{
		count1++;
		EMIOS_0.CH[3].CSR.B.FLAG=1;    //清除标志位
	}
}*/
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
void Get_speed()  //定时2mse采速度
{
	unsigned k;
	count1=(WORD)EMIOS_0.CH[3].CCNTR.R;
	if (count1 >= count2)
		{
			currentspeed = count1 - count2;
		}
	else
		{
			currentspeed = 0xffff - (-count1 + count2);
		}
	if(forward)
		currentspeed=-currentspeed;
	count2=count1;
	/*for(k=0;k<9;k++)
		speed[k]=speed[k+1];
	speed[9]=currentspeed;
	tempspeed=0;
	for(k=0;k<10;k++)
		tempspeed+=speed[k]*avr[k];
	currentspeed=tempspeed;
	//PIT.CH[1].TFLG.B.TIF=1;*/
}
/****************************************************************************************************************
* 函数名称：speed_set( )	
* 函数功能：速度设定
* 出口参数：无
* 修改人  ：温泉
* 修改时间：2016/03/16
*****************************************************************************************************************/
/*void speed_set()
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
*/
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
	temp_middle=MIDDLE+9;
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
{	unsigned int i;
	Outdata[0]=(unsigned char)LEFT;
	Outdata[1]=(unsigned char)RIGHT;
	Outdata[2]=(unsigned char)speed_target*2.5;//EMIOS_0.CH[2].CBDR.R ;
	Outdata[3]=(unsigned char)currentspeed;
	Outdata[4]=(unsigned char)(LEFT>>8);
	Outdata[5]=(unsigned char)(RIGHT>>8);
	Outdata[6]=(unsigned char)(speed_target*2.5)>>8;//(EMIOS_0.CH[2].CBDR.R >>8);
	Outdata[7]=(unsigned char)(currentspeed>>8);
	LINFlex_TX('=');
	LINFlex_TX('=');
	for(i=0;i<8;i++)
	{
		SendHex(Outdata[i]);
		if(i==3)
		{
			LINFlex_TX('f');
			LINFlex_TX('f');
		}
	}
}

