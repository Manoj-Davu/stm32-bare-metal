#define RCC_AHB1ENR   (*(volatile unsigned int *)0x40023830)
#define GPIOA_MODER   (*(volatile unsigned int *)0x40020000)
#define GPIOA_ODR     (*(volatile unsigned int *)0x40020014)

void delay(void)
{
    for (volatile unsigned int i = 0; i < 100000; i++)
    {
    }
}

int main(void)
{
    /* 1. Enable GPIOA clock */
    RCC_AHB1ENR |= (1 << 0);

    /* 2. Configure PA5 as output */
    GPIOA_MODER &= ~(3 << (5 * 2));
    GPIOA_MODER |=  (1 << (5 * 2));

    while (1)
    {
        /* 3. Toggle PA5 */
        GPIOA_ODR ^= (1 << 5);

        delay();
    }
}
