#pragma once

// INCLUDE THIS AFTER YOUR DRIVER

///**
#ifndef CANLIB_ARCH
#error "No architecture specified!"
#endif

#if CANLIB_ARCH == STM32F4xx
#include "drivers/inc/stm32f4xx.hpp"
#elif CANLIB_ARCH == STM32F2xx
#include "drivers/inc/stm32f2xx.hpp"
#elif CANLIB_ARCH == TESTFAMILY
//*/
#include "drivers/inc/testfamily.hpp"
///*
#else
#error "Architecture not supported!"
#endif
//*/
#include "bus.hpp"

namespace CANlib {
    Transmit_Error_T TransmitFrame(const Frame &frame, Bus_Names_T bus);
    void ReadFrame(Frame &frame, Bus_Names_T bus);
}
