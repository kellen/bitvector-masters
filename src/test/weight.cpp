#include <bitset>
#include <climits>
#include "test.h"
#include "bitvec/branch.h"
#include "gecodeutil.h"
#include "bitvec/propagator.h"
#include "util.h"

using Gecode::Space;
using BV::BIT_VAR_NONE;
using BV::BIT_VAL_RND_BIT;

#define TESTBITS 4
#define DEBUG false

TestSBox::WeightTest::WeightTest() : FullTestBase(), 
    v(*this, 1, TESTBITS) ,
    w(*this, 1, 0, (int)std::pow(2,TESTBITS))
{
    weight(*this, v[0], w[0]);
    Rnd r(1U);
    branch(*this, v, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}

TestSBox::WeightTest::WeightTest(bool share, WeightTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
    w.update(*this, share, sb.w);
}

Space* TestSBox::WeightTest::copy(bool share) {
    return new WeightTest(share, *this);
}

bool TestSBox::WeightTest::after(std::ostream& out) const {
    bool success = true;
    if(v[0].upper() == v[0].lower()) {
        if(w[0].assigned()) {
            if(std::bitset<TESTBITS>(v[0].upper()).count() != w[0].val()) {
                success = false;
                out << "fail: " << std::bitset<TESTBITS>(v[0].upper()) 
                    << " cnt " << std::bitset<TESTBITS>(v[0].upper()).count()
                    << " != " << w[0].val() << std::endl;
            } else if (DEBUG) {
                out << "ok: " << std::bitset<TESTBITS>(v[0].upper()) 
                    << " cnt " << std::bitset<TESTBITS>(v[0].upper()).count()
                    << " == " << w[0].val() << std::endl;
            }
        } else {
            success = false;
            out << "unassigned variable w"
                << ": (" << w[0].min() << "," << w[0].max() << ")" << std::endl;
        }
    } else {
        success = false;
        out << "unassigned variable v"
            << std::bitset<TESTBITS>(v[0].upper()) << ":" << std::bitset<TESTBITS>(v[0].lower()) << std::endl;
    }
    return success;
}
