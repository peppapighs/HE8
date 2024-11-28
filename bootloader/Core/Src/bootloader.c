/*
 * bootloader.c
 *
 *  Created on: Nov 3, 2024
 *      Author: Pep
 */

#include "bootloader.h"

#include "tusb.h"

#include "main.h"

//--------------------------------------------------------------------+
// Firmware Metadata
//--------------------------------------------------------------------+

// Get the end of stack address from the linker script to load the boot flag
extern const uint32_t _estack;

// Firmware metadata locating after the application
typedef struct {
  uint32_t magic1;
  uint32_t magic2;
  uint32_t crc32;
} __attribute__((packed)) fw_metadata_t;

//--------------------------------------------------------------------+
// Prototypes
//--------------------------------------------------------------------+

// Read the boot flag from the RAM and reset it back to 0. Return `true` if the
// boot flag is equal to `BOOT_FLAG`
static bool read_and_reset_boot_flag(void);
// Delay then sample the boot button. Return `true` if the button is pressed
static bool sample_button(void);
// Verify the firmware. Return `true` if the firmware is valid and we are safe
// to jump to the application
static bool verify_firmware(void);
static void jump_to_application(void);

//--------------------------------------------------------------------+
// Bootloader Functions
//--------------------------------------------------------------------+

void bootloader_init(void) {
  // Use branch AND here so we don't verify firmware unnecessarily as it is
  // quite expensive
  if (!read_and_reset_boot_flag() && !sample_button() && verify_firmware())
    jump_to_application();

  tusb_init(1, NULL);
}

void bootloader_loop(void) { tud_task(); }

//--------------------------------------------------------------------+
// Helper Functions
//--------------------------------------------------------------------+

static bool read_and_reset_boot_flag(void) {
  uint32_t *boot_flag = (uint32_t *)&_estack;
  bool ret = (*boot_flag == BOOT_FLAG);

  // Reset boot flag
  *boot_flag = 0;

  return ret;
}

static bool sample_button(void) {
  const uint32_t button_delay_start = HAL_GetTick();
  while (HAL_GetTick() < button_delay_start + BUTTON_SAMPLE_DELAY_MS)
    ;
  return (HAL_GPIO_ReadPin(USER_GPIO_Port, USER_Pin) == GPIO_PIN_SET);
}

static bool verify_firmware(void) {
  for (size_t i = 0; i < FW_FLASH_SECTOR_SIZE; i++) {
    const fw_metadata_t metadata =
        *(const fw_metadata_t *)(FW_BASE_ADDRESS + i);
    if (metadata.magic1 == FW_MAGIC1 && metadata.magic2 == FW_MAGIC2) {
      // Perform CRC32 check
      uint32_t crc32 =
          HAL_CRC_Calculate(&hcrc, (uint32_t *)FW_BASE_ADDRESS, i >> 2);

      return (crc32 == metadata.crc32);
    }
  }

  return false;
}

static void jump_to_application(void) {
  void (*app_reset_handler)(void) =
      (void *)(*((volatile uint32_t *)(FW_BASE_ADDRESS + 4U)));

  // De-initialize clock
  HAL_RCC_DeInit();
  HAL_DeInit();

  // Disable all interrupts
  __disable_irq();

  // Disable SysTick
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

  // Clear interrupt enable and pending registers
  for (size_t i = 0; i < 8; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;
    NVIC->ICPR[i] = 0xFFFFFFFF;
  }

  // Set vector address to the application vector table
  SCB->VTOR = FW_BASE_ADDRESS;

  // Set stack pointer to the start of the firmware
  __set_MSP(*(volatile uint32_t *)FW_BASE_ADDRESS);

  // Ensure the VTOR and SP operations are complete
  __DSB();

  // Flush the pipeline because of SP change
  __ISB();

  // Re-enable all interrupts
  __enable_irq();

  // Jump to application
  app_reset_handler();

  while (1)
    ;
}
