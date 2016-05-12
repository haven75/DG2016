/*
 * Beep.h
 *
 *  Created on: May 7, 2016
 *      Author: Quan
 */

#ifndef BEEP_H_
#define BEEP_H_

#define Beep SIU.GPDO[43].R

void Beep_ON(void);
void Beep_OFF(void);

#endif /* BEEP_H_ */
