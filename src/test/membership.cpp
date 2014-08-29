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

TestSBox::MembershipTest::MembershipTest() : FullTestBase(), 
    v(*this, 1, TESTBITS) ,
    w(*this, 1, 0, (int)std::pow(2,TESTBITS))
{
    membership(*this, v[0], w[0]);
    branch(*this, w, INT_VAR_NONE(), INT_VAL_SPLIT_MAX());
}

TestSBox::MembershipTest::MembershipTest(bool share, MembershipTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
    w.update(*this, share, sb.w);
}

Space* TestSBox::MembershipTest::copy(bool share) {
    return new MembershipTest(share, *this);
}

bool TestSBox::MembershipTest::after(std::ostream& out) const {
    bool success = true;
    if(v[0].upper() == v[0].lower()) {
        if(w[0].assigned()) {
            // once both v and w are assigned, v should be == to w
            if(v[0].upper() != w[0].val()) {
                success = false;
                out << "fail: " << std::bitset<TESTBITS>(v[0].upper()) 
                    << " : " << v[0].upper()
                    << " != " << w[0].val() << std::endl;
            } else if (DEBUG) {
                out << "ok: " << std::bitset<TESTBITS>(v[0].upper()) 
                    << " : " << v[0].upper()
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

