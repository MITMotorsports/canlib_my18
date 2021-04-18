#ifdef CANLIB_ARCH_STM32F4XX
#include "static.h"

#include <stdint.h>
#include <string.h>
#include "bus.h"
#include "driver.h"
#include "drivers/inc/stm32f4xx.hpp"
#include "stm32f4xx_hal.h"
#ifdef USING_LOGGING_CALLBACK
#include "log.h"
#endif

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern CAN_HandleTypeDef hcan3;

using namespace CANlib;

Transmit_Error_T CANlib::TransmitFrame(const Frame &frame, Bus_Names_T bus_name) {
  Raw_Bus_T raw_bus = GetRawBus(bus);
  int bus_num;
  CAN_HandleTypeDef *hcan;
  switch (raw_bus) {
    case CAN_1:
      hcan    = &hcan1;
      bus_num = 1;
      break;
    case CAN_2:
      hcan    = &hcan2;
      bus_num = 2;
      break;
    case CAN_3:
      hcan    = &hcan3;
      bus_num = 3;
      break;
    default:
      return HAL_ERROR;
  }

  CAN_TxHeaderTypeDef pHeader;
  uint32_t pTxMailbox = 0;

  pHeader.DLC                = frame.dlc;
  pHeader.StdId              = frame.id;
  pHeader.IDE                = frame.extended ? CAN_ID_EXT : CAN_ID_STD;
  pHeader.RTR                = CAN_RTR_DATA;  // Data frame (as opposed to a remote frame)
  pHeader.TransmitGlobalTime = DISABLE;       // Don't replace last 2 bytes of data with TX time.
#ifdef USING_LOGGING_CALLBACK
  log_frame(frame, bus_num);
#else
  UNUSED(bus_num);
#endif
  return HAL_CAN_AddTxMessage(hcan, &pHeader, frame.data, &pTxMailbox);
}

void ReadFrame(Frame &frame, Bus_Names_T bus) {
  Raw_Bus_T raw_bus = GetRawBus(bus);
  CAN_HandleTypeDef *hcan;
  switch (raw_bus) {
    case CAN_1:
      hcan = &hcan1;
      break;
    case CAN_2:
      hcan = &hcan2;
      break;
    case CAN_3:
      hcan = &hcan3;
      break;
    default:
      return;
  }

  uint8_t data[8] = {};
  CAN_RxHeaderTypeDef pHeader;
  for (int fifo = 0; fifo < 2; fifo++) {  // There are 2 receive FIFOs
    if (HAL_CAN_GetRxFifoFillLevel(hcan, fifo) > 0) {
      HAL_CAN_GetRxMessage(hcan, fifo, &pHeader, data);
      frame.id  = pHeader.IDE == CAN_ID_STD ? pHeader.StdId : pHeader.ExtId;
      frame.dlc = pHeader.DLC;

      memcpy(frame.data, data, sizeof(data));
      frame.extended = pHeader.IDE == CAN_ID_EXT;
      return;
    }
  }
}
#endif
