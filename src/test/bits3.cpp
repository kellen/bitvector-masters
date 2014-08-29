#include "test.h"
#include "bitvec/branch.h"

using TestSBox::TestBit;
using BV::BIT_VAR_NONE;
using BV::BIT_VAL_RND_BIT;

using namespace Gecode;
TestSBox::BitS3::BitS3() : TestBit() {
    #include "model/setup.cpp"
    #include "model/s3.cpp"
    Rnd r(1U);
    branch(*this, x, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}
