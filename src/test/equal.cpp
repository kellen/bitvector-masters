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

TestSBox::EqualTest::EqualTest() : FullTestBase(), v(*this, 2, TESTBITS) {
    equal(*this, v[0], v[1]);
    Rnd r(1U);
    branch(*this, v, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}

TestSBox::EqualTest::EqualTest(bool share, EqualTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
}

Space* TestSBox::EqualTest::copy(bool share) {
    return new EqualTest(share, *this);
}

bool TestSBox::EqualTest::after(std::ostream& out) const {
    bool success = true;
    if(v[0].upper() == v[0].lower()) {
        if(v[1].upper() == v[1].lower()) {
            BitVar x = v[0];
            BitVar y = v[1];
            if(x.upper() != y.upper()) {
                success = false;
                out << "fail: " << std::bitset<TESTBITS>(x.upper()) 
                    << " != " << std::bitset<TESTBITS>(y.upper()) << std::endl;
            } else if (DEBUG) {
                out << "ok: " << std::bitset<TESTBITS>(x.upper()) 
                    << " == " << std::bitset<TESTBITS>(y.upper()) << std::endl;
            }
        } else {
            success = false;
            out << "unassigned variable y";
            out << std::bitset<TESTBITS>(v[1].upper()) << ":" << std::bitset<TESTBITS>(v[1].lower()) << std::endl;
        }
    } else {
        success = false;
        out << "unassigned variable x"
            << std::bitset<TESTBITS>(v[0].upper()) << ":" << std::bitset<TESTBITS>(v[0].lower()) << std::endl;
    }
    return success;
}
