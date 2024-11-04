/*
 * dfu.c
 *
 *  Created on: Nov 3, 2024
 *      Author: Pep
 */

#include "stm32f4xx_hal.h"

#include "class/dfu/dfu_device.h"

//--------------------------------------------------------------------+
// DFU callbacks
// Note: alt is used as the partition number, in order to support multiple
// partitions like FLASH, EEPROM, etc.
//--------------------------------------------------------------------+

// Invoked right before tud_dfu_download_cb() (state=DFU_DNBUSY) or
// tud_dfu_manifest_cb() (state=DFU_MANIFEST) Application return timeout in
// milliseconds (bwPollTimeout) for the next download/manifest operation. During
// this period, USB host won't try to communicate with us.
uint32_t tud_dfu_get_timeout_cb(uint8_t alt, uint8_t state) {
  if (state == DFU_DNBUSY)
    // 1ms should be fast enough for flashing single block
    return 1;

  return 0;
}

// Invoked when received DFU_DNLOAD (wLength>0) following by DFU_GETSTATUS
// (state=DFU_DNBUSY) requests This callback could be returned before flashing
// op is complete (async). Once finished flashing, application must call
// tud_dfu_finish_flashing()
void tud_dfu_download_cb(uint8_t alt, uint16_t block_num, uint8_t const *data,
                         uint16_t length) {
  if (HAL_FLASH_Unlock() != HAL_OK) {
    tud_dfu_finish_flashing(DFU_STATUS_ERR_WRITE);
    return;
  }

  if (block_num == 0)
    // If this is the first block, we should erase the sector
    FLASH_Erase_Sector(FW_FLASH_SECTOR, VOLTAGE_RANGE_1);

  const uint32_t start_address =
      FW_BASE_ADDRESS + (block_num * CFG_TUD_DFU_XFER_BUFSIZE);
  for (uint16_t i = 0; i < length; i++) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, start_address + i, data[i]) !=
        HAL_OK) {
      tud_dfu_finish_flashing(DFU_STATUS_ERR_WRITE);
      return;
    }
  }

  if (HAL_FLASH_Lock() != HAL_OK) {
    tud_dfu_finish_flashing(DFU_STATUS_ERR_WRITE);
    return;
  }

  // flashing op for download complete without error
  tud_dfu_finish_flashing(DFU_STATUS_OK);
}

// Invoked when download process is complete, received DFU_DNLOAD (wLength=0)
// following by DFU_GETSTATUS (state=Manifest) Application can do checksum, or
// actual flashing if buffered entire image previously. Once finished flashing,
// application must call tud_dfu_finish_flashing()
void tud_dfu_manifest_cb(uint8_t alt) {
  // Reset after we finish downloading
  NVIC_SystemReset();
}

// Invoked when the Host has terminated a download or upload transfer
void tud_dfu_abort_cb(uint8_t alt) {
  // Reset if the transfer is aborted
  NVIC_SystemReset();
}

// Invoked when a DFU_DETACH request is received
void tud_dfu_detach_cb(void) {
  // Reset if requested to detach
  NVIC_SystemReset();
}
