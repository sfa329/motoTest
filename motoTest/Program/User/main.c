//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//电机检测工装
//用途：用于电调控制的电机速度检测
//说明：上电检测 PWM_IN脚
//      0：速度读取 & 速度给定 双程序
//      1：速度给定
//作者：lishengfa
//微信：listen329
//版本：V1.0
//日期：2019-05-19
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================

//==============================================================================
#include "stm8s.h"
#include "bsp.h"
#include "stm8s_it.h"

//==============================================================================
#define   GIVE          0
#define   GET           1


#define     MAXDUTY     12
unsigned short DUTYSHOW[MAXDUTY]={0  ,1   ,10  ,20  ,30  ,40  ,50  ,60  ,70  ,80  ,90  ,100 };
unsigned short DUTYTIME[MAXDUTY]={900,1050,1145,1240,1335,1430,1525,1620,1715,1810,1905,2000};

//------------------------------------------------------------------------------
#define KEY_INC_PORT	GPIOB
#define KEY_INC_PIN	GPIO_PIN_7

#define KEY_DEC_PORT	GPIOB
#define KEY_DEC_PIN	GPIO_PIN_5

#define KEY_FOOD_PORT	GPIOB
#define KEY_FOOD_PIN	GPIO_PIN_6

#define KEY_PWR_PORT	GPIOB
#define KEY_PWR_PIN	GPIO_PIN_4

#define FLAG_PWR        0x01
#define FLAG_FOOD       0x02
#define FLAG_INC        0x04
#define FLAG_DEC        0x08

#define KEY_60MS        3
#define KEY_1300MS      65
#define KEY_1500MS      75

//------------------------------------------------------------------------------
#define PWM_OUT_PORT	GPIOA
#define PWM_OUT_PIN	GPIO_PIN_3

//------------------------------------------------------------------------------
#define PWM_IN_PORT	GPIOE
#define PWM_IN_PIN	GPIO_PIN_5

//------------------------------------------------------------------------------
#define BEEP_PORT	GPIOD
#define BEEP_PIN	GPIO_PIN_4

//------------------------------------------------------------------------------
#define AIR_PORT	GPIOA
#define AIR_PIN	        GPIO_PIN_4

//==============================================================================
unsigned char type;

unsigned char keyFlag;

unsigned char timePwr;
unsigned char timeFood;
unsigned char timeInc;
unsigned char timeDec;

unsigned short pwmDuty=900;
unsigned short tmpDuty=900;
unsigned char timeShow;

unsigned char timeSecondFlag;
unsigned short speedSecond;
unsigned short speedMinute;


//==============================================================================
//判断是速度给定 还是 速度读取
//==============================================================================
void check_InOut(void)
{
  unsigned char i;
  
  GPIO_Init(AIR_PORT, (GPIO_Pin_TypeDef)AIR_PIN, GPIO_MODE_IN_PU_NO_IT);
  for(i=0;i<100;i++);
  if(GPIO_ReadInputPin(AIR_PORT, (GPIO_Pin_TypeDef)AIR_PIN))
    type = GET;
  else
    type = GIVE;
}

