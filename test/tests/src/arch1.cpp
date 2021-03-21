#include <cassert>
#include "test.hpp"

using namespace CANlib;
using namespace testcomp1;

extern CAN can1;
extern CAN can2;
extern CAN can3;

void testArch1() {
    assert(GetRawBus(Bus_Names_T::map1) == Raw_Bus_T::CAN_2);
    assert(GetRawBus(Bus_Names_T::map2) == Raw_Bus_T::CAN_3);
    assert(GetRawBus(Bus_Names_T::INVALID_NAME) == Raw_Bus_T::INVALID_BUS);
    assert(GetBusName(Raw_Bus_T::CAN_2) == Bus_Names_T::map1);
    assert(GetBusName(Raw_Bus_T::CAN_3) == Bus_Names_T::map2);
    assert(GetBusName(Raw_Bus_T::CAN_1) == Bus_Names_T::INVALID_NAME);
    assert(GetBusName(Raw_Bus_T::INVALID_BUS) == Bus_Names_T::INVALID_NAME);
    can1.clear();
    can2.clear();
    can3.clear();
    update_can();
    assert(0 == can1.framesReceived());
    assert(1 == can2.framesReceived());
    assert(1 == can3.framesReceived());
    can1.clear();
    can2.clear();
    can3.clear();
    Frame f;
    f.id = 69;
    can2.setFrameToSend(f);
    testcomp1::map1::update_can();
    assert(69 == can2.topFrame().id);
    assert(0 == can1.framesReceived());
    assert(1 == can2.framesReceived());
    assert(0 == can3.framesReceived());
    can1.clear();
    can2.clear();
    can3.clear();
    f.id = 69;
    can3.setFrameToSend(f);
    testcomp1::map2::update_can();
    assert(69 == can3.topFrame().id);
    assert(0 == can1.framesReceived());
    assert(0 == can2.framesReceived());
    assert(1 == can3.framesReceived());
}
