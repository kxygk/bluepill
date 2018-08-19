
cmake_minimum_required(VERSION 3.0)

enable_language(ASM)
set(STARTUP_FILE "startup_stm32f103xb.s")

add_subdirectory(stm32f1-ll)

add_executable(blinky.elf ${STARTUP_FILE} 
  src/main.c
  src/stm32f1xx_it.c
  src/system_stm32f1xx.c)
target_include_directories(blinky.elf PUBLIC inc)

target_link_libraries( blinky.elf ll )

target_compile_options(ll PUBLIC
  -mthumb
  -mcpu=cortex-m3
  -mfloat-abi=soft
  -mlittle-endian)

set_target_properties(
  blinky.elf 
  PROPERTIES 
  LINK_FLAGS 
  "-TSTM32F103RBTx_FLASH.ld")
