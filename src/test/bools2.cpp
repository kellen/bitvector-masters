#include "test.h"
#include "nonlinearity.h"

using TestSBox::TestBool;

using namespace Gecode;
TestSBox::BoolS2::BoolS2() : TestBool() {
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"
    #include "model/s2.cpp"

    Rnd r(1U);
    branch(*this, x, INT_VAR_NONE(), INT_VAL_RND(r));
}

