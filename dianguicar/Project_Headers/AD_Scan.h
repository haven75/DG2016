/*
 * AD_Scan.h
 *
 *  Created on: May 8, 2016
 *      Author: Quan
 */

#ifndef AD_SCAN_H_
#define AD_SCAN_H_

unsigned int ADread(unsigned int channel);
void GET_AD_Channel(void);

extern unsigned int tempAD[2];

#endif /* AD_SCAN_H_ */
