#include "test.h"
#include <bitset>
#include "nonlinearity.h"
#include "util.h"
#include "gecodeutil.h"

// bitvec
#include "bitvec/bitvec.h"
#include "bitvec/branch.h"
#include "bitvec/propagator.h"

using BV::BIT_VAR_NONE;
using BV::BIT_VAL_RND_BIT;
using BV::BIT_VAL_MIN_BIT;

#define DEBUG false
#define LOG(x) if(DEBUG){std::cerr << x;}

TestSBox::BitLast::BitLast() : TestBit() {
    #include "model/setup.cpp"

    // channeling done in base class

    // constraint S-1; implicit by choice of variables (pg 61)
    // constraint S-2; nonlinearity constraint
    #include "model/s2.cpp"
    LOG("nonlinearity,");

    // constraint S-3; distinct per-row
    #include "model/s3.cpp"
    LOG("s-3,");

    // constraints S-4, S-5, S-6
    #include "model/bitvec/s4.cpp"
    #include "model/bitvec/s5.cpp"
    #include "model/bitvec/s6.cpp"
    LOG("s-4&s-5&s-6,");

    // constraint S-7
    #include "model/bitvec/s7.cpp"
    LOG("s-7");

    print(std::cerr);
    LOG("branching now...");
    Rnd r(1U);
    // branching
    branch(*this, x, BIT_VAR_NONE(), BIT_VAL_MIN_BIT());
}
