#include "test.h"
#include "nonlinearity.h"

using namespace Gecode;

TestSBox::TestS2::TestS2() : TestSBox::TestSBox() {
    #include "model/setup.cpp"
    #include "model/s2.cpp"
	// finally, branch
    Rnd r(1U);
	branch(*this, x, SET_VAR_NONE(), SET_VAL_RND_INC(r));
}
