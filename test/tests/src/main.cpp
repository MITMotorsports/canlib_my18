#include "test.hpp"

std::default_random_engine generator;
std::uniform_int_distribution<uint16_t> distribution(0,511);

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
