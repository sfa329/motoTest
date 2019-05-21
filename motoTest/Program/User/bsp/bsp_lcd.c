/*
*********************************************************************************************************
*	                                  
*	模块名称 : LED数码管驱动模块
*	文件名称 : bsp_lcd.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-06-26 zhg     创建该文件
*
*********************************************************************************************************
*/

#include "stm8s.h"
#include <stdio.h>

#include "bsp_lcd.h"

#define LCD_PWR_PORT	GPIOD			/* LCD电源端口 */
#define LCD_PWR_PIN 	GPIO_PIN_0		/* LCD电源bit */

#define LCD_CLK_PORT	GPIOD			/* LCD时钟端口 */
#define LCD_CLK_PIN 	GPIO_PIN_2		/* LCD时钟bit */

#define LCD_DATA_PORT	GPIOD			/* LCD数据端口 */
#define LCD_DATA_PIN 	GPIO_PIN_3		/* LCD数据bit */



#define LED8_ON()		GPIO_WriteLow(LCD_PWR_PORT, (GPIO_Pin_TypeDef)LCD_PWR_PIN)
#define LED8_OFF()		GPIO_WriteHigh(LCD_PWR_PORT, (GPIO_Pin_TypeDef)LCD_PWR_PIN)

#define LED_SDA_1()		GPIO_WriteHigh(LCD_DATA_PORT, (GPIO_Pin_TypeDef)LCD_DATA_PIN)
#define LED_SDA_0()		GPIO_WriteLow(LCD_DATA_PORT, (GPIO_Pin_TypeDef)LCD_DATA_PIN)	

#define LED_CLK_1()		GPIO_WriteHigh(LCD_CLK_PORT, (GPIO_Pin_TypeDef)LCD_CLK_PIN)
#define LED_CLK_0()		GPIO_WriteLow(LCD_CLK_PORT, (GPIO_Pin_TypeDef)LCD_CLK_PIN)	

/*
	共阳数码管0-3和的16进制代码
	从ASCII字符空格开始 ‘ ’

;-----------------------------        
;         d6d6d6d6
;        d4      d7
;        d4      d7
;        d4      d7
;         d5d5d5d5
;        d3      d0
;        d3      d0
;        d3      d0
;         d2d2d2d2      d1        
;------------------------------
	
*/
#define CHAR_SUB	0xDF		/* '-' 的显示代码 */
#define CHAR_F		0x87		/* 'F' */
#define CHAR_A		0x06		/* 'A' */
#define CHAR_n		0xD6		/* 'n' */
#define CHAR_S		0x8A		/* 'S' */
#define CHAR_E		0x83		/* 'E' */
#define CHAR_O		0x22		/* 'O' */
#define CHAR_o		0xD2		/* 'o' */
#define CHAR_d		0x52		/* 'd' */
#define CHAR_P		0x07		/* 'P' */
#define CHAR_G		0xA2	        /* 'G' */
#define CHAR_r		0xD7	        /* 'r' */
#define CHAR_C      0xA3            // C
#define CHAR_v      0xF2            // v
#define CHAR_L      0xE3            // L	
#define CHAR_B      0xC2            // b	
#define CHAR_H      0x46            // H	
#define CHAR_U      0xF2            // u	
#define CHAR_t      0XC3
#define CHAR_SPACE	0xFF		/* 不显示 */
#define DOT_MASK 	0xFD		/* 小数点MASK */

