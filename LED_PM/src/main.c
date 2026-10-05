#include "gpio.h"
#include<stdint.h>

#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840)


#define  ADC_CLK     (*(volatile uint32_t *) 0x40023844 )
#define  ADC         0x40012000
#define  ADC_SR      (*(volatile uint32_t *)(ADC+0x00))
#define  ADC_CR1     (*(volatile uint32_t *)(ADC+0x04))
#define  ADC_CR2     (*(volatile uint32_t *)(ADC+0x08))
#define  ADC_SMPR1   (*(volatile uint32_t *)(ADC+0x0c))
#define  ADC_SMPR2   (*(volatile uint32_t *)(ADC+0X10))
#define  ADC_SQR1    (*(volatile uint32_t *)(ADC+0x2c))
#define  ADC_SQR3    (*(volatile uint32_t *)(ADC+0x34))
#define  ADC_DR      (*(volatile uint32_t *)(ADC+0x4c))
#define  ADC_CCR     (*(volatile uint32_t *)(ADC+0x300))

#define  TIM2        0x40000000
#define  TIM2_CR1    (*(volatile uint32_t *)(TIM2+0x00))
#define  TIM2_EGR    (*(volatile uint32_t *)(TIM2+0x14))
#define  TIM2_CCMR1  (*(volatile uint32_t *)(TIM2+0x18))
#define  TIM2_CCER   (*(volatile uint32_t *)(TIM2+0x20))
#define  TIM2_PSC    (*(volatile uint32_t *)(TIM2+0x28))
#define  TIM2_ARR    (*(volatile uint32_t *)(TIM2+0x2c))
#define  TIM2_CCR1   (*(volatile uint32_t *)(TIM2+0x34))


GPIO_Config led =
{
.pin=5,
.Mode=GPIO_AF,
.pull=NOPULL,
.speed=LOWSPEED,
.otype=PUSHPULL,
.af=1
};


GPIO_Config pm =
{
.pin=0,
.Mode=GPIO_ANALOG,
.pull=NOPULL,
.speed=HIGHSPEED,
.otype=PUSHPULL,
.af=0
};



void main(void)
{

RCC_APB1ENR |= (1 << 0);
ADC_CLK  |= (1<<8);

GPIO_Init(GPIOA,&led);
GPIO_Init(GPIOA,&pm);

TIM2_PSC = 419;
TIM2_ARR = 199;
TIM2_CCMR1 |= (6<<4);
TIM2_CCMR1 |= (1<<3);

TIM2_EGR |= (1<<0);
TIM2_CCER |= (1<<0);
TIM2_CR1 |= (1<<0);


ADC_SQR3 |= (0<<4);
ADC_SMPR2 |= (1<<0);
ADC_CCR &= ~(3U << 16);
ADC_CCR |=  (1U << 16);
ADC_CR2 |= (1<<0);


uint16_t adc_val;

while(1)
{
    /* Start ADC conversion */
    ADC_CR2 |= (1 << 30);

    /* Wait until conversion is complete */
    while (!(ADC_SR & (1 << 1)))
    {
    }

    /* Read ADC result */
    adc_val = ADC_DR;
TIM2_CCR1 = ((adc_val)*(TIM2_ARR))/4095;
}
}
