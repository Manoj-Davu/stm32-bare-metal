#define RCC (*(volatile unsigned int*)0x40023830)
#define MODE (*(volatile unsigned int*)0x40020000)
#define OUT (*(volatile unsigned int*)0x40020014)
#define IN (*(volatile unsigned int*)0x40020010)
#define UD (*(volatile unsigned int*)0x4002000c)



void delay(void)
{
for( volatile unsigned int i=0;i<1000000;i++)
{}
}

void blink(void)
{
OUT|=((1<<5) | (1<<4) | (1<<3));
delay();
OUT&=~((1<<5) | (1<<4) | (1<<3));

}

int main(void)
{
RCC|=(1<<0);//Enabling clock to GPIO pins

/*Configuring Pin-5 as output*/
MODE&=~(3<<(5*2));
MODE|=(1<<(5*2));

/*Configuring Pin-4 as output*/
MODE&=~(3<<(4*2));
MODE|=(1<<(4*2));

/*Configuring Pin-3 as output*/
MODE&=~(3<<(3*2));
MODE|=(1<<(3*2));

/*Configuring Pin-2 as input*/
MODE&=~(3<<(2*2));

/*Configuring Pin-2 as Pull-up-down register*/
UD&=~(3<<(2*2));
UD|=(2<<(2*2));


while(1)
{
OUT |= (1<<5);
delay();
OUT&=~(1<<5);

OUT |=(1<<4);
delay();

if(IN & (1<<2))
{
OUT &=~(1<<4);
OUT|=(1<<3);
delay();
OUT&=~(1<<3);
blink();
delay();
}

}

}