/* 0-9,A-F 数字的显示代码 */
const uint8_t g_NumberCode[] = 
{
	0x22,	// 0	
	0x7E,	// 1
	0x13,	// 2	
	0x1A,	// 3	
	0x4E,	// 4	
	0x8A,	// 5	
	0x82,	// 6	
	0x3E,	// 7	
	0x02,	// 8	
	0x0A,	// 9	
	
	0x06,	// A	
	0xC2,	// B	
	0xA3,	// C	
	0x52,	// D	
	0x83,	// E	
	0x87	// F	
} 
;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 初始化LED指示灯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLcd(void)
{
	/* 初始化LED数码管的3个GPIO，全部配置为推挽输出 */
	GPIO_Init(LCD_PWR_PORT, (GPIO_Pin_TypeDef)LCD_PWR_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LCD_CLK_PORT, (GPIO_Pin_TypeDef)LCD_CLK_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(LCD_DATA_PORT, (GPIO_Pin_TypeDef)LCD_DATA_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
      
	
}

/*
*********************************************************************************************************
*	函 数 名: lcd_SendData
*	功能说明: 8位数据从74HC164的串行输入端输入，从输出端并行输出
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void lcd_SendData(uint8_t dd) 
{        
	uint8_t i;
	
	for (i = 0; i < 8; i++)
	{
		if (dd & 0x80)
		{
			LED_SDA_1();
		}
		else
		{
			LED_SDA_0();
		}
		
		LED_CLK_1();
		dd <<= 1;
		LED_CLK_0();		
	}
}

/*
*********************************************************************************************************
*	函 数 名: DispNumber1
*	功能说明: 显示10进制整数
*				  dot   : 0 最高位显示 L，1：最高位显示 H,  如果数字> 99, 则最高位显示数字
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispNumber1(uint16_t value, uint8_t dot)
{
	 uint8_t num1,num2,num3;
     
	 LED8_OFF();
       
        if (value > 1000)
        { 
         
          /* 16进制显示 */
         num1 = value & 0x0f;               //   最低位
         num2 = (value >> 4) & 0x0f;        //   
         num3 = (value >> 8) & 0x0f;        //   
         
        }
        else
        {  
           /*  10进制显示 */
             num1 = value % 10;             //   个  
	     num2 = (value % 100) / 10;     //   十
	     num3 = (value / 100)%10;       //   百
	          
         }
         if(value==1000)
         {
           lcd_SendData(g_NumberCode[9]);               //   9
            lcd_SendData(g_NumberCode[9]);              //   9
             lcd_SendData(g_NumberCode[9]);             //   9
             
         }
        
        else
        {      
          if(dot==1)
                 {
                   lcd_SendData(g_NumberCode[num1]);               //   个 
		   if(  num2==0&&num3==0)
                   lcd_SendData(CHAR_SPACE);
                   else
                   lcd_SendData(g_NumberCode[num2]);             //   十 
                   if( num3==0)
                   lcd_SendData(CHAR_SPACE);
                   else
                   lcd_SendData(g_NumberCode[num3]);           //   百
                 }
		 if(dot==2)
                 {
                   lcd_SendData(g_NumberCode[num1]);              //   个
                   lcd_SendData(g_NumberCode[num2]&DOT_MASK);     //   十  
                                    
		  if(num3==0)
                   
                   lcd_SendData(CHAR_SPACE); 
                  else
		  lcd_SendData(g_NumberCode[num3]);             //   百
                 }
        
        }
   
			             
     LED8_ON();        
     LED8_OFF();       
     LED8_ON();
	 
} 

	

/*
*********************************************************************************************************
*	函 数 名: DispNumber
*	功能说明: 显示10进制整数
*	形    参：value : 16bit整数，最大999
*			  dot   : 0 不显示小数点，1:十位上显示小数点, 2:个,十,百，位都显示小数点，3:个位显示小数点
*	返 回 值: 无
*********************************************************************************************************


*/
void DispNumber(uint16_t value,uint8_t dot)
{
	DispNumber1(value, dot);
}
/*************************************/
void DispNumber2(uint16_t value)
{
 
     DispNumber1(value/10, 1);  
   
			             
    
	 
} 


/*
*********************************************************************************************************
*	函 数 名: DispNumberWithDot
*	功能说明: 显示10进制整数, 带小数点
*	形    参：value : 16bit整数，最大999
*	返 回 值: 无
*********************************************************************************************************
*/
void DispNumberWithDot(uint16_t value)
{
	DispNumber1(value, 1);
}

/*
*********************************************************************************************************
*	函 数 名: DispNumberWith3Dot
*	功能说明: 显示10进制整数, 带3小数点
*	形    参：value : 16bit整数，最大999
*	返 回 值: 无
*********************************************************************************************************
*/
void DispNumberWith3Dot(uint16_t value)
{
	DispNumber1(value, 2);
}

/********************************************************************************************************/

void DispNumberWith1Dot(uint16_t value)  // 最低位带点
{
	DispNumber1(value, 3);
}


