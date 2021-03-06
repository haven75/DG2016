/*
 * define.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#ifndef DEFINE_H_
#define DEFINE_H_

typedef unsigned short WORD;
typedef unsigned char byte;

/****************************************************************set_speed***********************************************/



typedef struct PID
{ 
	double SetPoint;
	long SumError;
	
	double Proportion;
	double Integral;
	double Derivative;
	
	int LastError;
	int PrevError;
}PID;

static PID spPID,sePID;
static PID *sp=&spPID,*se=&sePID;

#endif /* DEFINE_H_ */
