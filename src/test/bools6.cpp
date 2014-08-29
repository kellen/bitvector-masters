#include "test.h"

using TestSBox::TestBool;

using namespace Gecode;
TestSBox::BoolS6::BoolS6() : TestBool() {
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"
    #include "model/boolean/s6.cpp"

    Rnd r(1U);
    branch(*this, x, INT_VAR_NONE(), INT_VAL_RND(r));
}

