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

TestSBox::ChannelTest::ChannelTest() : FullTestBase(), 
    v(*this, 1, TESTBITS) ,
    w(*this, 1, 0, (int)std::pow(2,TESTBITS))
{
    channel(*this, v[0], w[0]);
    Rnd r(1U);
    branch(*this, v, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));
    branch(*this, w, INT_VAR_NONE(), INT_VAL_SPLIT_MAX());
}

TestSBox::ChannelTest::ChannelTest(bool share, ChannelTest& sb) : FullTestBase(share, sb) {
    v.update(*this, share, sb.v);
    w.update(*this, share, sb.w);
}

Space* TestSBox::ChannelTest::copy(bool share) {
    return new ChannelTest(share, *this);
}

bool TestSBox::ChannelTest::after(std::ostream& out) const {
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


