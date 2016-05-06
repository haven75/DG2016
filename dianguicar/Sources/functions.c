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
unsigned int left,right,middle,flag=0,zd_flag=0; //������������λ�ñ�־
unsigned int count1,count2,currentspeed,speed_target; 
unsigned int presteer,currentsteer,dsteer;
unsigned int speed1=62,
			 speed2=47,
			 speed3=46,
			 speed4=44,
			 speed5=42;
float  /*	kp0=16.5,ki0=0,kd0=4.2,
		kp1=12,ki=0,kd1=3.3,// �ֶ�PID
		kp2=7.8,ki2=0,kd2=2.15,  
		kp3=5.7,ki3=0,kd3=1.4,
		kp4=2.3,ki4=0,kd4=0.6;    */ //  ��ת83
		
	/*	kp0=16.7,ki0=0,kd0=4.2,
		kp1=12,ki=0,kd1=3.2,// �ֶ�PID
		kp2=7.8,ki2=0,kd2=2.3,  
		kp3=5.7,ki3=0,kd3=1.6,
		kp4=2.3,ki4=0,kd4=0.65; //��ת86*/

		kp0=15.7,ki0=0,kd0=4,
		kp1=11.5,ki=0,kd1=3.3,// �ֶ�PID
		kp2=8.5,ki2=0,kd2=2.5,  
		kp3=5.4,ki3=0,kd3=1.7,
		kp4=2.3,ki4=0,kd4=0.65; 
float kp,ki,kd;
int RIGHT,LEFT,MIDDLE,temp_fre[2],temp_steer_old,dtemp_steer;
unsigned char Outdata[8];
float sumerror,lasterror,Msetpoint=0,temp_middle=0,sensor_compensator=0,middleflag=0,start_left=0,start_right=0;
int Set_speed,temp_speed,pwm;
int speed_iError,speed_lastError,speed_prevError,Error[3],Sumerror;
float speed_kp=5.4/2.5,
	  speed_ki=1.1/2.5,
	  speed_kd=0.258/2.5;


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
* �������ƣ�position()	
* �������ܣ����㳵�ӵ�λ��
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/03/6
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
		if(LEFT<=567&&RIGHT>=575)
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
* �������ƣ�InitsePID()	
* �������ܣ���ʼ�������PID����
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/18
*****************************************************************************************************************/

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
	register float iError,dError;
	
//	if((LEFT>=start_left+8)&&(RIGHT>=start_right+8))  // ��ʮ��
	//	return(0);
//	if(((flag==1)&&(LEFT<572))||((flag==2)&&(RIGHT<580)))
	
	if(((flag==1)||(flag==2))&&(MIDDLE<=Msetpoint)) //���Ҵ�������
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
		if(MIDDLE<=Msetpoint)      //�м���Ȧ�ж�Ƶ��ƫ���С
		{
			middleflag++;
//			if(middleflag>=2)           //u���䴦��  middleflag����
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
			
		
		if(fre_diff>=-5&&fre_diff<=5)      //ֱ��
		{
			flag=0;
			kp=kp4;
			kd=kd4;
		}
		else if(fre_diff>=-10&&fre_diff<=10)                                //С��
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
		else if(fre_diff>=-15&&fre_diff<=15)                                //С��
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
		else if(fre_diff>=-20&&fre_diff<=20)                                //С��
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
		else                    //����
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
			flag=1;               //�����
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
    		speed_target = speed2-(abs(temp_steer)-30)/30*(speed2-speed1);
    	zd_flag=0;
    } 
    else if(temp_steer>-100 && temp_steer<100)
    {
    	zd_flag=0; 
        speed_target = speed3-(abs(temp_steer)-60)/40*(speed3-speed2);
    } 
    else if(temp_steer>=-140 && temp_steer<140)
    {
    	zd_flag=0;
        speed_target = speed4-(abs(temp_steer)-100)/40*(speed4-speed3);
    }  
    else 
    {
    	zd_flag=0;
        speed_target = speed5-(abs(temp_steer)-140)/40*(speed5-speed4);
    }  
    
//    if(middleflag>100)
 //   	speed_target+=2;
    
}

/****************************************************************************************************************
* �������ƣ�speed_control( )	
* �������ܣ��ٶȿ���
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/03/16
*****************************************************************************************************************/
void speed_control()
{
	speed_iError=speed_target*2.5-currentspeed;
	Error[2]=Error[1];
	Error[1]=Error[0];
	Error[0]=speed_iError;
	Sumerror+=speed_iError;
	
	
	
	//temp_speed=speed_kp*speed_iError+speed_ki*Sumerror+speed_kd*(speed_iError-speed_prevError);
	temp_speed+=speed_kp*(Error[0]-Error[1])+speed_ki*Error[0]+speed_kd*(Error[0]-Error[1]-(Error[1]-Error[2]));
	if(temp_speed>115)
		temp_speed=115;
	if(temp_speed<-100)
			temp_speed=-100;
	SET_motor(temp_speed);
	speed_prevError=speed_iError;
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
	Dis_Num(64,0,(WORD)LEFT,5);
	Dis_Num(64,1,(WORD)MIDDLE,5);
	Dis_Num(64,2,(WORD)RIGHT,5);
	Dis_Num(64,4,(WORD)currentspeed,5);
	Dis_Num(64,5,(WORD)speed_target,5);
	//Dis_Num(64,6,(WORD)fre_diff,5);

}

/****************************************************************************************************************
* �������ƣ�SAIC1_inter()	
* �������ܣ�����������岶׽
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/23
*****************************************************************************************************************/
/*void SAIC1_inter(void) 
{

    if(  EMIOS_0.CH[3].CSR.B.FLAG  == 1)
	{
		count1++;
		EMIOS_0.CH[3].CSR.B.FLAG=1;    //�����־λ
	}
}*/
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
void Get_speed()  //��ʱ2mse���ٶ�
{
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
	//PIT.CH[1].TFLG.B.TIF=1;*/
}
/****************************************************************************************************************
* �������ƣ�speed_set( )	
* �������ܣ��ٶ��趨
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/03/16
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
	if(((flag==1)||(flag==2))&&(MIDDLE<=Msetpoint))     //�ж����Ͻ�
		Set_speed=Biground;
}
*/
/****************************************************************************************************************
* �������ƣ�Set_Middlepoint()	
* �������ܣ��м���ȦƵ�ʱ궨
* ��ڲ�������
* ���ڲ�������
* �޸���  ����Ȫ
* �޸�ʱ�䣺2016/02/23
*****************************************************************************************************************/
void Set_Middlepoint()
{
	temp_middle=MIDDLE+11;
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