/*********************************************************************************************************
*	函 数 名: DispLOV
*	功能说明: 显示"ON"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void DispLOV(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_v);   
	lcd_SendData(CHAR_o);	
	lcd_SendData(CHAR_L);   	
	LED8_ON();
}

/*********************************************************************************************************
*	函 数 名: DispLOV
*	功能说明: 显示"ON"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void DispLOVIGBT(void)
{
	LED8_OFF();
        lcd_SendData(CHAR_C);   
	lcd_SendData(CHAR_v);   
	lcd_SendData(CHAR_o);	
		
	LED8_ON();
}


/*********************************************************************************************************
*	函 数 名: DispFan
*	功能说明: 显示"FAN"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/


void DispFan(uint8_t bz)

{
	
        LED8_OFF();
        if(bz==1)
        {
	  lcd_SendData(CHAR_n & DOT_MASK);
        }
	else
        {
          lcd_SendData(CHAR_n);
        }
        
        lcd_SendData(CHAR_A);	              
        lcd_SendData(CHAR_F);	
	LED8_ON();
}
/*
*********************************************************************************************************
*	函 数 名: DispFod
*	功能说明: 显示"Fod"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispPGA(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_A);
	lcd_SendData(CHAR_G);
	lcd_SendData(CHAR_P);	
	LED8_ON();
}
/*
*********************************************************************************************************
*	函 数 名: DispFod
*	功能说明: 显示"Fod"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispFod(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_d);
	lcd_SendData(CHAR_o);	
	lcd_SendData(CHAR_F);	
	LED8_ON();
}

/*
*********************************************************************************************************
*	函 数 名: DispON
*	功能说明: 显示"ON"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispON(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_n);
	lcd_SendData(CHAR_o);	
	lcd_SendData(CHAR_SPACE);	
	LED8_ON();
}
/*
*********************************************************************************************************
*	函 数 名: DispSen
*	功能说明: 显示"Sen"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispSen(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_n);
	lcd_SendData(CHAR_E);
	lcd_SendData(CHAR_S);	
	LED8_ON();
}

/*

*********************************************************************************************************
*	函 数 名: DispnFd
*	功能说明: 显示"nFd"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispnFd(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_d);
	lcd_SendData(CHAR_F);
	lcd_SendData(CHAR_n);	
	LED8_ON();
}

/*
*********************************************************************************************************
*	函 数 名: DispOFF
*	功能说明: 显示"OFF"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispOFF(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_F);
	lcd_SendData(CHAR_F);	
	lcd_SendData(CHAR_o);
	LED8_ON();	
}

/*
*********************************************************************************************************
*	函 数 名: DispSUB
*	功能说明: 显示"---"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispSUB(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_SUB);
	lcd_SendData(CHAR_SUB);	
	lcd_SendData(CHAR_SUB);
	LED8_ON();	
}
/*
*********************************************************************************************************
*	函 数 名: DispENd
*	功能说明: 显示"End"
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DispEnd(void)
{
	LED8_OFF();
	lcd_SendData(CHAR_d);
	lcd_SendData(CHAR_n);	
	lcd_SendData(CHAR_E);
	LED8_ON();	
}

/*
*********************************************************************************************************
*	函 数 名: DispErr
*	功能说明: 显示"E00--EFF
*	形    参：num=0x00--0Xff
*	返 回 值: 无
*********************************************************************************************************
*/                                              //  显示E01,    标定数据错
                                                //  显示E02  ， DC24 伏 A/D 偏大  
                                                //  显示E03  ， DC24 伏 A/D 偏小 
void DispErr(uint8_t num)                      //   显示E04 ,   表示DS18B20芯片复位失败
{       
       uint8_t value;
  
        LED8_OFF();
        value = num % 16;
        lcd_SendData(g_NumberCode[value]); 
        
        lcd_SendData(g_NumberCode[value]);
        value =num/16;
        lcd_SendData(g_NumberCode[value]);
	
	lcd_SendData(CHAR_E);
	LED8_ON();	
}
void Disprun(void)
{
  
	LED8_OFF();
        lcd_SendData(CHAR_SPACE);
	lcd_SendData(CHAR_SPACE);
	lcd_SendData(CHAR_n);	
	
	lcd_SendData(CHAR_v);
	lcd_SendData(CHAR_r);
	
	
    	
	LED8_ON();	
    
}


void DispREV(uint8_t num)

{
        LED8_OFF();
        lcd_SendData(CHAR_SPACE);
	lcd_SendData(CHAR_SPACE);
        lcd_SendData(g_NumberCode[num]);
	lcd_SendData(CHAR_SPACE);
	lcd_SendData(CHAR_A);
    	LED8_ON();	
    
}

void Disp(uint8_t num)
{
	LED8_OFF();   
	lcd_SendData(CHAR_SPACE);
	if(num==0)	
	lcd_SendData(CHAR_A);	
	if(num==1)
	lcd_SendData(CHAR_B); 
	if(num==2)
	lcd_SendData(CHAR_C);
	if(num==3)
	lcd_SendData(CHAR_d);
	
	lcd_SendData(CHAR_P);
	LED8_ON();	
}

void Disp___(void)
{
	LED8_OFF();
    lcd_SendData(0xFF);
        lcd_SendData(0xFD);
        lcd_SendData(0xFF);
        lcd_SendData(0xFF);
        
	
    LED8_ON();	
        
}
void DispAuto(void)
{
  
  		
	
	LED8_OFF();
	lcd_SendData(CHAR_o);
	lcd_SendData(CHAR_t);
	lcd_SendData(CHAR_v);	
	lcd_SendData(CHAR_A);
	LED8_ON();
}

void DispnFAL(void)  
{
	LED8_OFF();
	lcd_SendData(CHAR_L);
	lcd_SendData(CHAR_A);
	lcd_SendData(CHAR_F);	
	LED8_ON();
}

void DispnCUR(void)  
{
	LED8_OFF();
	
	lcd_SendData(CHAR_r);
        lcd_SendData(CHAR_v);
	lcd_SendData(CHAR_C);	
	LED8_ON();
}

void DispnVOL(void)  
{
	LED8_OFF();
	
	lcd_SendData(CHAR_L);
        lcd_SendData(CHAR_o);
	lcd_SendData(CHAR_v);	
	LED8_ON();
}
void DispnEEE(void)  
{
	LED8_OFF();
	
	lcd_SendData(CHAR_E);
        lcd_SendData(CHAR_E);
	lcd_SendData(CHAR_E);
	LED8_ON();
}
