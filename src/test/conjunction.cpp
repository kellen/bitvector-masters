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

TestSBox::ConjunctionTest::ConjunctionTest() : FullTestBase(), v(*this, 3, TESTBITS) {
    conjunction(*this, v[0], v[1], v[2]);
    Rnd r(1U);
    branch(*this, v, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}

TestSBox::ConjunctionTest::ConjunctionTest(bool share, ConjunctionTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
}

Space* TestSBox::ConjunctionTest::copy(bool share) {
    return new ConjunctionTest(share, *this);
}

bool TestSBox::ConjunctionTest::after(std::ostream& out) const {
    bool success = true;
    if(v[0].upper() == v[0].lower()) {
        if(v[1].upper() == v[1].lower()) {
            if(v[2].upper() == v[2].lower()) {
                BitVar x = v[0];
                BitVar y = v[1];
                BitVar z = v[2];
                BitType xandy = (BitType)(x.upper() bitand y.upper());
                if(xandy != z.upper()) {
                    success = false;
                    out << "fail: " << std::bitset<TESTBITS>(z.upper()) 
                        << " != " << std::bitset<TESTBITS>(xandy) << std::endl;
                } else if (DEBUG) {
                    out << "ok: " << std::bitset<TESTBITS>(z.upper()) 
                        << " == " << std::bitset<TESTBITS>(xandy) << std::endl;
                }
            } else {
                success = false;
                out << "unassigned variable z" ;
                out << std::bitset<TESTBITS>(v[2].upper()) << ":" << std::bitset<TESTBITS>(v[2].lower()) << std::endl;
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
