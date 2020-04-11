#include "board.h"

typedef struct {
    uint32_t baudrate;
    uint32_t brr;
}usartbauds_t;

static const usartbauds_t bauds[] = {
    {2400, 0x4e20},
    {9600, 0x1388},
    {115200, 0x1a1},
};

uint32_t SystemCoreClock = 8000000UL;
static volatile uint32_t ticks = 0;
static void (*usartEOR)(void);

void error_handler(void){
	while(1){

	}
}

void SystemInit(void)
{
	/** Enable HSE **/
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));

	/** Configure PLL **/
	if(RCC->CR & RCC_CR_PLLON){
		RCC->CR &= ~RCC_CR_PLLON;
		while(!(RCC->CR & RCC_CR_PLLON));
	}

	// Select PLL source
	RCC->CFGR |=RCC_CFGR_PLLSRC;
	// Set PLL multiplier to 12
	RCC->CFGR = (RCC->CFGR & (15 << 18)) | (10 << 18);
	// Set prediv 2
	RCC->CFGR |=RCC_CFGR_PLLXTPRE;
	RCC->CFGR2 = 1;
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLON));

	/** Configure clocks **/
	// Flash latency one wait state
	FLASH->ACR = 1;
	// AHB /1
	RCC->CFGR = (RCC->CFGR & ~(15 << 4));
	// APB1 /1
	RCC->CFGR = (RCC->CFGR & ~(7 << 8));
	// Select PLL as clock source
	RCC->CFGR = (RCC->CFGR & ~(3 << 0)) | 2;

	while((RCC->CFGR & (3 << 2)) != (2 << 2));
	SystemCoreClock = 48000000UL;
	BOARD_Init();
}

/**
 *  PA9     ------> USART1_TX (AF1)
 *  PA10    ------> USART1_RX (AF1)
 * */
void BOARD_UsartInit(uint32_t speed){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHBENR |= RCC_AHBENR_DMAEN;

	RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;

	BOARD_gpioInit(GPIOA, 9, GPIO_AF1);
	BOARD_gpioInit(GPIOA, 10, GPIO_AF1);

	USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE;

	for(uint32_t i = 0; i < sizeof(bauds)/sizeof(usartbauds_t); i++){
		if(bauds[i].baudrate == speed){
			USART1->BRR = bauds[i].brr;
			break;
		}
	}	

	USART1->CR1 |= USART_CR1_UE;
}

void BOARD_Init(void){
	SysTick_Config(SystemCoreClock / 1000);
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN;
	
	LED_INIT;

	BOARD_gpioInit(LCD_CK_Port, LCD_CK_Pin, GPO_LS);
	BOARD_gpioInit(LCD_DI_Port, LCD_DI_Pin, GPO_LS);
	BOARD_gpioInit(LCD_RS_Port, LCD_RS_Pin, GPO_LS);
	BOARD_gpioInit(LCD_RST_Port, LCD_RST_Pin, GPO_LS);	
}

void BOARD_gpioInit(GPIO_TypeDef *port, uint8_t pin, uint16_t mode) {

uint16_t tmp = mode & 3;

	// Configure mode
	port->MODER = (port->MODER & ~(3 << (pin << 1))) | (tmp << (pin << 1));
	if(tmp == GPIO_AF){
		tmp = (mode >> 7) & 15;
		uint8_t bit = ((pin & 7) << 2);
		volatile uint32_t *reg = &port->AFR[(pin>>3) & 1];
		*reg = (*reg & ~(15 << bit)) | (tmp << bit);
	}

	// Configure speed
	tmp = (mode >> 2) & 3;
	port->OSPEEDR = (port->OSPEEDR & ~(3 << (pin << 1))) | (tmp << (pin << 1));

	// Configure pull-up/down
	tmp = (mode >> 4) & 3;
	port->PUPDR = (port->PUPDR & ~(3 << (pin << 1))) | (tmp << (pin << 1));

	// Configure open drain
	tmp = (mode >> 6) & 1;
	port->OTYPER = (port->OTYPER & ~(1 << pin)) | (tmp << pin);
}

uint32_t BOARD_GetTicks(void){
	return ticks;
}

void BOARD_DelayMs(uint32_t ms){
uint32_t expire = ticks + ms;
	while(ticks < expire);	
}

void BOARD_UsartTransmit(uint8_t *data, uint16_t count){
	while(!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = *data;
}

void BOARD_UsartReceiveDMA(uint8_t *data, uint16_t count, void(*eor)(void)){
	DMA1_Channel3->CCR = 
			//DMA_CCR_PL |        // Highest priority
            (0 << 10) |         // Memory size 8bit
            (0 << 8)  |         // Peripheral size 8bit
            DMA_CCR_MINC |      // Memory increment
            DMA_CCR_TCIE;       // Enable end of transfer interrupt

	DMA1_Channel3->CNDTR = count;
	DMA1_Channel3->CPAR = (uint32_t)&USART1->RDR;
	DMA1_Channel3->CMAR = (uint32_t)data;

	usartEOR = eor;

	USART1->CR3 |= USART_CR3_DMAR;

	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}
/**
 * 
 * */
void SysTick_Handler(void){
	ticks++;
}

void DMA1_Channel2_3_IRQHandler(void){
	if(DMA1->ISR & DMA_ISR_TCIF3){
		usartEOR();
		DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	}

	DMA1->IFCR |= DMA_IFCR_CGIF3;
}
