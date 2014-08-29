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

TestSBox::ExorTest::ExorTest() : FullTestBase(), v(*this, 3, TESTBITS) {
    exor(*this, v[0], v[1], v[2]);
    Rnd r(1U);
    branch(*this, v, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
}

TestSBox::ExorTest::ExorTest(bool share, ExorTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
}

Space* TestSBox::ExorTest::copy(bool share) {
    return new ExorTest(share, *this);
}

bool TestSBox::ExorTest::after(std::ostream& out) const {
    bool success = true;
    if(v[0].upper() == v[0].lower()) {
        if(v[1].upper() == v[1].lower()) {
            if(v[2].upper() == v[2].lower()) {
                BitVar x = v[0];
                BitVar y = v[1];
                BitVar z = v[2];
                BitType xxory = (BitType)(x.upper() xor y.upper());
                if(xxory != z.upper()) {
                    success = false;
                    out << "fail: " << std::bitset<TESTBITS>(z.upper()) 
                        << " != " << std::bitset<TESTBITS>(xxory) << std::endl;
                } else if (DEBUG) {
                    out << "ok: " << std::bitset<TESTBITS>(z.upper()) 
                        << " == " << std::bitset<TESTBITS>(xxory) << std::endl;
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
