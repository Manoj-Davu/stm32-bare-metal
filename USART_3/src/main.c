#include "gpio.h"
#include <stdint.h>

#define USART      0x40011000
#define USART_SR   (*(volatile uint32_t *)(USART))
#define USART_DR   (*(volatile uint32_t *)(USART+0x04))
#define USART_BRR  (*(volatile uint32_t *)(USART+0x08))
#define USART_CR1  (*(volatile uint32_t *)(USART+0x0c))
#define USART_CR2  (*(volatile uint32_t *)(USART+0x10))
#define USART_CR3  (*(volatile uint32_t *)(USART+0x14))
#define USART_GTPR (*(volatile uint32_t *)(USART+0x18))


#define APB2  (*(volatile uint32_t *)0x40023844)

GPIO_Config usart_tx=
{
.pin=9,
.Mode=GPIO_AF,
.pull=NOPULL,
.speed=HIGHSPEED,
.otype=PUSHPULL,
.af=7
};


GPIO_Config usart_rx=
{
.pin=10,
.Mode=GPIO_AF,
.pull=NOPULL,
.speed=HIGHSPEED,
.otype=PUSHPULL,
.af=7
};

GPIO_Config led=
{
.pin=13,
.Mode=GPIO_OUTPUT,
.pull=NOPULL,
.speed=HIGHSPEED,
.otype=PUSHPULL,
.af=0
};


void error(const char* p)
{

while(*p!='\0')
{
	while(!(USART_SR&(1<<7)))
	{}
	USART_DR=*p;
	p++;
}

}

void status(void)
{
const char on_msg[]="\nLED ON\r\n";
const char off_msg[]="\nLED OFF\r\n";

uint16_t i=0;
if(!GPIO_Read(GPIOC,13))
{

while(on_msg[i]!='\0')
{
	while(!(USART_SR&(1<<7)))
	{}
	USART_DR=on_msg[i];
	i++;
}
}

else
{
i=0;
while(off_msg[i]!='\0')
{
	while(!(USART_SR&(1<<7)))
	{}
	USART_DR=off_msg[i];
	i++;
}
}

}

int main(void)
{

APB2|=(1<<4);
GPIO_Init(GPIOC,&led);
GPIO_Init(GPIOA,&usart_tx);
GPIO_Init(GPIOA,&usart_rx);
USART_BRR=(104<<4)|3;
USART_CR1|=(1<<2);
USART_CR1|=(1<<3);
USART_CR1|=(1<<13);

const char err_msg[]="\nPlease enter a correct command\r\n";
const char *p=&err_msg[0];
uint8_t cmd;


while (1)
{

while(!(USART_SR&(1<<5)))
{}
cmd=USART_DR;
switch  (cmd)
{
case '1':GPIO_Write(GPIOC,13,GPIO_PINHIGH);
	 status();
	 break;
case '0':GPIO_Write(GPIOC,13,GPIO_PINLOW);
	 status();
	 break;
case 's':status();
	 break;
case 'S':status();
	 break;
default:error(p);

}

}



}
