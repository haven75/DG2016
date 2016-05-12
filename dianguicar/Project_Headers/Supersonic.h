/*
 * Supersonic.h
 *
 *  Created on: May 7, 2016
 *      Author: Quan
 */

#ifndef SUPERSONIC_H_
#define SUPERSONIC_H_

#define Trig SIU.GPDO[58].R

void Supersonic_Echo(void);
void Supersonic_Trig(void);
float Supersonic_Distance(void);

extern unsigned char Supersonic_flag;
extern unsigned int Supersonic_Time;

#endif /* SUPERSONIC_H_ */
