#include "identify.hpp"
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

extern Message_T** messages[2];

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0,511);

#define CREATE_TEST0(ID, bus_idx, key) \
    static void test##ID() { \
        for (int cs = 0;cs < 100;++cs) { \
            Frame f; \
            f.dlc = distribution(generator); \
            for (int i = 0;i < 8;++i) { \
                f.data[i] = distribution(generator); \
            } \
            f.id = key; \
            Message_T* msg = messages[bus_idx][Identify(Bus_Names_T::map##bus_idx, f)]; \
            assert(msg == (&ID##_input)); \
        } \
    }

#define CREATE_TEST1(ID, bus_idx, key) \
    static void test##ID() { \
        Frame f; \
        ID##_input.unpack(f); \
        Message_T* msg = messages[bus_idx][Identify(Bus_Names_T::map##bus_idx, f)]; \
        assert(msg == (&ID##_input)); \
    }

CREATE_TEST0(A, 1, 0X2FF)
CREATE_TEST0(B, 1, 0X305)
CREATE_TEST0(C, 1, 0X306)
CREATE_TEST0(D, 1, 0X307)
CREATE_TEST0(E, 1, 0X308)
CREATE_TEST0(F, 2, 0X016)
CREATE_TEST0(G, 2, 0X017)
CREATE_TEST0(H, 2, 0X0A0)
CREATE_TEST0(I, 2, 0XA2)
CREATE_TEST1(AA, 2, 0)
CREATE_TEST1(BB, 2, 0X14)
CREATE_TEST1(AAA, 2, 0X69)
CREATE_TEST1(BBB, 2, 0X70)
CREATE_TEST1(CCC, 2, 0X71)
CREATE_TEST1(DDD, 2, 0X72)
CREATE_TEST1(DD, 2, 0X65)
CREATE_TEST1(EE, 2, 0X66)
CREATE_TEST1(FF, 2, 0X67)
CREATE_TEST1(GG, 2, 0X68)
CREATE_TEST1(HH, 2, 0X6A)
CREATE_TEST1(II, 2, 0X6B)
CREATE_TEST1(JJ, 2, 0X6C)
CREATE_TEST1(KK, 2, 0X6D)
CREATE_TEST1(LL, 2, 0X6F)
CREATE_TEST1(MM, 2, 0XAB)
CREATE_TEST1(NN, 2, 0X91)
CREATE_TEST0(K, 2, 0X0BB)
CREATE_TEST0(L, 2, 0X0DF)
CREATE_TEST0(M, 2, 0X0F0)
CREATE_TEST0(N, 2, 0X400)

void testIdentify() {
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
