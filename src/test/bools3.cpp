#include "test.h"

using TestSBox::TestBool;

using namespace Gecode;
TestSBox::BoolS3::BoolS3() : TestBool() {
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"
    #include "model/s3.cpp"
    Rnd r(1U);
    branch(*this, x, INT_VAR_NONE(), INT_VAL_RND(r));
}

