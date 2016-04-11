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
#define Strait 95
#define Littleround 88
#define LittleSround 90
#define Biground 85
#define Uround 80


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
