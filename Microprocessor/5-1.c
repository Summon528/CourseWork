extern void GPIO_init();
extern void max7219_init();
extern void max7219_send(unsigned char address, unsigned char data);

void display(int data, int num_digits) {
    max7219_send(0x0B, num_digits-1);
    for(int idx = 1; idx <= num_digits; idx++) {
        int tmp = data % 10;
        data /= 10;
        max7219_send(idx, tmp);
    }
}


int main() {
    int student_id = 611262;
    GPIO_init();
    max7219_init();
    display(student_id, 7);
    return 0;
}
