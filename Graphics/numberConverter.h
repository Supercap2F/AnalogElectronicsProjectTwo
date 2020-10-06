/* 
 * File:   numberConverter.h
 * Author: super
 *
 * Created on October 6, 2020, 11:30 AM
 */

#ifndef NUMBERCONVERTER_H
#define	NUMBERCONVERTER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void ftoa(float n, char* res, int afterpoint);
unsigned int intToStr(unsigned int x, char str[], int d);
void reverse(char* str, int len);
unsigned int reverseBits(unsigned int num);


#ifdef	__cplusplus
}
#endif

#endif	/* NUMBERCONVERTER_H */

