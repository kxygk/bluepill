
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */


#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_spi.h"

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
void SPI1_Init_Pins(void)
{
    // ** See Reference Manual RM0008 for details **
    // SPI1 is on the APB2 peripheral (see section 3.1 of Reference Manual)
    //
    // Because there are a limited amount of pin on the chip
    // SPI1 has to share pins with GPIO.
    // There are two options available as listed in Section: 9.3.10
    //
    // By default it will use the first option - shared pins on GPIOA (PA4-7)
    // uint32_t NSS = LL_GPIO_PIN_4;
    uint32_t SCK = LL_GPIO_PIN_5;
    // uint32_t MISO = LL_GPIO_PIN_6;
    uint32_t MOSI = LL_GPIO_PIN_7;
    //
    // If we want to use the second set of alternate pins we use 
    // LL_GPIO_AF_EnableRemap_SPI1(); // see gpio.h for details

    // First we enable the clock on GPIOA (ie. power it on)
    LL_APB2_GRP1_EnableClock ( LL_APB2_GRP1_PERIPH_GPIOA );
    // And enable the use of alternate functions on pins
    LL_APB2_GRP1_EnableClock ( LL_APB2_GRP1_PERIPH_AFIO );

    // TODO: Figure out what this is all about...
    LL_GPIO_AF_Remap_SWJ_NOJTAG(); //??
    while ( ( AFIO->MAPR & AFIO_MAPR_SWJ_CFG_JTAGDISABLE )  //??
        != AFIO_MAPR_SWJ_CFG_JTAGDISABLE ); //??

    LL_GPIO_SetPinMode ( GPIOA, SCK, LL_GPIO_MODE_ALTERNATE );
    LL_GPIO_SetPinMode ( GPIOA, MOSI, LL_GPIO_MODE_ALTERNATE );

    // And then set their speeds and default direction (pull down)
    LL_GPIO_SetPinSpeed ( GPIOA, SCK, LL_GPIO_SPEED_FREQ_LOW );
    LL_GPIO_SetPinPull ( GPIOA, SCK, LL_GPIO_PULL_DOWN );

    LL_GPIO_SetPinSpeed ( GPIOA, MOSI, LL_GPIO_SPEED_FREQ_LOW );
    LL_GPIO_SetPinPull ( GPIOA, MOSI, LL_GPIO_PULL_DOWN );

    // Once all the pins are enabled correctly we can "power on" SPI1
    LL_APB2_GRP1_EnableClock ( LL_APB2_GRP1_PERIPH_SPI1 );
}


void SPI1_Init_Params(void)
{
    // SPI Parameters are collected into a struct
    LL_SPI_InitTypeDef spi_initstruct;
    spi_initstruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV32;
    spi_initstruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
    spi_initstruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
    spi_initstruct.ClockPolarity     = LL_SPI_POLARITY_HIGH;
    spi_initstruct.BitOrder          = LL_SPI_MSB_FIRST;
    spi_initstruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
    spi_initstruct.NSS               = LL_SPI_NSS_SOFT;
    spi_initstruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
    spi_initstruct.Mode              = LL_SPI_MODE_MASTER;

    LL_SPI_Init(SPI1, &spi_initstruct); // and sent into a Init function
    LL_SPI_Enable(SPI1);
}

#define BLINKY_PIN LL_GPIO_PIN_13
#define TOGGLE_PIN LL_GPIO_PIN_14

int main(void)
{
    SystemClock_Config();

    SPI1_Init_Pins();
    SPI1_Init_Params();

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
