
cmake_minimum_required(VERSION 3.0)
project(blinky)

enable_language(ASM)
set(STARTUP_FILE "startup_stm32f103xb.s")

add_subdirectory(stm32f1-ll)

add_executable(${PROJECT_NAME}.elf ${STARTUP_FILE} 
  src/main.c
  src/stm32f1xx_it.c
  src/system_stm32f1xx.c)
target_include_directories(${PROJECT_NAME}.elf PUBLIC inc)

target_link_libraries(${PROJECT_NAME}.elf ll )

target_compile_options(${PROJECT_NAME}.elf PUBLIC
  -Wall 
  -g 
  -std=gnu99 
  -Os
  -mthumb
  -mcpu=cortex-m3
  -mfloat-abi=soft
  -mlittle-endian
  -ffunction-sections 
  -fdata-sections
  -Werror 
  -Wstrict-prototypes 
  -Warray-bounds 
  -fno-strict-aliasing 
  -Wno-unused-const-variable 
  -specs=nano.specs 
  -specs=nosys.specs)

set_target_properties(
  ${PROJECT_NAME}.elf 
  PROPERTIES 
  LINK_FLAGS 
  "-T${PROJECT_SOURCE_DIR}/STM32F103RBTx_FLASH.ld \
   -Wl,--gc-sections \
   -Wl,-Map=${PROJECT_NAME}.map")

file(COPY
  openocd.cfg
  DESTINATION
  ${CMAKE_BINARY_DIR})
