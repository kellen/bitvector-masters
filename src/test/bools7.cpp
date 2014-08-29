#include "test.h"

using TestSBox::TestBool;

using namespace Gecode;
TestSBox::BoolS7::BoolS7() : TestBool() {
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"
    #include "model/boolean/s7.cpp"

    Rnd r(1U);
    branch(*this, x, INT_VAR_NONE(), INT_VAL_RND(r));
}


