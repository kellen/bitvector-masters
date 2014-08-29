#include <bitset>
#include <climits>
#include "test.h"

using Gecode::Space;
#define DEBUG false
#define LOG(x) if(DEBUG){std::cerr << x;}

#define TESTVARS 4
#define NUM_BOOL_BITS 4

TestSBox::BoolChannelTest::BoolChannelTest() : 
    FullTestBase(), 
    x(*this, TESTVARS * NUM_BOOL_BITS, 0, 1),
    //x(*this, TESTVARS * NUM_BOOL_BITS),
    intvals(*this, TESTVARS, 0, std::pow(2,NUM_BOOL_BITS)) 
{
    /*
	for(int i=0; i<TESTVARS; i++) {
        LOG("solution[" << i << "]: " << solution[i] << " (" << std::bitset<NUM_BOOL_BITS>(solution[i]) << ") bits:");
        for(int bitidx=0; bitidx<NUM_BOOL_BITS; bitidx++) {
            int b = (solution[i] bitand (1U << bitidx)) > 0 ? 1 : 0;
            LOG(b);
            x[(i*NUM_BOOL_BITS)+bitidx] = BoolVar(*this, b, b);
        }
        LOG(std::endl);
	}
    */

    LOG("x.size(): " << x.size() << " intvals.size(): " << intvals.size() << std::endl);
    LOG("I,");
    IntArgs weights(NUM_BOOL_BITS);
    for(int i=0; i<NUM_BOOL_BITS; i++) {
        weights[i] = std::pow(2,i);
        if(DEBUG) {
            std::cerr << "pow(2," << i << ")=" << std::pow(2,i) << std::endl;
        }
    }
    LOG("W,");
    // channel to ints
    for(int i=0; i<TESTVARS; i++) {
        LOG("l:" << i << ",");
        linear(*this, weights, x.slice(i*NUM_BOOL_BITS, 1, NUM_BOOL_BITS), IRT_EQ, intvals[i]);
    }
    LOG("L,");

    Rnd r(1U);
    branch(*this, x, INT_VAR_RND(r), INT_VAL_RND(r));
}

TestSBox::BoolChannelTest::BoolChannelTest(bool share, BoolChannelTest& sb) : FullTestBase(share, sb) {
    x.update(*this, share, sb.x);
    intvals.update(*this, share, sb.intvals);
}

Space* TestSBox::BoolChannelTest::copy(bool share) {
    return new BoolChannelTest(share, *this);
}

bool TestSBox::BoolChannelTest::after(std::ostream& out) const {
    bool success = true;
    for(int i=0; i<TESTVARS * NUM_BOOL_BITS; i++) {
        if(!x[i].assigned()) {
            out << "unassigned variable x[" << i << "]" << std::endl;
            success = false;
        }
    }
    if(!success) {
        return success;
    }

    for(int i=0; i<TESTVARS; i++) {
        if(!intvals[i].assigned()) {
            out << "unassigned variable intvals[" << i << "]" << std::endl;
            success = false;
        }
    }
    if(!success) {
        return success;
    }

    for(int i=0; i<TESTVARS; i++) {
        if(DEBUG) {
            out << i << ": intval: " << intvals[i] << " bools: ";
        }
        int sum = 0;
        for(int j=0; j<NUM_BOOL_BITS; j++) {
            if(DEBUG) {
                out << x[(i*NUM_BOOL_BITS) + j];
            }
            sum = sum + (x[(i*NUM_BOOL_BITS)+j].val() * std::pow(2,j));
        }
        if(DEBUG) {
            out << "(";
            for(int j=0; j<NUM_BOOL_BITS; j++) {
                out << x[(i*NUM_BOOL_BITS) + j].val();
            }
            out << ")";
        }
        if(sum != (intvals[i].val())) {
            if(DEBUG) {
                out << " sum:" << sum << " != " << intvals[i].val();
            }
            success = false;
        }
        if(DEBUG) {
            out<< std::endl;
        }
    }
    if(DEBUG) {
        out << "----------------" << std::endl;
    }
    return success;
}

