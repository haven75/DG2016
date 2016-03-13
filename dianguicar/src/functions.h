/*
 * functions.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

void frequency_measure();
void InitspPID();
void InitsePID();
void position();
void GETservoPID();
signed int LocPIDCal();
void sensor_display();
unsigned int Get_speed();

void SAIC1_inter();
//void SAIC2_inter(void);
void Set_Middlepoint();
void SendHex(unsigned char hex);
void Senddata();

#endif /* FUNCTIONS_H_ */
