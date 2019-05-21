/**
  ******************************************************************************
  * @file     stm8s_it.h
  * @author   MCD Application Team
  * @version  V2.0.0
  * @date     25-February-2011
  * @brief    This file contains the headers of the interrupt handlers
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_IT_H
#define __STM8S_IT_H
#include "stm8s.h"

    
//------------------------------------------------------------------------------
#define KEY_INC_PORT	GPIOB
#define KEY_INC_PIN	GPIO_PIN_7

#define KEY_DEC_PORT	GPIOB
#define KEY_DEC_PIN	GPIO_PIN_5

#define KEY_FOOD_PORT	GPIOB
#define KEY_FOOD_PIN	GPIO_PIN_6

#define KEY_PWR_PORT	GPIOB
#define KEY_PWR_PIN	GPIO_PIN_4

//------------------------------------------------------------------------------
#define PWM_OUT_PORT	GPIOA
#define PWM_OUT_PIN	GPIO_PIN_3

//------------------------------------------------------------------------------
#define PWM_IN_PORT	GPIOE
#define PWM_IN_PIN	GPIO_PIN_5

//------------------------------------------------------------------------------
extern unsigned char keyFlag;
extern unsigned char timePwr;
extern unsigned char timeFood;
extern unsigned char timeInc;
extern unsigned char timeDec;

#define KEY_60MS        3
#define KEY_1300MS      65
#define KEY_1500MS      75

#define FLAG_PWR        0x01
#define FLAG_FOOD       0x02
#define FLAG_INC        0x04
#define FLAG_DEC        0x08

extern unsigned char beepFlag;

extern unsigned char timeShow;
//------------------------------------------------------------------------------


void time_buffer_init(void);

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef _COSMIC_
 void _stext(void); /* RESET startup routine */
 INTERRUPT void NonHandledInterrupt(void);
#endif /* _COSMIC_ */

#ifndef _RAISONANCE_
 INTERRUPT void TRAP_IRQHandler(void); /* TRAP */
 INTERRUPT void TLI_IRQHandler(void); /* TLI */
 INTERRUPT void AWU_IRQHandler(void); /* AWU */
 INTERRUPT void CLK_IRQHandler(void); /* CLOCK */
 INTERRUPT void EXTI_PORTA_IRQHandler(void); /* EXTI PORTA */
 INTERRUPT void EXTI_PORTB_IRQHandler(void); /* EXTI PORTB */
 INTERRUPT void EXTI_PORTC_IRQHandler(void); /* EXTI PORTC */
 INTERRUPT void EXTI_PORTD_IRQHandler(void); /* EXTI PORTD */
 INTERRUPT void EXTI_PORTE_IRQHandler(void); /* EXTI PORTE */

#ifdef STM8S903
 INTERRUPT void EXTI_PORTF_IRQHandler(void); /* EXTI PORTF */
#endif /*STM8S903*/

#if defined (STM8S208) || defined (STM8AF52Ax)
 INTERRUPT void CAN_RX_IRQHandler(void); /* CAN RX */
 INTERRUPT void CAN_TX_IRQHandler(void); /* CAN TX/ER/SC */
#endif /* STM8S208 || STM8AF52Ax */

 INTERRUPT void SPI_IRQHandler(void); /* SPI */
 INTERRUPT void TIM1_CAP_COM_IRQHandler(void); /* TIM1 CAP/COM */
 INTERRUPT void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void); /* TIM1 UPD/OVF/TRG/BRK */

#ifdef STM8S903
 INTERRUPT void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void); /* TIM5 UPD/OVF/BRK/TRG */
 INTERRUPT void TIM5_CAP_COM_IRQHandler(void); /* TIM5 CAP/COM */
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8A626x*/
 INTERRUPT void TIM2_UPD_OVF_BRK_IRQHandler(void); /* TIM2 UPD/OVF/BRK */
 INTERRUPT void TIM2_CAP_COM_IRQHandler(void); /* TIM2 CAP/COM */
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105) || defined (STM8AF52Ax) ||\
    defined (STM8AF62Ax) || defined (STM8AF626x)
 INTERRUPT void TIM3_UPD_OVF_BRK_IRQHandler(void); /* TIM3 UPD/OVF/BRK */
 INTERRUPT void TIM3_CAP_COM_IRQHandler(void); /* TIM3 CAP/COM */
#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF52Ax or STM8AF62Ax or STM8A626x */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S103) || defined (STM8AF52Ax) ||\
    defined (STM8AF62Ax) || defined (STM8S903)
 INTERRUPT void UART1_TX_IRQHandler(void); /* UART1 TX */
 INTERRUPT void UART1_RX_IRQHandler(void); /* UART1 RX */
#endif /*STM8S208, STM8S207, STM8S903 or STM8S103 or STM8AF52Ax or STM8AF62Ax */

 INTERRUPT void I2C_IRQHandler(void); /* I2C */

#if defined (STM8S105) || defined (STM8AF626x)
 INTERRUPT void UART2_RX_IRQHandler(void); /* UART2 RX */
 INTERRUPT void UART2_TX_IRQHandler(void); /* UART2 TX */
#endif /* STM8S105 or STM8AF626x */

#if defined(STM8S207) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 INTERRUPT void UART3_RX_IRQHandler(void); /* UART3 RX */
 INTERRUPT void UART3_TX_IRQHandler(void); /* UART3 TX */
#endif /*STM8S207, STM8S208, STM8AF62Ax or STM8AF52Ax */

#if defined(STM8S207) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 INTERRUPT void ADC2_IRQHandler(void); /* ADC2 */
#else /*STM8S105, STM8S103 or STM8S903*/
 INTERRUPT void ADC1_IRQHandler(void); /* ADC1 */
#endif /*STM8S207, STM8S208, STM8AF62Ax or STM8AF52Ax */

#ifdef STM8S903
 INTERRUPT void TIM6_UPD_OVF_TRG_IRQHandler(void); /* TIM6 UPD/OVF/TRG */
#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */
 INTERRUPT void TIM4_UPD_OVF_IRQHandler(void); /* TIM4 UPD/OVF */
#endif /*STM8S903*/
 INTERRUPT void EEPROM_EEC_IRQHandler(void); /* EEPROM ECC CORRECTION */
#endif /* _RAISONANCE_ */

#endif /* __STM8S_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