//==============================================================================
//时钟初始化--16MHZ
//==============================================================================
void init_Clk(void)
{
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

//==============================================================================
//按键初始化
//按键口均设置为上拉无中断输入
//==============================================================================
void init_Button(void)
{
  GPIO_Init(KEY_INC_PORT, (GPIO_Pin_TypeDef)KEY_INC_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY_DEC_PORT, (GPIO_Pin_TypeDef)KEY_DEC_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY_FOOD_PORT, (GPIO_Pin_TypeDef)KEY_FOOD_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY_PWR_PORT, (GPIO_Pin_TypeDef)KEY_PWR_PIN, GPIO_MODE_IN_PU_NO_IT);
}

//------------------------------------------------------------------------------
//获取按键信息
//20ms定时中断中调用
//短按---60ms
//长按---1500ms
//------------------------------------------------------------------------------
void get_Button(void)
{
  if(GPIO_ReadInputPin(KEY_INC_PORT,(GPIO_Pin_TypeDef)KEY_INC_PIN)==0)
  {
    if(++timeInc>=KEY_1500MS)
      timeInc = KEY_1300MS;
    
    if( (timeInc==KEY_60MS) || (timeInc==KEY_1300MS) )
      keyFlag |= FLAG_INC;
  }
  else
    timeInc=0;
  
  if(GPIO_ReadInputPin(KEY_DEC_PORT,(GPIO_Pin_TypeDef)KEY_DEC_PIN)==0)
  {
    if(++timeDec>=KEY_1500MS)
      timeDec = KEY_1300MS;
    
    if( (timeDec==KEY_60MS) || (timeDec==KEY_1300MS) )
      keyFlag |= FLAG_DEC;
  }
  else
    timeDec=0;

  if(GPIO_ReadInputPin(KEY_FOOD_PORT,(GPIO_Pin_TypeDef)KEY_FOOD_PIN)==0)
  {
    if(++timeFood>=KEY_1500MS)
      timeFood = KEY_1300MS;
    
    if( (timeFood==KEY_60MS) || (timeFood==KEY_1300MS) )
      keyFlag |= FLAG_FOOD;
  }
  else
    timeFood=0;

  if(GPIO_ReadInputPin(KEY_PWR_PORT,(GPIO_Pin_TypeDef)KEY_PWR_PIN)==0)
  {
    if(++timePwr>=KEY_1500MS)
      timePwr = KEY_1300MS;
    
    if( (timePwr==KEY_60MS) || (timePwr==KEY_1300MS) )
      keyFlag |= FLAG_PWR;
  }
  else
    timePwr=0;
}

//==============================================================================
//速度给定--PWM输出脚设置
//PWM波形：周期-20ms 占空比:900us~2000us
//==============================================================================
void init_PwmOut(void)
{
  GPIO_Init(PWM_OUT_PORT, (GPIO_Pin_TypeDef)PWM_OUT_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

  TIM2_DeInit(); 
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 20000);
  TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 900, TIM2_OCPOLARITY_HIGH);
  
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
  TIM2_Cmd(ENABLE);
}

//------------------------------------------------------------------------------
//20ms定时中断中调用
//设置占空比
//------------------------------------------------------------------------------
void set_PwmOut(void)
{  
  static unsigned char flag;
  static unsigned short dutyTime;
  unsigned short tmp;

  if(pwmDuty<=900)
    flag = 0;
  else if(pwmDuty<=1500)
  {
    if(++flag>=100)
      flag=100;
  }
  else
    flag = 100;
    
  if( (flag>0)&&(flag<100) )
    tmp=1500;
  else
    tmp=pwmDuty;
  
  if(dutyTime!=tmp)
  {
    dutyTime = tmp;
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, tmp, TIM2_OCPOLARITY_HIGH);
  }
}

//==============================================================================
//==============================================================================
unsigned char beepFlag;

void init_Beep(void)
{
  GPIO_Init(BEEP_PORT, (GPIO_Pin_TypeDef)BEEP_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOA, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);

  TIM4_DeInit(); 
  TIM4_TimeBaseInit(TIM4_PRESCALER_64, 250);
  
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);
  TIM4_Cmd(ENABLE);
}

//==============================================================================
//按键-设置占空比-控制速度
//加上OFF 共12档
//==============================================================================
void key_ToGiveSpeed(void)
{
  static unsigned char i=0;
  static unsigned char tmp=0;
  
  if( keyFlag&FLAG_INC )
  {
    keyFlag &= ~FLAG_INC;
    
    if(++i>=MAXDUTY)
      i = MAXDUTY-1;
  }
  
  if( keyFlag&FLAG_DEC )
  {
    keyFlag &= ~FLAG_DEC;
    
    if(i>1)
      i--;
  }
  
  if( keyFlag&FLAG_FOOD )
  {
    keyFlag &= ~FLAG_FOOD;
    
    i = MAXDUTY-1;
  }
  
  if( keyFlag&FLAG_PWR )
  {
    keyFlag &= ~FLAG_PWR;
    
    i = 0;
  }
  
  if( tmp != i )
  {
    tmp = i;

    pwmDuty = DUTYTIME[i];
    tmpDuty = DUTYTIME[i];
    
    if(i==0)
      DispOFF();
    else
      DispNumber(DUTYSHOW[i],1);
  }
}

