#include <ctype.h>
#include <stdint.h>
#include "stm32l476xx.h"

extern void GPIO_init();
extern void max7219_init();
extern void max7219_send(unsigned char address, unsigned char data);

uint32_t y_pin[4] = {5, 6, 7, 8};
uint32_t x_pin[4] = {9, 10, 11, 12};
char key_map[4][4] = {{'1', '4', '7', '*'},
                      {'2', '5', '8', '0'},
                      {'3', '6', '9', '#'},
                      {'A', 'B', 'C', 'D'}};

void keypad_init() {
    RCC->AHB2ENR = RCC->AHB2ENR | 0x1;
    GPIOA->MODER = 0xA80003FF;
    GPIOA->PUPDR = GPIOA->PUPDR | 0x2A800;
}

int keypad_scan() {
    int flag = 0;
    int sun = 0;
    for (int i = 0; i < 4; i++) {
        GPIOA->MODER = 0xA80003FF | (0x01 << ((9 + i) * 2));
        GPIOA->ODR = 0x1 << x_pin[i];
        for (int j = 0; j < 4; j++) {
            int cnt = 0;
            for (int k = 0; k < 100; k++) {
                uint32_t on = (GPIOA->IDR >> 5) & (0x1 << j);
                if (on) cnt++;
            }
            if (cnt >= 99 && isdigit(key_map[i][j])) {
                sun += key_map[i][j] - '0';
                flag++;
            }
        }
        GPIOA->ODR = 0x0;
        GPIOA->MODER = 0xA80003FF;
    }
    return flag ? sun : -1;
}

void set_limit(int x) { max7219_send(0x0B, x - 1); }

void show_num(int data) {
    if (data == -1) {
        max7219_send(0xC, 0x0);
        return;
    }
    max7219_send(0xC, 0x1);
    if (data == 0) {
        max7219_send(0x0B, 0x0);
        max7219_send(0x01, 0x0);
        return;
    }
    int idx;
    for (idx = 1; data; idx++) {
        int tmp = data % 10;
        data /= 10;
        max7219_send(idx, tmp);
    }
    max7219_send(0x0B, idx - 2);
}

int main() {
    GPIO_init();
    max7219_init();
    keypad_init();
    set_limit(1);
    while (show_num(keypad_scan()), 1)
        ;
    return 0;
}
