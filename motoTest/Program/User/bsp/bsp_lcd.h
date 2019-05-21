/*
*********************************************************************************************************
*	                                  
*	模块名称 : LED数码管驱动模块    
*	文件名称 : bsp_lcd.h
*	版    本 : V1.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-01-11 zhg     首版
*
*********************************************************************************************************
*/

#ifndef __BSP_LCD_H
#define __BSP_LCD_H

/* 供外部调用的函数声明 */
void bsp_InitLcd(void);

void DispNumber(uint16_t value,uint8_t dot);
void DispNumberWithDot(uint16_t value);
void DispNumberWith3Dot(uint16_t value);
void DispLOV(void);
void DispFan(uint8_t bz);
void DispSen(void);
void DispOFF(void);
void DispSUB(void);
void DispFod(void);
void DispON(void);
void DispPGA(void);
void DispEnd(void);
void DispNumberWith1Dot(uint16_t value);  // 最低位带点
void DispnFd(void);
void DispErr(uint8_t num);
void DispnFAL(void);
void DispLOVIGBT(void);
void DispnVOL(void);
void DispnCUR(void);
void DispnEEE(void);
void Disp___(void);
void Disprun(void);
void DispNumber2(uint16_t value);
void DispNumberWithDot(uint16_t);
void DispAuto(void);
void Disp(uint8_t num);

void DispREV(uint8_t num);
#endif


