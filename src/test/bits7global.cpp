#include "test.h"
#include "bitvec/branch.h"
#include "bitvec/propagator.h"

using TestSBox::TestBit;
using BV::BIT_VAR_NONE;
using BV::BIT_VAL_RND_BIT;

using namespace Gecode;
TestSBox::BitS7Global::BitS7Global() : TestBit() {
    #include "model/setup.cpp"
    #include "model/bitvec/s7global.cpp"

    Rnd r(1U);
    branch(*this, x, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}

