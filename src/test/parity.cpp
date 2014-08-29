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

TestSBox::ParityTest::ParityTest() : FullTestBase(), 
    v(*this, 1, TESTBITS) ,
    w(*this, 1, 0, 1)
{
    parity(*this, v[0], w[0]);
    Rnd r(1U);
    branch(*this, v, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}

TestSBox::ParityTest::ParityTest(bool share, ParityTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
    w.update(*this, share, sb.w);
}

Space* TestSBox::ParityTest::copy(bool share) {
    return new ParityTest(share, *this);
}

bool TestSBox::ParityTest::after(std::ostream& out) const {
    bool success = true;
    if(v[0].upper() == v[0].lower()) {
        if(w[0].assigned()) {
            bool val = w[0].val() == 1;

            if(parity(v[0].upper()) != val) {
                success = false;
                out << "fail: " << std::bitset<TESTBITS>(v[0].upper()) 
                    << " par " << parity(v[0].upper())
                    << " != " << val << std::endl;
            } else if (DEBUG) {
                out << "fail: " << std::bitset<TESTBITS>(v[0].upper()) 
                    << " par " << parity(v[0].upper())
                    << " != " << val << std::endl;
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
