#ifndef _board_h_
#define _board_h_

#include <stdint.h>
#include "stm32f030x6.h"

#include "ks0713.h"
#include "text.h"


/** ------------------- GPIO definitions -------------------------------
 * |31 - 11 | 10 - 7 | 6  | 5 - 4 | 3 - 2 | 1 - 0 |
 * |  RSV   |  AFx   | OD | PU-PD | SPEED |  MODE |
 * */

#define OSPEEDR_LS          (0 << 2)
#define OSPEEDR_MS          (1 << 2)
#define OSPEEDR_HS          (3 << 2)

/** Output **/
#define GPO                 (1 << 0)
#define GPO_LS              (GPO | OSPEEDR_LS)
#define GPO_MS              (GPO | OSPEEDR_MS)
#define GPO_HS              (GPO | OSPEEDR_HS)

/** Input **/
#define GPI                 (0 << 0)
#define GPI_AN              (3 << 0)

/** ALternative Function **/
#define GPIO_AF             (2 << 0)
#define GPIO_AF0            ((0 << 7) | GPIO_AF)
#define GPIO_AF1            ((1 << 7) | GPIO_AF)
#define GPIO_AF2            ((2 << 7) | GPIO_AF)
#define GPIO_AF3            ((3 << 7) | GPIO_AF)
#define GPIO_AF4            ((4 << 7) | GPIO_AF)
#define GPIO_AF5            ((5 << 7) | GPIO_AF)
#define GPIO_AF6            ((6 << 7) | GPIO_AF)
#define GPIO_AF7            ((7 << 7) | GPIO_AF)

#define GPIO_PU             (1 << 4)
#define GPIO_PD             (2 << 4)
#define GPIO_OD             (1 << 6)
      
/*--------------------------------------------------------------------*/

#define LED_PIN             4
#define LED_PORT            GPIOA
#define LED_INIT            BOARD_gpioInit(LED_PORT, LED_PIN, GPO_LS)
#define LED_TOGGLE          LED_PORT->ODR ^= (1 << LED_PIN)

#define LCD_CK_Pin          5
#define LCD_CK_Port         GPIOA
#define LCD_RS_Pin          6
#define LCD_RS_Port         GPIOA
#define LCD_DI_Pin          7
#define LCD_DI_Port         GPIOA
#define LCD_RST_Pin         1
#define LCD_RST_Port        GPIOB
#define LCD_BIT_BANG
#define LCDSDI0             LCD_DI_Port->BRR   = (1 << LCD_DI_Pin)
#define LCDSDI1             LCD_DI_Port->BSRR  = (1 << LCD_DI_Pin)
#define LCDCLK0             LCD_CK_Port->BRR   = (1 << LCD_CK_Pin)
#define LCDCLK1             LCD_CK_Port->BSRR  = (1 << LCD_CK_Pin)
#define LCDRS0              LCD_RS_Port->BRR   = (1 << LCD_RS_Pin)
#define LCDRS1              LCD_RS_Port->BSRR  = (1 << LCD_RS_Pin)
#define LCDRST0             LCD_RST_Port->BRR  = (1 << LCD_RST_Pin)
#define LCDRST1             LCD_RST_Port->BSRR = (1 << LCD_RST_Pin)
#define LCDCS0
#define LCDCS1

#define PERIODIC_TIMER      TIM3

extern uint32_t SystemCoreClock;

void BOARD_gpioInit(GPIO_TypeDef *port, uint8_t pin, uint16_t mode);
void BOARD_Init(void);
void BOARD_DelayMs(uint32_t ms);
uint32_t BOARD_GetTicks(void);
void BOARD_UsartInit(uint32_t speed);
void BOARD_UsartTransmit(uint8_t *data, uint16_t count);
void BOARD_UsartReceiveDMA(uint8_t *data, uint16_t count, void(*eor)(void));

#define DelayMs             BOARD_DelayMs
#define getTicks            BOARD_GetTicks
#define serialInit          BOARD_UsartInit

#endif