//==============================================================================
//速度读取
//设置IO口上升下降沿中断
//==============================================================================
void init_PwmIn(void)
{
  GPIO_Init(PWM_IN_PORT, (GPIO_Pin_TypeDef)PWM_IN_PIN, GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOE,EXTI_SENSITIVITY_RISE_FALL);
  
  //使能1s定时中断
  TIM3_DeInit();
  TIM3_TimeBaseInit(TIM3_PRESCALER_256,62500);  //每计一次数为16us
  
  TIM3_ClearFlag(TIM3_FLAG_UPDATE);
  TIM3_ITConfig(TIM3_IT_UPDATE,ENABLE);
  TIM3_Cmd(ENABLE);
}

//==============================================================================
//根据1s内的脉冲数计算转速
//取2次的平均值
//==============================================================================
void time_ToGetSpeed(void)
{
#define  MAX_LOSE_TIME          2
#define  MAX_SPD_MIN            3
  
  static unsigned char flag=0;
  static unsigned short tmp[MAX_SPD_MIN];
  static unsigned char j=0;
  unsigned short sum;
  unsigned char m;

  if(timeSecondFlag<1)
    return;
  timeSecondFlag=0;

  if( (speedMinute<450)&&(tmpDuty>900) )
  {
    if(flag==0)    pwmDuty=900;
    else           pwmDuty=tmpDuty;
  
    if(++flag>=MAX_LOSE_TIME)
      flag = 0;
  }
  else
  {
    flag = 1;

    if(++j>=MAX_SPD_MIN)  j=0;
    tmp[j] = speedMinute;
    
    for(sum=0,m=0;m<MAX_SPD_MIN;m++)
      sum += tmp[m];
    
    sum/=MAX_SPD_MIN;
  }
  
  if( type==GET)
  {
    if(timeShow>=50)
      DispNumber((sum/10)%1000,1);
  }
}

//==============================================================================
//在中断中计脉冲数
//高/低电平时间小于1ms则不计数
//==============================================================================
void portE_Interrupt(void)
{
  static unsigned short lastTime=0;
  static unsigned short nowTime=0;
  unsigned short tmp; 
  
  nowTime = TIM3_GetCounter();
  
  if( GPIO_ReadInputPin(PWM_IN_PORT,PWM_IN_PIN) )
  {
    if(nowTime<lastTime)
      tmp = nowTime+62500;
    else
      tmp = nowTime;
    
    if( (tmp-lastTime) > 62 )
    {
      speedSecond++;
    }
  }
  
  lastTime = nowTime;
}

//==============================================================================
//tim2 20ms定时中断程序
//读取按键信息
//有按键延时显示
//==============================================================================
void tim2_Interrupt(void)
{
  set_PwmOut();

  get_Button();
  
  if(timeInc||timeDec||timeFood||timePwr)
    timeShow=0;
  else
  {
    if(++timeShow>=50)
      timeShow=50;
  }
  
  if(keyFlag)
    beepFlag = 1;
  
  TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}

//==============================================================================
//tim3 1s定时中断程序
//计算1min的脉冲数
//脉冲数清零
//==============================================================================
void tim3_Interrupe(void)
{
  timeSecondFlag ++ ;
  
  speedMinute = speedSecond*30;
  speedSecond = 0;
  TIM3_ClearITPendingBit(TIM3_IT_UPDATE);    //清除中断悬挂位
}

//==============================================================================
//tim4 1ms定时中断程序
//==============================================================================
void tim4_Interrupt(void)
{
  static unsigned char i;

  if(beepFlag)
  {
    if(++i>=100)
    {
      i=0;
      beepFlag=0;
      GPIO_WriteLow(BEEP_PORT,BEEP_PIN);
    }
    else
    {
      GPIO_WriteReverse(BEEP_PORT,BEEP_PIN);
    }
  }
  else
  {
    i=0;
    GPIO_WriteLow(BEEP_PORT,BEEP_PIN);
  }
  TIM4_ClearITPendingBit(TIM4_IT_UPDATE);    //清除中断悬挂位
}

//==============================================================================
//==============================================================================
void main(void)
{
  check_InOut();
  init_Clk();
  init_Beep();
  bsp_InitLcd();
  init_Button();
  init_PwmOut();
  init_PwmIn();
  
  DispOFF();
  rim();
  
  while(1)
  {
    key_ToGiveSpeed();

    time_ToGetSpeed();
  }
}


//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
