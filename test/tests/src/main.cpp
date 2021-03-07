#include "identify.hpp"
#include "send.hpp"
#include "arch.hpp"
#include "message.hpp"
#include "constants.hpp"

int main() {
    testIdentify();
    testSend();
    testArch0();
    testArch1();
    testArch2();
    testKeys();
    testPeriod();
    testMessageType();
    testFrameConst();
    testPackUnpack();
    return 0;
}
