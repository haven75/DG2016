/*
 * functions.h
 *
 *  Created on: Feb 27, 2016
 *      Author: Administrator
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void frequency_measure();
void InitspPID();
void InitsePID();
void position();
void GETservoPID();
signed int LocPIDCal();
void SpeedSet();
void sensor_display();
unsigned int abs(signed int x);
void Get_speed();

void SAIC1_inter(void);
//void SAIC2_inter(void);
void Set_Middlepoint();
void SendHex(unsigned char hex);
void Senddata();
void speed_set();
void speed_control();

#endif /* FUNCTIONS_H_ */
