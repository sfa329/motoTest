/*
*********************************************************************************************************
*	                                  
*	ģ������ : LED���������ģ��
*	�ļ����� : bsp_lcd.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2011-06-26 zhg     �������ļ�
*
*********************************************************************************************************
*/

#include "stm8s.h"
#include <stdio.h>

#include "bsp_lcd.h"

#define LCD_PWR_PORT	GPIOD			/* LCD��Դ�˿� */
#define LCD_PWR_PIN 	GPIO_PIN_0		/* LCD��Դbit */

#define LCD_CLK_PORT	GPIOD			/* LCDʱ�Ӷ˿� */
#define LCD_CLK_PIN 	GPIO_PIN_2		/* LCDʱ��bit */

#define LCD_DATA_PORT	GPIOD			/* LCD���ݶ˿� */
#define LCD_DATA_PIN 	GPIO_PIN_3		/* LCD����bit */



#define LED8_ON()		GPIO_WriteLow(LCD_PWR_PORT, (GPIO_Pin_TypeDef)LCD_PWR_PIN)
#define LED8_OFF()		GPIO_WriteHigh(LCD_PWR_PORT, (GPIO_Pin_TypeDef)LCD_PWR_PIN)

#define LED_SDA_1()		GPIO_WriteHigh(LCD_DATA_PORT, (GPIO_Pin_TypeDef)LCD_DATA_PIN)
#define LED_SDA_0()		GPIO_WriteLow(LCD_DATA_PORT, (GPIO_Pin_TypeDef)LCD_DATA_PIN)	

#define LED_CLK_1()		GPIO_WriteHigh(LCD_CLK_PORT, (GPIO_Pin_TypeDef)LCD_CLK_PIN)
#define LED_CLK_0()		GPIO_WriteLow(LCD_CLK_PORT, (GPIO_Pin_TypeDef)LCD_CLK_PIN)	

/*
	���������0-3�͵�16���ƴ���
	��ASCII�ַ��ո�ʼ �� ��

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
#define CHAR_SUB	0xDF		/* '-' ����ʾ���� */
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
#define CHAR_SPACE	0xFF		/* ����ʾ */
#define DOT_MASK 	0xFD		/* С����MASK */

/* 0-9,A-F ���ֵ���ʾ���� */
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
*	�� �� ��: bsp_InitLed
*	����˵��: ��ʼ��LEDָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLcd(void)
{
	/* ��ʼ��LED����ܵ�3��GPIO��ȫ������Ϊ������� */
	GPIO_Init(LCD_PWR_PORT, (GPIO_Pin_TypeDef)LCD_PWR_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LCD_CLK_PORT, (GPIO_Pin_TypeDef)LCD_CLK_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(LCD_DATA_PORT, (GPIO_Pin_TypeDef)LCD_DATA_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
      
	
}

/*
*********************************************************************************************************
*	�� �� ��: lcd_SendData
*	����˵��: 8λ���ݴ�74HC164�Ĵ�����������룬������˲������
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispNumber1
*	����˵��: ��ʾ10��������
*				  dot   : 0 ���λ��ʾ L��1�����λ��ʾ H,  �������> 99, �����λ��ʾ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispNumber1(uint16_t value, uint8_t dot)
{
	 uint8_t num1,num2,num3;
     
	 LED8_OFF();
       
        if (value > 1000)
        { 
         
          /* 16������ʾ */
         num1 = value & 0x0f;               //   ���λ
         num2 = (value >> 4) & 0x0f;        //   
         num3 = (value >> 8) & 0x0f;        //   
         
        }
        else
        {  
           /*  10������ʾ */
             num1 = value % 10;             //   ��  
	     num2 = (value % 100) / 10;     //   ʮ
	     num3 = (value / 100)%10;       //   ��
	          
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
                   lcd_SendData(g_NumberCode[num1]);               //   �� 
		   if(  num2==0&&num3==0)
                   lcd_SendData(CHAR_SPACE);
                   else
                   lcd_SendData(g_NumberCode[num2]);             //   ʮ 
                   if( num3==0)
                   lcd_SendData(CHAR_SPACE);
                   else
                   lcd_SendData(g_NumberCode[num3]);           //   ��
                 }
		 if(dot==2)
                 {
                   lcd_SendData(g_NumberCode[num1]);              //   ��
                   lcd_SendData(g_NumberCode[num2]&DOT_MASK);     //   ʮ  
                                    
		  if(num3==0)
                   
                   lcd_SendData(CHAR_SPACE); 
                  else
		  lcd_SendData(g_NumberCode[num3]);             //   ��
                 }
        
        }
   
			             
     LED8_ON();        
     LED8_OFF();       
     LED8_ON();
	 
} 

	

/*
*********************************************************************************************************
*	�� �� ��: DispNumber
*	����˵��: ��ʾ10��������
*	��    �Σ�value : 16bit���������999
*			  dot   : 0 ����ʾС���㣬1:ʮλ����ʾС����, 2:��,ʮ,�٣�λ����ʾС���㣬3:��λ��ʾС����
*	�� �� ֵ: ��
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
*	�� �� ��: DispNumberWithDot
*	����˵��: ��ʾ10��������, ��С����
*	��    �Σ�value : 16bit���������999
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DispNumberWithDot(uint16_t value)
{
	DispNumber1(value, 1);
}

/*
*********************************************************************************************************
*	�� �� ��: DispNumberWith3Dot
*	����˵��: ��ʾ10��������, ��3С����
*	��    �Σ�value : 16bit���������999
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DispNumberWith3Dot(uint16_t value)
{
	DispNumber1(value, 2);
}

/********************************************************************************************************/

void DispNumberWith1Dot(uint16_t value)  // ���λ����
{
	DispNumber1(value, 3);
}


/*********************************************************************************************************
*	�� �� ��: DispLOV
*	����˵��: ��ʾ"ON"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispLOV
*	����˵��: ��ʾ"ON"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispFan
*	����˵��: ��ʾ"FAN"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispFod
*	����˵��: ��ʾ"Fod"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispFod
*	����˵��: ��ʾ"Fod"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispON
*	����˵��: ��ʾ"ON"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispSen
*	����˵��: ��ʾ"Sen"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispnFd
*	����˵��: ��ʾ"nFd"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispOFF
*	����˵��: ��ʾ"OFF"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispSUB
*	����˵��: ��ʾ"---"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispENd
*	����˵��: ��ʾ"End"
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: DispErr
*	����˵��: ��ʾ"E00--EFF
*	��    �Σ�num=0x00--0Xff
*	�� �� ֵ: ��
*********************************************************************************************************
*/                                              //  ��ʾE01,    �궨���ݴ�
                                                //  ��ʾE02  �� DC24 �� A/D ƫ��  
                                                //  ��ʾE03  �� DC24 �� A/D ƫС 
void DispErr(uint8_t num)                      //   ��ʾE04 ,   ��ʾDS18B20оƬ��λʧ��
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
