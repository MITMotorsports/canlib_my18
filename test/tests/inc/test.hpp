#pragma once
#include <random>
#include "bus.hpp"
#include "structs.hpp"
#include "testfamily.hpp"
#include "canlib_testcomp0.hpp"
#include "canlib_testcomp1.hpp"
#include "canlib_testcomp2.hpp"
#include "driver.hpp"

#define get_bitmask(l, r) (!l ? ((1ULL << (r - 1)) - 1) : ((1ULL << (r - 1)) - (1ULL << (l - 1))))

extern std::default_random_engine generator;
extern std::uniform_int_distribution<uint16_t> distribution;

void testArch0();
void testArch1();
void testArch2();
void testPackUnpack();
void testKeys();
void testPeriod();
void testMessageType();
void testFrameConst();
void testIdentify();
void testSend();
