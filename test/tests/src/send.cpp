#include <cassert>
#include <random>
#include <chrono>
#include "send.hpp"

using namespace CANlib;
using namespace std::chrono_literals;

extern CAN can1;
extern CAN can2;
extern CAN can3;

extern map1::A_T A_input;
extern map1::B_T B_input;
extern map1::C_T C_input;
extern map1::D_T D_input;
extern map1::E_T E_input;
extern map2::F_T F_input;
extern map2::G_T G_input;
extern map2::H_T H_input;
extern map2::I_T I_input;
extern map2::J::AA_T AA_input;
extern map2::J::BB_T BB_input;
extern map2::J::CC::AAA_T AAA_input;
extern map2::J::CC::BBB_T BBB_input;
extern map2::J::CC::CCC_T CCC_input;
extern map2::J::CC::DDD_T DDD_input;
extern map2::J::DD_T DD_input;
extern map2::J::EE_T EE_input;
extern map2::J::FF_T FF_input;
extern map2::J::GG_T GG_input;
extern map2::J::HH_T HH_input;
extern map2::J::II_T II_input;
extern map2::J::JJ_T JJ_input;
extern map2::J::KK_T KK_input;
extern map2::J::LL_T LL_input;
extern map2::J::MM_T MM_input;
extern map2::J::NN_T NN_input;
extern map2::K_T K_input;
extern map2::L_T L_input;
extern map2::M_T M_input;
extern map2::N_T N_input;

std::default_random_engine generator;
std::uniform_int_distribution<uint16_t> distribution(0,511);

constexpr uint64_t get_bitmask(const int l, const int r) {
    return !l ? ((1ULL << r) - 1) : ((1ULL << r) - (1ULL << (l - 1)));
}

#define CREATE_TEST(ID, can_idx, BITMASK) \
    static void testSend##ID() { \
         can##can_idx.clear(); \
         auto starting_time = Clock::now(); \
         Frame f0; \
         for (int i = 0;i < 8;++i) { \
             f0.data[i] = distribution(generator); \
         } \
         uint64_t bitstring0; \
         to_bitstring(f0.data, &bitstring0); \
         bitstring0 &= BITMASK; \
         ID##_input.unpack(f0); \
         while (Clock::now() - starting_time <= ID##_input.period_ + 5ms) { \
            ID##_input.send(); \
         } \
         ID##_input.send(); \
         uint64_t bitstring1; \
         assert(can##can_idx.framesReceived() == 1); \
         Frame f1 = can##can_idx.topFrame(); \
         to_bitstring(f1.data, &bitstring1); \
         bitstring1 &= BITMASK;\
         assert(bitstring0 == bitstring1); \
    } \

CREATE_TEST(A, 2, get_bitmask(7,56))
CREATE_TEST(B, 2, get_bitmask(5,64))
CREATE_TEST(C, 2, get_bitmask(0,56))
CREATE_TEST(D, 2, get_bitmask(0,18))
CREATE_TEST(E, 2, get_bitmask(0,64))
CREATE_TEST(F, 1, get_bitmask(0,10) | get_bitmask(18, 28) | get_bitmask(36, 46) | get_bitmask(54, 64))
CREATE_TEST(G, 1, get_bitmask(0,60))
CREATE_TEST(H, 1, get_bitmask(0,64))
CREATE_TEST(I, 1, get_bitmask(0,64))
CREATE_TEST(AA, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(BB, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(AAA, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(BBB, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(CCC, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DDD, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DD, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(EE, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(FF, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(GG, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(HH, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(II, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(JJ, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(KK, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(LL, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(MM, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(NN, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(K, 1, get_bitmask(0, 32))
CREATE_TEST(L, 1, get_bitmask(0, 25))
CREATE_TEST(M, 1, get_bitmask(0, 16))
CREATE_TEST(N, 1, get_bitmask(3, 11))

void testSend() {
  testSendA();
  testSendB();
  testSendC();
  testSendD();
  testSendE();
  testSendF();
  testSendG();
  testSendH();
  testSendI();
  testSendAA();
  testSendBB();
  testSendAAA();
  testSendBBB();
  testSendCCC();
  testSendDDD();
  testSendDD();
  testSendEE();
  testSendFF();
  testSendGG();
  testSendHH();
  testSendII();
  testSendJJ();
  testSendKK();
  testSendLL();
  testSendMM();
  testSendNN();
  testSendK();
  testSendL();
  testSendM();
  testSendN();
}
