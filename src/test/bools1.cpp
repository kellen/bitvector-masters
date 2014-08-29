#include "test.h"

using TestSBox::TestBool;

using namespace Gecode;
TestSBox::BoolS1::BoolS1() : TestBool() {
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"
    Rnd r(1U);
    branch(*this, x, INT_VAR_NONE(), INT_VAL_RND(r));
}

