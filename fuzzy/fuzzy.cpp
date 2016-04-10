// Fuzzy.cpp : 定义控制台应用程序的入口点。
//赛道偏左error为负，赛道偏右error为正；
/*小车向左入弯error为负，error_delta为负
*小车向左出弯error为负，error_delta为正
*小车向右入弯error为正，error_delta为正
*小车向右出弯error为正，error_delta为负
*/

//#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

/***************************************试验调整参数***************************************/
#define ke			0.6		///<误差量化因子，调整该参数把实际误差映射到[-6,6]
#define kec			0.3		///<误差变化率（后一图像和前一图像误差的变化）量化因子，调整该参数把实际误差变化率映射到[-6,6]
#define ku			20		///<输出比例因子，调整该参数把论域[-6,6]映射到实际值
#define EHLimit		10		///<定义error实际上限
#define ELLimit		-10		///<定义error实际下限
#define ECHLimit	20		///<定义error_delta实际上限
#define ECLLimit	-20		///<定义error_delta实际上限
#define UHLimit		200		///<定义输出实际上限
#define ULLimit		40		///<定义输出实际下限


int Input1_Terms_Index=0,Input2_Terms_Index=0,Output_Terms_Index=0;///<索引参数，均从0增加到6查询7档


/**
* 列坐标：NB,NM,NS,Z,PS,PM,PB
* 横坐标：-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6
* @param 建立输入、输出隶属度函数,进行微调调整模糊量的范围
*/
/***************************************误差隶属度函数***************************************/
float Input1_Terms_Membership[7][13]=
{ 1,0.5,0,0,0,0,0,0,0,0,0,0,0,
  0,0.5,1,0.5,0,0,0,0,0,0,0,0,0,
  0,0,0,0.5,1,0.5,0,0,0,0,0,0,0,
  0,0,0,0,0,0.5,1,0.5,0,0,0,0,0,
  0,0,0,0,0,0,0,0.5,1,0.5,0,0,0,
  0,0,0,0,0,0,0,0,0,0.5,1,0.5,0,
  0,0,0,0,0,0,0,0,0,0,0,0.5,1
};
/***************************************误差变化率隶属度函数***************************************/
float Input2_Terms_Membership[7][13]=
{1,0.5,0,0,0,0,0,0,0,0,0,0,0,
0,0.5,1,0.5,0,0,0,0,0,0,0,0,0,
0,0,0,0.5,1,0.5,0,0,0,0,0,0,0,
0,0,0,0,0,0.5,1,0.5,0,0,0,0,0,
0,0,0,0,0,0,0,0.5,1,0.5,0,0,0,
0,0,0,0,0,0,0,0,0,0.5,1,0.5,0,
0,0,0,0,0,0,0,0,0,0,0,0.5,1
};
/***************************************输出（速度）***************************************/
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
* 横轴为E(error)，纵轴为EC(error_delta)，值为速度七档NB(0),NM(1),NS(2),Z(3),PS(4),PM(5),PB(6)速度由小变大
*全0，1，2，3，4，5，6对应模糊查询表全-6，-4，-2，0，2，4，6
* @param 模糊控制规则表，调整速度变化趋势
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

///<float SPEED[13]={200,220,230,240,250,270,300,270,250,240,230,220,200};//调车
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
* 纵轴为E(error)，横轴为EC(error_delta)，语言值为速度七档NB(0),NM(1),NS(2),Z(3),PS(4),PM(5),PB(6)速度由小变大
* @param 模糊化过程实现论域内不同值对应隶属度最大的语言值
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
	/***************************************计算所有规则模糊关系的并集Rule***************************************/
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
			///<R1=AXB建立13x13的矩阵,R2=R1'把矩阵转成169x1的列向量
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
	
	
	/*************************对于每种可能的E、EC的精确取值模糊化后进行推理得到模糊输出Cd*************************/
	for(Input1_value_index=0;Input1_value_index<13;Input1_value_index++)
	{
		for(Input2_value_index=0;Input2_value_index<13;Input2_value_index++)
		{
			for(j=0;j<13;j++)
				Cd[j]=0;
			int kd=0;	
			float temp=0;
			Max_Input1_value=E_MAX(Input1_value_index); ///<找出误差隶属度最大的语言值
			Max_Input2_value=EC_MAX(Input2_value_index);///<找出误差变化率隶属度最大的语言值
			for(i=0;i<13;i++)
				for(j=0;j<13;j++)
				{
					if(Input1_Terms_Membership[Max_Input1_value][i]<Input2_Terms_Membership[Max_Input2_value][j])
						AdBd1[i][j]=Input1_Terms_Membership[Max_Input1_value][i];
					else
						AdBd1[i][j]=Input2_Terms_Membership[Max_Input2_value][j];
					AdBd2[kd]=AdBd1[i][j];
					kd++;
				}///<Rd1＝Ad×Bd,Rd2＝Rd1'
			for(j=0;j<13;j++)
				for (i = 0; i<169; i++)
				{
					if(AdBd2[i]<R[i][j])
						temp=AdBd2[i];
					else
						temp=R[i][j];
					if(temp>Cd[j])
						Cd[j]=temp;
				}///<Cd＝Rd2oR

			/*************************去模糊化（加权平均法），计算实际输出*************************/
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
				OUT = (int)(sum2 / sum1 - 0.5);///<四舍五入
			Fuzzy_Table[Input1_value_index][Input2_value_index]=OUT;
			Speed_Table[Input1_value_index][Input2_value_index] = ku*OUT + (UHLimit + ULLimit) / 2;///<把模糊表转为实际速度表
		}
	}
}

void main()
{
	float ek;
	float eck;//计算第k个采样周期的误差和误差变化率
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
	E = ke*(ek - (EHLimit + ELLimit) / 2)+0.5;//将E的范围转换到模糊控制器的论域
	if(E>6)
		E=6;
	else if(E<-6)
		E=-6;
	EC = kec*(eck - (ECHLimit + ECLLimit) / 2) + 0.5; //将EC的范围转换到模糊控制器的论域
	if(EC>6)
		EC=6;
	else if(EC<-6)
		EC=-6;
	U=Fuzzy_Table[E+6][EC+6]; //查模糊控制查询表得到输出值U
	u=Speed_Table[E+6][EC+6]; //查模糊控制查询表得到输出值u
	u=ku*U+(UHLimit + ULLimit)/2; //将输出转换到实际论域
	cout<<U;
	cout << endl;
	cout << u;
	cout << endl;
	*/
}



