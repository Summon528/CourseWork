#include <stdint.h>

#include "stm32l476xx.h"
#include "core_cm4.h"

#define SET_REG(REG, SELECT, VAL) ((REG) = ((REG) & (~(SELECT))) | (VAL))
#define GET_REG(REG, SELECT) ((REG) & (SELECT))

int key_map[4][4] = {
    {1, 4, 7, 15}, {2, 5, 8, 0}, {3, 6, 9, 14}, {10, 11, 12, 13}};

int keypad_scan(int target);
int blink_cnt = 0;

void GPIO_init() {
    SET_REG(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN, RCC_AHB2ENR_GPIOAEN);
    // LED
    SET_REG(GPIOA->MODER, GPIO_MODER_MODER5, GPIO_MODER_MODE5_0);
    // Key Pad
    SET_REG(GPIOA->MODER, GPIO_MODER_MODER4, 0);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODER6, 0);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODER7, 0);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODER8, 0);

    SET_REG(GPIOA->MODER, GPIO_MODER_MODE9, GPIO_MODER_MODE9_0);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODE10, GPIO_MODER_MODE10_0);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODE11, GPIO_MODER_MODE11_0);
    SET_REG(GPIOA->MODER, GPIO_MODER_MODE12, GPIO_MODER_MODE12_0);

    SET_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD4, GPIO_PUPDR_PUPD4_1);
    SET_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD6, GPIO_PUPDR_PUPD6_1);
    SET_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD7, GPIO_PUPDR_PUPD7_1);
    SET_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD8, GPIO_PUPDR_PUPD8_1);
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

void EXTI_config() {
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM4, EXTI_IMR1_IM4);
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM6, EXTI_IMR1_IM6);
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM7, EXTI_IMR1_IM7);
    SET_REG(EXTI->IMR1, EXTI_IMR1_IM8, EXTI_IMR1_IM8);

    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT4, EXTI_RTSR1_RT4);
    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT6, EXTI_RTSR1_RT6);
    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT7, EXTI_RTSR1_RT7);
    SET_REG(EXTI->RTSR1, EXTI_RTSR1_RT8, EXTI_RTSR1_RT8);

    SET_REG(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN, RCC_APB2ENR_SYSCFGEN);

    SET_REG(SYSCFG->EXTICR[2], SYSCFG_EXTICR2_EXTI4, SYSCFG_EXTICR2_EXTI4_PA);
    SET_REG(SYSCFG->EXTICR[2], SYSCFG_EXTICR2_EXTI6, SYSCFG_EXTICR2_EXTI6_PA);
    SET_REG(SYSCFG->EXTICR[2], SYSCFG_EXTICR2_EXTI7, SYSCFG_EXTICR2_EXTI7_PA);
    SET_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR3_EXTI8, SYSCFG_EXTICR3_EXTI8_PA);
}

void NVIC_config() {
    NVIC->ISER[0] |= 0b1 << EXTI4_IRQn;
    NVIC->ISER[0] |= 0b1 << EXTI9_5_IRQn;
}

void EXTI4_IRQHandler() {
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF4)) {
        if (blink_cnt <= 0) blink_cnt = keypad_scan(0) * 2;
        SET_REG(EXTI->PR1, EXTI_PR1_PIF4, EXTI_PR1_PIF4);
    }
}

void EXTI9_5_IRQHandler() {
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF6)) {
        if (blink_cnt <= 0) blink_cnt = keypad_scan(1) * 2;
        SET_REG(EXTI->PR1, EXTI_PR1_PIF6, EXTI_PR1_PIF6);
    }
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF7)) {
        if (blink_cnt <= 0) blink_cnt = keypad_scan(2) * 2;
        SET_REG(EXTI->PR1, EXTI_PR1_PIF7, EXTI_PR1_PIF7);
    }
    if (GET_REG(EXTI->PR1, EXTI_PR1_PIF8)) {
        if (blink_cnt <= 0) blink_cnt = keypad_scan(3) * 2;
        SET_REG(EXTI->PR1, EXTI_PR1_PIF8, EXTI_PR1_PIF8);
    }
}

uint32_t x_pin[4] = {9, 10, 11, 12};
int keypad_scan(int target) {
    int result = -1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                GPIOA->BSRR |= 1 << x_pin[j];
            } else {
                GPIOA->BRR |= 1 << x_pin[j];
            }
        }
        int cnt = 0;
        for (int k = 0; k < 100; k++) {
            uint32_t tmp = ((GPIOA->IDR >> 4) & 0x1) + ((GPIOA->IDR >> 6) << 1);
            uint32_t on = (tmp) & (0x1 << target);
            if (on) cnt++;
        }
        if (cnt >= 99) result = key_map[i][target];
        for (int j = 0; j < 4; j++) {
            GPIOA->BSRR |= 1 << x_pin[j];
        }
    }
    return result;
}

void SysTick_Handler(void) {
    if (blink_cnt <= 0) return;
    if (blink_cnt & 1) {
        GPIOA->BSRR |= 1 << 5;
    } else {
        GPIOA->BRR |= 1 << 5;
    }
    blink_cnt--;
}

int main() {
    SystemClock_Config();
    GPIO_init();
    NVIC_config();
    EXTI_config();

    for (int i = 0; i < 4; i++) {
        GPIOA->BSRR |= 1 << x_pin[i];
    }
    GPIOA->BSRR |= 1 << 5;
    SysTick_Config(16e6 / 16 / 2);
    while (1)
        ;
}
