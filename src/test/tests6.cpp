#include "test.h"

using namespace Gecode;

TestSBox::TestS6::TestS6() : TestSBox::TestSBox() {
    #include "model/setup.cpp"
    #include "model/set/s6.cpp"

	// finally, branch
    Rnd r(1U);
	branch(*this, x, SET_VAR_NONE(), SET_VAL_RND_INC(r));
}
