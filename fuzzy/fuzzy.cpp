// Fuzzy.cpp : �������̨Ӧ�ó������ڵ㡣
//����ƫ��errorΪ��������ƫ��errorΪ����
/*С����������errorΪ����error_deltaΪ��
*С���������errorΪ����error_deltaΪ��
*С����������errorΪ����error_deltaΪ��
*С�����ҳ���errorΪ����error_deltaΪ��
*/

//#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

/***************************************�����������***************************************/
#define ke			0.6		///<����������ӣ������ò�����ʵ�����ӳ�䵽[-6,6]
#define kec			0.3		///<���仯�ʣ���һͼ���ǰһͼ�����ı仯���������ӣ������ò�����ʵ�����仯��ӳ�䵽[-6,6]
#define ku			20		///<����������ӣ������ò���������[-6,6]ӳ�䵽ʵ��ֵ
#define EHLimit		10		///<����errorʵ������
#define ELLimit		-10		///<����errorʵ������
#define ECHLimit	20		///<����error_deltaʵ������
#define ECLLimit	-20		///<����error_deltaʵ������
#define UHLimit		200		///<�������ʵ������
#define ULLimit		40		///<�������ʵ������


int Input1_Terms_Index=0,Input2_Terms_Index=0,Output_Terms_Index=0;///<��������������0���ӵ�6��ѯ7��


/**
* �����꣺NB,NM,NS,Z,PS,PM,PB
* �����꣺-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6
* @param �������롢��������Ⱥ���,����΢������ģ�����ķ�Χ
*/
/***************************************��������Ⱥ���***************************************/
float Input1_Terms_Membership[7][13]=
{ 1,0.5,0,0,0,0,0,0,0,0,0,0,0,
  0,0.5,1,0.5,0,0,0,0,0,0,0,0,0,
  0,0,0,0.5,1,0.5,0,0,0,0,0,0,0,
  0,0,0,0,0,0.5,1,0.5,0,0,0,0,0,
  0,0,0,0,0,0,0,0.5,1,0.5,0,0,0,
  0,0,0,0,0,0,0,0,0,0.5,1,0.5,0,
  0,0,0,0,0,0,0,0,0,0,0,0.5,1
};
/***************************************���仯�������Ⱥ���***************************************/
float Input2_Terms_Membership[7][13]=
{1,0.5,0,0,0,0,0,0,0,0,0,0,0,
0,0.5,1,0.5,0,0,0,0,0,0,0,0,0,
0,0,0,0.5,1,0.5,0,0,0,0,0,0,0,
0,0,0,0,0,0.5,1,0.5,0,0,0,0,0,
0,0,0,0,0,0,0,0.5,1,0.5,0,0,0,
0,0,0,0,0,0,0,0,0,0.5,1,0.5,0,
0,0,0,0,0,0,0,0,0,0,0,0.5,1
};
/***************************************������ٶȣ�***************************************/
float Output_Terms_Membership[7][13]=
{ 1,0.5,0,0,0,0,0,0,0,0,0,0,0,
0,0.5,1,0.5,0,0,0,0,0,0,0,0,0,
0,0,0,0.5,1,0.5,0,0,0,0,0,0,0,
0,0,0,0,0,0.5,1,0.5,0,0,0,0,0,
0,0,0,0,0,0,0,0.5,1,0.5,0,0,0,
0,0,0,0,0,0,0,0,0,0.5,1,0.5,0,
0,0,0,0,0,0,0,0,0,0,0,0.5,1
};

/**
* ����ΪE(error)������ΪEC(error_delta)��ֵΪ�ٶ��ߵ�NB(0),NM(1),NS(2),Z(3),PS(4),PM(5),PB(6)�ٶ���С���
*ȫ0��1��2��3��4��5��6��Ӧģ����ѯ��ȫ-6��-4��-2��0��2��4��6
* @param ģ�����ƹ���������ٶȱ仯����
*/
int Rule[7][7] =
{ 0,0,1,4,5,4,3,
0,0,2,5,5,5,4,
1,2,3,6,5,5,5,
3,4,5,6,5,4,3,
5,5,5,6,2,2,1,
4,5,5,5,2,0,0,
3,4,5,4,1,0,0
};/*int Rule[7][7] =
{ 0,0,1,4,5,4,3,
0,1,2,5,5,5,4,
1,2,3,6,5,5,5,
3,4,5,6,5,4,3,
5,5,5,6,2,2,1,
4,5,5,5,2,1,0,
3,4,5,4,1,0,0
};
  */

///<float SPEED[13]={200,220,230,240,250,270,300,270,250,240,230,220,200};//����
float  R[169][13]={0};
float R1[13][13]={0};
float AdBd1[13][13]={0};
float R2[169]={0};
float AdBd2[169]={0};
float R3[169][13]={0};
float  Cd[13]={0};
int Fuzzy_Table[13][13] = { 0 };
int Speed_Table[13][13] = { 0 };
int output[13] = {-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6};
int Max_Input1_value=0,Max_Input2_value=0;

/**
* ����ΪE(error)������ΪEC(error_delta)������ֵΪ�ٶ��ߵ�NB(0),NM(1),NS(2),Z(3),PS(4),PM(5),PB(6)�ٶ���С���
* @param ģ��������ʵ�������ڲ�ֵͬ��Ӧ��������������ֵ
*/
int  E_MAX(int e)
{
	int i=0,max=0;
	for(i=0;i<7;i++)
		if(Input1_Terms_Membership[i][e]>Input1_Terms_Membership[max][e])
			max=i;
	return max;
}
int  EC_MAX(int ex)
{
	int i=0,max=0;
	for(i=0;i<7;i++)
		if(Input2_Terms_Membership[i][ex]>Input1_Terms_Membership[max][ex])
			max=i;
	return max;
}

