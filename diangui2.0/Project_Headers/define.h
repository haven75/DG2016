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
<<<<<<< HEAD
#define Strait1 60
#define Strait2 55
#define Curve1 40
#define Curve2 40
#define BigCurve 40
#define Round 40
=======
#define Strait 150
#define Littleround 100
#define LittleSround 120
#define Biground 130
#define Uround 70
>>>>>>> master


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
