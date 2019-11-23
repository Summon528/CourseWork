#include <ctype.h>
#include <stdint.h>
#include "stm32l476xx.h"
#include "core_cm4.h"

#define SET_REG(REG, SELECT, VAL) ((REG) = ((REG) & (~(SELECT))) | (VAL))
#define GET_REG(REG, SELECT) ((REG) & (SELECT))

void GPIO_init() {
    SET_REG(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN, RCC_AHB2ENR_GPIOAEN);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODER5, GPIO_MODER_MODER5_0);
}

void SystemClock_Config() {
    RCC->CR |= RCC_CR_HSION;
    while (!GET_REG(RCC->CR, RCC_CR_HSIRDY))
        ;
    SET_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV16);
    SET_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);
    while (GET_REG(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
        ;
}

uint32_t time_s = 0;
void SysTick_Handler(void) {
    time_s += 1;
    if (time_s == 3) {
        GPIOA->BRR |= 1 << 5;
    } else if (time_s == 6) {
        GPIOA->BSRR |= 1 << 5;
        time_s = 0;
    }
}

int main() {
    SystemClock_Config();
    GPIO_init();
    GPIOA->BSRR |= 1 << 5;
    SysTick_Config(16e6 / 16);
    while (1)
        ;
}