void calculate()
{
	/***************************************�������й���ģ����ϵ�Ĳ���Rule***************************************/
	int Input1_value_index=0,Input2_value_index=0;
	int i=0,j=0,k=0;
	for(Input1_Terms_Index=0;Input1_Terms_Index<7;Input1_Terms_Index++)
		for(Input2_Terms_Index=0;Input2_Terms_Index<7;Input2_Terms_Index++)
		{
			Output_Terms_Index=Rule[Input1_Terms_Index][Input2_Terms_Index];
			k=0;
			for(i=0;i<13;i++)
				for(j=0;j<13;j++)
				{
					if(Input1_Terms_Membership[Input1_Terms_Index][i]<Input2_Terms_Membership[Input2_Terms_Index][j])
						R1[i][j]=Input1_Terms_Membership[Input1_Terms_Index][i];
					else
						R1[i][j]=Input2_Terms_Membership[Input2_Terms_Index][j];
					R2[k]=R1[i][j];
					k++;
				}
			///<A=Input1_Terms_Membership[Input1_Terms_Index],B=Input2_Terms_Membership[Input2_Terms_Index]
			///<R1=AXB����13x13�ľ���,R2=R1'�Ѿ���ת��169x1��������
			for(i=0;i<169;i++)
				for(j=0;j<13;j++)
				{
					if(R2[i]<Output_Terms_Membership[Output_Terms_Index][j])
						R3[i][j]=R2[i];
					else
						R3[i][j]=Output_Terms_Membership[Output_Terms_Index][j];
					if(R3[i][j]>R[i][j])
						R[i][j]=R3[i][j];					
				}///<C=Output_Terms_Membership[Output_Terms_Index],R3=R2XC
		}
	
	
	/*************************����ÿ�ֿ��ܵ�E��EC�ľ�ȷȡֵģ�������������õ�ģ�����Cd*************************/
	for(Input1_value_index=0;Input1_value_index<13;Input1_value_index++)
	{
		for(Input2_value_index=0;Input2_value_index<13;Input2_value_index++)
		{
			for(j=0;j<13;j++)
				Cd[j]=0;
			int kd=0;	
			float temp=0;
			Max_Input1_value=E_MAX(Input1_value_index); ///<�ҳ������������������ֵ
			Max_Input2_value=EC_MAX(Input2_value_index);///<�ҳ����仯����������������ֵ
			for(i=0;i<13;i++)
				for(j=0;j<13;j++)
				{
					if(Input1_Terms_Membership[Max_Input1_value][i]<Input2_Terms_Membership[Max_Input2_value][j])
						AdBd1[i][j]=Input1_Terms_Membership[Max_Input1_value][i];
					else
						AdBd1[i][j]=Input2_Terms_Membership[Max_Input2_value][j];
					AdBd2[kd]=AdBd1[i][j];
					kd++;
				}///<Rd1��Ad��Bd,Rd2��Rd1'
			for(j=0;j<13;j++)
				for (i = 0; i<169; i++)
				{
					if(AdBd2[i]<R[i][j])
						temp=AdBd2[i];
					else
						temp=R[i][j];
					if(temp>Cd[j])
						Cd[j]=temp;
				}///<Cd��Rd2oR

			/*************************ȥģ��������Ȩƽ������������ʵ�����*************************/
			int OUT;
			float sum1 = 0, sum2 = 0;
			for(i=0;i<13;i++)
			{

					sum1=sum1+Cd[i];
					sum2=sum2+Cd[i]* output[i];
			}
			if(sum2 / sum1>0)
				OUT=(int)(sum2/sum1+0.5);
			else
				OUT = (int)(sum2 / sum1 - 0.5);///<��������
			Fuzzy_Table[Input1_value_index][Input2_value_index]=OUT;
			Speed_Table[Input1_value_index][Input2_value_index] = ku*OUT + (UHLimit + ULLimit) / 2;///<��ģ����תΪʵ���ٶȱ�
		}
	}
}

void main()
{
	float ek;
	float eck;//�����k���������ڵ��������仯��
	int E=0,EC=0,U=0,u=0;
	int s_i, s_j;
	calculate();
	cout << "Fuzzy_Table" << endl;
	for (s_i = 0; s_i < 13; s_i++)
	{
		for (s_j = 0; s_j < 13; s_j++)
		{
			u = Fuzzy_Table[s_i][s_j];
			cout << setw (2) << u << ",";
		}
		cout << endl;
	}
	cout <<"Speed_Tab" << endl;
	for (s_i = 0; s_i < 13; s_i++)
	{
		for (s_j = 0; s_j < 13; s_j++)
		{
			u = Speed_Table[s_i][s_j];
			cout << setw(4) << u << ",";
		}
		cout << endl;
	}
	cin >> u;
	/*cout<<"ek:"<<endl;
	cin>> ek;
	cout<<"eck:"<<endl;
	cin >> eck; 
	E = ke*(ek - (EHLimit + ELLimit) / 2)+0.5;//��E�ķ�Χת����ģ��������������
	if(E>6)
		E=6;
	else if(E<-6)
		E=-6;
	EC = kec*(eck - (ECHLimit + ECLLimit) / 2) + 0.5; //��EC�ķ�Χת����ģ��������������
	if(EC>6)
		EC=6;
	else if(EC<-6)
		EC=-6;
	U=Fuzzy_Table[E+6][EC+6]; //��ģ�����Ʋ�ѯ��õ����ֵU
	u=Speed_Table[E+6][EC+6]; //��ģ�����Ʋ�ѯ��õ����ֵu
	u=ku*U+(UHLimit + ULLimit)/2; //�����ת����ʵ������
	cout<<U;
	cout << endl;
	cout << u;
	cout << endl;
	*/
}



