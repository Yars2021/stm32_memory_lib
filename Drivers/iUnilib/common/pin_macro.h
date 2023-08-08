/*
 *  pin_macro.h rev.0.0.1 2011-04-07
 *
 *  Stm32 GPIO manipulator.
 *
 *
 *  Contributors:
 *                Ivanov Anton (c) www.automatix.ru
 *
 * MODES:
 * 	ANALOG
 * 	INPUT_FLOATING
 * 	INPUT_PULL_DOWN
 * 	INPUT_PULL_UP
 * 	OUTPUT_PUSH_PULL
 * 	OUTPUT_OPEN_DRAIN
 * 	ALT_OUTPUT_PUSH_PULL
 * 	ALT_OUTPUT_OPEN_DRAIN
 *
 * SPEEDS:
 * 	SPEED_NONE
 * 	SPEED_2MHZ
 * 	SPEED_10MHZ
 * 	SPEED_50MHZ
 *
 * Usage example:
 *
 *	#define PIN_ADC_CS	    A,1,OUTPUT_PUSH_PULL,SPEED_50MHZ
 * 	#define PIN_ADC_READY	B,12,INPUT_PULL_UP,SPEED_NONE
 *
 * 	PIN_INIT(PIN_ADC_CS);  PIN_SET(PIN_ADC_CS);
 * 	PIN_INIT(PIN_ADC_READY);
 *
 * 	PIN_CLR(PIN_ADC_CS);
 *
 * 	if (PIN_TEST(PIN_ADC_READY))
 * 	{
 * 		...
 * 	}
 *
 * 	PIN_SET(PIN_ADC_CS);
 *
 */

#ifndef _PIN_MACRO_H_
#define _PIN_MACRO_H_

#if F1_CHECK

