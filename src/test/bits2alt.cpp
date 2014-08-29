#include "test.h"
//#include "nonlinearity.h"
#include "bitvec/branch.h"
#include "bitvec/propagator.h"
#include "util.h"

using TestSBox::TestBit;
using BV::BIT_VAR_NONE;
using BV::BIT_VAL_RND_BIT;

using namespace Gecode;
TestSBox::BitS2Alt::BitS2Alt() : TestBit() {
    #include "model/setup.cpp"
    #include "model/bitvec/s2decomp.cpp"

    Rnd r(1U);
    branch(*this, x, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}
