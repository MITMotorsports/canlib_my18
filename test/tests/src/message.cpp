#include "message.hpp"
#include <cassert>
#include <random>

using namespace CANlib;

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

#define CREATE_TEST(ID, BITMASK) \
    void test##ID() { \
        Frame f0; \
        for (int i = 0;i < 8;++i) { \
            f0.data[i] = distribution(generator); \
        } \
        uint64_t bitstring0; \
        to_bitstring(f0.data, &bitstring0); \
        bitstring0 &= BITMASK; \
        ID##_input.pack(f0); \
        Frame f1; \
        ID##_input.unpack(f1); \
        uint64_t bitstring1;\
        to_bitstring(f1.data, &bitstring1); \
        assert(bitstring0 == bitstring1); \
    }


std::default_random_engine generator;
std::uniform_int_distribution<uint16_t> distribution(0,511);

constexpr uint64_t get_bitmask(const int l, const int r) {
    return !l ? ((1ULL << r) - 1) : ((1ULL << r) - (1ULL << (l - 1)));
}

CREATE_TEST(A, get_bitmask(7,56))
CREATE_TEST(B, get_bitmask(5,64))
CREATE_TEST(C, get_bitmask(0,56))
CREATE_TEST(D, get_bitmask(0,18))
CREATE_TEST(E, get_bitmask(0,64))
CREATE_TEST(F, get_bitmask(0,10) | get_bitmask(18, 28) | get_bitmask(36, 46) | get_bitmask(54, 64))
CREATE_TEST(G, get_bitmask(0,60))
CREATE_TEST(H, get_bitmask(0,64))
CREATE_TEST(I, get_bitmask(0,64))
CREATE_TEST(AA, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(BB, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(AAA, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(BBB, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(CCC, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DDD, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DD, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(EE, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(FF, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(GG, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(HH, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(II, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(JJ, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(KK, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(LL, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(MM, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(NN, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(K, get_bitmask(0, 32))
CREATE_TEST(L, get_bitmask(0, 25))
CREATE_TEST(M, get_bitmask(0, 16))
CREATE_TEST(N, get_bitmask(3, 11))


void testPackUnpack() {
    testA();
    testB();
    testC();
    testD();
    testE();
    testF();
    testG();
    testH();
    testI();
    testAA();
    testBB();
    testAAA();
    testBBB();
    testCCC();
    testDDD();
    testDD();
    testEE();
    testFF();
    testGG();
    testHH();
    testII();
    testJJ();
    testKK();
    testLL();
    testMM();
    testNN();
    testK();
    testL();
    testM();
    testN();
}