//-----------------------------------------------------------------------------
	// definition for RCC_APB2ENR register:
	/*
	#define  RCC_APB2ENR_AFIOEN           (1UL<<0)  // 0x01   Alternate Function I/O clock enable
	#define  RCC_APB2ENR_IOPAEN           (1UL<<2)  // 0x04   I/O port A clock enable
	#define  RCC_APB2ENR_IOPBEN           (1UL<<3)  // 0x08   I/O port B clock enable
	#define  RCC_APB2ENR_IOPCEN           (1UL<<4)  // 0x10   I/O port C clock enable
	#define  RCC_APB2ENR_IOPDEN           (1UL<<5)  // 0x20   I/O port D clock enable
	#define  RCC_APB2ENR_ADC1EN           (1UL<<9)  // 0x0200 ADC 1 interface clock enable
	*/

	//-----------------------------------------------------------------------------
	#define SPEED_NONE                    0UL  // input mode
	#define SPEED_10MHZ                   1UL  // output mode
	#define SPEED_2MHZ                    2UL  // output mode
	#define SPEED_50MHZ                   3UL  // output mode

	//-----------------------------------------------------------------------------
	#define _GPIO_PORT_CLK_ON( XPORT)  do {RCC->APB2ENR |=  RCC_APB2ENR_IOP##XPORT##EN;} while (0)
	#define _GPIO_PORT_CLK_OFF(XPORT)  do {RCC->APB2ENR &= ~RCC_APB2ENR_IOP##XPORT##EN;} while (0)

	//-----------------------------------------------------------------------------
	#define _GPIO_PIN_INIT(XPORT, XPIN, XALEVEL, XMODE)                                                        \
	do                                                                                                         \
	{                                                                                                          \
		static const uint32_t OFFSET = ((XPIN)*4UL) & 31UL;                                                    \
																											   \
		_GPIO_PORT_CLK_ON(XPORT);                                                                              \
																											   \
		if ((XPIN)<8) (GPIO##XPORT)->CRL = ((GPIO##XPORT)->CRL & ~(15UL << OFFSET)) | ((XMODE) << OFFSET); \
		else	      (GPIO##XPORT)->CRH = ((GPIO##XPORT)->CRH & ~(15UL << OFFSET)) | ((XMODE) << OFFSET); \
	} while (0)

	//-----------------------------------------------------------------------------
	#define _PIN_INIT(  XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {_GPIO_INIT_##XMODE(XPORT, XPIN, XALEVEL, XMODE, XSPEED);} while (0)

	#define _PIN_RD_PORT(XPORT, XPIN, XALEVEL, XMODE, XSPEED)    GPIO##XPORT->IDR
	#define _PIN_WR_PORT(XPORT, XPIN, XALEVEL, XMODE, XSPEED)    GPIO##XPORT->ODR

	#define _PIN_MASK(XPORT, XPIN, XALEVEL, XMODE, XSPEED)    (1UL << (XPIN))

	#define _PIN_SET(   XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {GPIO##XPORT->BSRR = (1UL << (XPIN)); } while (0)  // Atomic operation
	#define _PIN_CLR(   XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {GPIO##XPORT->BRR  = (1UL << (XPIN)); } while (0)  // Atomic operation

	#define _PIN_TEST(  XPORT, XPIN, XALEVEL, XMODE, XSPEED)   ( (GPIO##XPORT->IDR  & (1UL << (XPIN))) != 0UL )

	#define _PIN_TOGGLE(XPORT, XPIN, XALEVEL, XMODE, XSPEED)                                                 \
	do                                                                                                       \
	{                                                                                                        \
		if (_PIN_TEST(XPORT, XPIN, XALEVEL, XMODE, XSPEED))  _PIN_CLR(XPORT, XPIN, XALEVEL, XMODE, XSPEED);  \
		else                                                 _PIN_SET(XPORT, XPIN, XALEVEL, XMODE, XSPEED);  \
	} while (0)

	//-----------------------------------------------------------------------------
	#define _GPIO_PIN_INIT_H(XPORT, XPIN, XALEVEL, XMODE)                                                      \
	do                                                                                                         \
	{                                                                                                          \
		_GPIO_PIN_INIT(XPORT, XPIN, XALEVEL, XMODE);                                                           \
		_PIN_CLR(XPORT, XPIN, XALEVEL, XMODE, XSPEED);                                                         \
	} while (0)

	//-----------------------------------------------------------------------------
	#define _GPIO_PIN_INIT_L(XPORT, XPIN, XALEVEL, XMODE)                                                      \
	do                                                                                                         \
	{                                                                                                          \
		_GPIO_PIN_INIT(XPORT, XPIN, XALEVEL, XMODE);                                                           \
		_PIN_SET(XPORT, XPIN, XALEVEL, XMODE, XSPEED);                                                         \
	} while (0)

	//-----------------------------------------------------------------------------
	#define _GPIO_INIT_ANALOG(               XPORT, XPIN, XALEVEL, XMODE, XSPEED)      _GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  0UL)
	#define _GPIO_INIT_INPUT_FLOATING(       XPORT, XPIN, XALEVEL, XMODE, XSPEED)      _GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  4UL)
	#define _GPIO_INIT_INPUT_PULL_UP(        XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {_GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  8UL); _PIN_SET(XPORT, XPIN, XALEVEL, XMODE, XSPEED);} while (0)
	#define _GPIO_INIT_INPUT_PULL_DOWN(      XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {_GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  8UL); _PIN_CLR(XPORT, XPIN, XALEVEL, XMODE, XSPEED);} while (0)
	#define _GPIO_INIT_OUTPUT_PUSH_PULL(     XPORT, XPIN, XALEVEL, XMODE, XSPEED)      _GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  0UL | XSPEED)
	#define _GPIO_INIT_OUTPUT_OPEN_DRAIN(    XPORT, XPIN, XALEVEL, XMODE, XSPEED)      _GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  4UL | XSPEED)
	#define _GPIO_INIT_ALT_OUTPUT_PUSH_PULL( XPORT, XPIN, XALEVEL, XMODE, XSPEED)      _GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL,  8UL | XSPEED)
	#define _GPIO_INIT_ALT_OUTPUT_OPEN_DRAIN(XPORT, XPIN, XALEVEL, XMODE, XSPEED)      _GPIO_PIN_INIT_##XALEVEL(XPORT, XPIN, XALEVEL, 12UL | XSPEED)

#elif F4_CHECK
//-----------------------------------------------------------------------------
#define SPEED_2MHZ                    0UL  // output mode
#define SPEED_25MHZ                   1UL  // output mode
#define SPEED_50MHZ                   2UL  // output mode
#define SPEED_100MHZ                  3UL  // output mode

//-----------------------------------------------------------------------------
#define _GPIO_PORT_CLK_ON( XPORT)  do {RCC->AHB1ENR |=  RCC_AHB1ENR_GPIO##XPORT##EN;} while (0)
#define _GPIO_PORT_CLK_OFF(XPORT)  do {RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIO##XPORT##EN;} while (0)

//-----------------------------------------------------------------------------
#define _GPIO_PORT_SPEED_SET(XPORT, XSPEED)  do {GPIO##XPORT->OSPEEDR |= (XSPEED << XPIN);} while (0)

//-----------------------------------------------------------------------------
#define _PIN_INIT(  XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {_GPIO_PIN_INIT(XPORT, XPIN, XALEVEL, XMODE, XSPEED);} while (0)

#define _PIN_RD_PORT(XPORT, XPIN, XALEVEL, XMODE, XSPEED)    GPIO##XPORT->IDR
#define _PIN_WR_PORT(XPORT, XPIN, XALEVEL, XMODE, XSPEED)    GPIO##XPORT->ODR

#define _PIN_MASK(XPORT, XPIN, XALEVEL, XMODE, XSPEED)    (1UL << (XPIN))

#define _PIN_SET(   XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {GPIO##XPORT->BSRR = (1UL << (uint16_t)(XPIN)); } while (0)  // Atomic operation
#define _PIN_CLR(   XPORT, XPIN, XALEVEL, XMODE, XSPEED)  do {GPIO##XPORT->BSRR  = (1UL << (uint16_t)(XPIN+16)); } while (0)  // Atomic operation

#define _PIN_TEST(  XPORT, XPIN, XALEVEL, XMODE, XSPEED)   ( (GPIO##XPORT->IDR  & (1UL << (XPIN))) != 0UL )
#define _PIN_TOGGLE(XPORT, XPIN, XALEVEL, XMODE, XSPEED)                                                 \
	do                                                                                                       \
	{                                                                                                        \
		if (_PIN_TEST(XPORT, XPIN, XALEVEL, XMODE, XSPEED))  _PIN_CLR(XPORT, XPIN, XALEVEL, XMODE, XSPEED);  \
		else                                                 _PIN_SET(XPORT, XPIN, XALEVEL, XMODE, XSPEED);  \
	} while (0)

#define _PIN_INIT_AF(  XPORT, XPIN, XALEVEL, XMODE, XSPEED, XGPIOAF)													\
	do                                                                                                                      \
	{                                                                                                                       \
		uint32_t temp = 0x00;																								\
		uint32_t temp_2 = 0x00;                                                                                             \
	                                                                                                                        \
		temp = ((uint32_t)(XGPIOAF) << ((uint32_t)((XPIN) & (uint32_t)0x07) * 4));                           				\
		(GPIO##XPORT)->AFR[(XPIN) >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((XPIN) & (uint32_t)0x07) * 4)); 				\
		temp_2 = (GPIO##XPORT)->AFR[(XPIN) >> 0x03] | temp;																\
		(GPIO##XPORT)->AFR[(XPIN) >> 0x03] = temp_2;                                                                      \
	}while  (0)

#define _PIN_INIT_SET_H(XPORT, XPIN, XALEVEL, XMODE, XSPEED) do {_PIN_SET(XPORT, XPIN, XALEVEL, XMODE, XSPEED);} while (0)
#define _PIN_INIT_SET_L(XPORT, XPIN, XALEVEL, XMODE, XSPEED) do {_PIN_CLR(XPORT, XPIN, XALEVEL, XMODE, XSPEED);} while (0)

//-----------------------------------------------------------------------------
#define _GPIO_PIN_INIT(XPORT, XPIN, XALEVEL, XMODE, XSPEED)													    \
	do																											\
	{																											\
		_GPIO_PORT_CLK_ON(XPORT);																				\
 		(GPIO##XPORT)->MODER &=~  ( (0x03UL) << (((uint32_t)(XPIN)) * 2UL));									\
		(GPIO##XPORT)->MODER |=   ( ((((uint32_t)(XMODE))>>4UL)& 0x03UL) << (((uint32_t)(XPIN)) * 2UL));		\
																												\
		(GPIO##XPORT)->OSPEEDR &=~  ( (0x03UL) << ((uint32_t)(XPIN) * 2UL));									\
		(GPIO##XPORT)->OSPEEDR |=   ((uint32_t)(XSPEED) << ((uint32_t)(XPIN) * 2UL));							\
																												\
		(GPIO##XPORT)->OTYPER &=~  ( 0x01UL << (uint32_t)(XPIN) );											    \
		(GPIO##XPORT)->OTYPER |=   ( (((uint32_t)(XMODE) &0x0CUL)>>2UL) << (uint32_t)(XPIN) );				    \
																												\
		(GPIO##XPORT)->PUPDR &=~  ( 0x03UL << ((uint32_t)(XPIN) * 2UL));										\
		(GPIO##XPORT)->PUPDR |=   ( ((uint32_t)(XMODE) & 0x03UL) << ((uint32_t)(XPIN) * 2UL));				    \
                                                                                                                \
        _PIN_INIT_SET_##XALEVEL(XPORT, XPIN, XALEVEL, XMODE, XSPEED);                                           \
	} while (0)

/******************modes*********************/
/************************************GPIO_Mode***OType*******PullUpPullDownConf*/
#define ANALOG				        		((0x03<<4) | (0x00<<2) | (0x00))
#define INPUT_FLOATING		        		((0x00<<4) | (0x00<<2) | (0x00))
#define INPUT_PULL_UP		        		((0x00<<4) | (0x00<<2) | (0x01))
#define INPUT_PULL_DOWN		        		((0x00<<4) | (0x00<<2) | (0x02))

#define OUTPUT_PUSH_PULL			        ((0x01<<4) | (0x00<<2) | (0x00))
#define OUTPUT_OPEN_DRAIN_NO_PULL       	((0x01<<4) | (0x01<<2) | (0x00))
#define OUTPUT_OPEN_DRAIN_PULL_UP       	((0x01<<4) | (0x01<<2) | (0x01))
#define OUTPUT_OPEN_DRAIN_PULL_DOWN       	((0x01<<4) | (0x01<<2) | (0x02))
#define ALT_OUTPUT_PUSH_PULL		        ((0x02<<4) | (0x00<<2) | (0x01))
#define ALT_OUTPUT_OPEN_DRAIN	        	((0x02<<4) | (0x01<<2) | (0x00))

#endif





//-----------------------------------------------------------------------------
#define pin_init(PIN)              _PIN_INIT(PIN)

#define pin_rd_port(PIN)           _PIN_RD_PORT(PIN)
#define pin_wr_port(PIN)           _PIN_WD_PORT(PIN)
#define pin_mask(PIN)              _PIN_MASK(PIN)

#define pin_set(PIN)               _PIN_SET(PIN)
#define pin_clr(PIN)               _PIN_CLR(PIN)
#define pin_toggle(PIN)            _PIN_TOGGLE(PIN)

#define pin_test(PIN)              _PIN_TEST(PIN)

#define pin_init_af(PIN, GPIOAF)   _PIN_INIT_AF(PIN, GPIOAF)

#define pin_write(PIN,VALUE)       do {if (VALUE) _PIN_SET(PIN); else _PIN_CLR(PIN); } while (0)

#define pincpy(PIN_DST,PIN_SRC)    do {if (_PIN_TEST(PIN_SRC)) _PIN_SET(PIN_DST); else _PIN_CLR(PIN_DST); } while (0)

#endif /* _PIN_MACRO_H_ */
