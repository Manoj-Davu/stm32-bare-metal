#define RCC_AHB1ENR   (*(volatile unsigned int *)0x40023830)
#define GPIOA_MODER   (*(volatile unsigned int *)0x40020000)
#define GPIOA_PUPDR   (*(volatile unsigned int *)0x4002000C)
#define GPIOA_IDR     (*(volatile unsigned int *)0x40020010)
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

    /* 3. Configure PA0 as input */
    GPIOA_MODER &= ~(3 << (0 * 2));

    /* 4. Enable pull-down resistor on PA0 */
    GPIOA_PUPDR &= ~(3 << (0 * 2));
    GPIOA_PUPDR |=  (2 << (0 * 2));

    while (1)
    {
        /* 5. Check if button is pressed */
        if (GPIOA_IDR & (1 << 0))
        {
            /* Button pressed → blink LED */

            GPIOA_ODR ^= (1 << 5);

            delay();
        }
        else
        {
            /* Button released → LED OFF */

            GPIOA_ODR &= ~(1 << 5);
        }
    }}
