
int main(void){
    while(1) {
        (*(volatile unsigned int *)(0x40011010)) = (1 << 13);
        for (int i = 0; i < 1000000; ++i) __asm__("nop");

        (*(volatile unsigned short *)(0x40011014)) = (1 << 13);
        for (int i = 0; i < 500000; ++i) __asm__("nop");
    }
}
