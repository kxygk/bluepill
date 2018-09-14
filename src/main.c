
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */


#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"

void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
  LL_RCC_HSE_DisableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}

#define BLINKY_PIN LL_GPIO_PIN_13
#define TOGGLE_PIN LL_GPIO_PIN_14

int main(void)
{
    SystemClock_Config();

    // enabling the clock on a peripheral effectively powers it on
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

    // set pin to be an output pin
    LL_GPIO_SetPinMode(GPIOC, BLINKY_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOC, BLINKY_PIN, LL_GPIO_SPEED_FREQ_LOW);

    // set pin to be an input pin
    LL_GPIO_SetPinMode(GPIOC, TOGGLE_PIN, LL_GPIO_MODE_INPUT);

while (1)
  {
    /* Insert delay 250 ms */
    LL_mDelay(250);
    if(!LL_GPIO_IsInputPinSet (GPIOC, TOGGLE_PIN))
    {
        LL_GPIO_TogglePin(GPIOC, BLINKY_PIN);
    }
  }
}


#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
