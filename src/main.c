
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"

int main(void)
{

LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH);

while (1)
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    int i = 200000;/* About 1/4 second delay */
    while (i-- > 0) {
        asm("nop");/* This stops it optimising code out */
    }
  }
}
