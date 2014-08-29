#include "bitvec/propagator.h"

#include "util.h"
#include "bitvec/exception.h"

using BV::Bit::GeneralError;

#define DEBUG false
#define LOG(x) if(DEBUG){std::cerr << x;}
#define BIT_ME_CHECK(x) GECODE_ME_CHECK(x) 
//#define BIT_ME_CHECK(x) if(me_failed(x)) { LOG("f,"); return ES_FAILED; }

// propagator implementations

// equality (=)
Equal::Equal(Home home, BitView x0, BitView x1) : BinaryPropagator<BitView,PC_BIT_BND>(home, x0, x1) {}
Equal::Equal(Space& home, bool share, Equal& p) : BinaryPropagator<BitView,PC_BIT_BND>(home, share, p) {}
ExecStatus Equal::post(Home home, BitView x0, BitView x1) {
    if(!same(x0, x1)) {
        (void) new (home) Equal(home, x0, x1);
    }
    return ES_OK;
}
Propagator* Equal::copy(Space& home, bool share) {
    return new (home) Equal(home, share, *this);
}
ExecStatus Equal::propagate(Space& home, const ModEventDelta&) {
    LOG("=");
    // fail if both are already assigned but not equal
    if(x0.assigned() && x1.assigned()) {
        LOG(std::endl << "b:" << std::bitset<NUMBITS>(x0.lower()) << " = " << std::bitset<NUMBITS>(x1.lower()) );
        if(x0.lower() != x1.lower() || x0.upper() != x1.upper()) {
            LOG("f,");
            return ES_FAILED;
        }
        LOG("s,");
        return home.ES_SUBSUMED(*this);
    }
    // fixpoint if the bounds are already the same
    if(x0.lower() == x1.lower() && x0.upper() == x1.upper()) {
        LOG(",");
        return ES_FIX;
    }
    // adjust the bounds
    BitType newl = x0.lower() bitor x1.lower();
    BitType newu = x0.upper() bitand x1.upper();
    BIT_ME_CHECK(x0.bounds(home, newu, newl));
    BIT_ME_CHECK(x1.bounds(home, newu, newl));
    // if everything's assigned now, we're done
    if(x0.assigned() && x1.assigned()) {
        LOG(std::endl << "b:" << std::bitset<NUMBITS>(x0.lower()) << " = " << std::bitset<NUMBITS>(x1.lower()) );
        LOG("s,");
        return home.ES_SUBSUMED(*this);
    }
    LOG(",");
    return ES_FIX;
}
void equal(Space& home, BitVar x0, BitVar x1) {
    if(home.failed()) {
        return;
    }
    GECODE_ES_FAIL(Equal::post(home, x0, x1));
}

// isequality x0 != x1
Disequal::Disequal(Home home, BitView x0, BitView x1) : BinaryPropagator<BitView,PC_BIT_BND>(home, x0, x1) {}
Disequal::Disequal(Space& home, bool share, Disequal& p) : BinaryPropagator<BitView,PC_BIT_BND>(home, share, p) {}
ExecStatus Disequal::post(Home home, BitView x0, BitView x1) {
    if(same(x0, x1)) {
        return ES_FAILED;
    }
    (void) new (home) Disequal(home, x0, x1);
    return ES_OK;
}
Propagator* Disequal::copy(Space& home, bool share) {
    return new (home) Disequal(home, share, *this);
}
ExecStatus Disequal::propagate(Space& home, const ModEventDelta&) {
    // check for subsumption
    BitType fixedBoth = fixed(x0.upper(), x0.lower()) bitand fixed(x1.upper(), x1.lower());
    if((x0.lower() bitand fixedBoth) != (x1.lower() bitand fixedBoth)) {
        return home.ES_SUBSUMED(*this);
    }
    // both assigned and equal; fail
    if(x0.assigned() && x1.assigned() && x0.val() == x1.val()) {
        return ES_FAILED;
    }
    // propagate the last bit if possible
    if((x0.assigned() && !x1.assigned()) 
            || (!x0.assigned() && x1.assigned())) {
        BitType free_x1 = free(x1.upper(), x1.lower());
        if(x0.assigned() && (__builtin_popcount(free_x1) == 1)) {
            BitType val = x1.upper() bitand (compl (x0.lower() bitand free_x1));
            BIT_ME_CHECK(x1.bounds(home, val, val));
            return home.ES_SUBSUMED(*this);
        }
        BitType free_x0 = free(x0.upper(), x0.lower());
        if(x1.assigned() && (__builtin_popcount(free_x0) == 1)) {
            BitType val = x0.upper() bitand (compl (x1.lower() bitand free_x0));
            BIT_ME_CHECK(x0.bounds(home, val, val));
            return home.ES_SUBSUMED(*this);
        }
    }
    return ES_FIX;
}
void disequal(Space& home, BitVar x0, BitVar x1) {
    if(home.failed()) {
        return;
    }
    GECODE_ES_FAIL(Disequal::post(home, x0, x1));
}

// exclusive or: x0 xor x1 = x2
Exor::Exor(Home home, BitView x0, BitView x1, BitView x2) : TernaryPropagator<BitView,PC_BIT_BND>(home, x0, x1, x2) {}
Exor::Exor(Space& home, bool share, Exor& p) : TernaryPropagator<BitView,PC_BIT_BND>(home, share, p) {}
ExecStatus Exor::post(Home home, BitView x0, BitView x1, BitView x2) {
    (void) new (home) Exor(home, x0, x1, x2);
    return ES_OK;
}
Propagator* Exor::copy(Space& home, bool share) {
    return new (home) Exor(home, share, *this);
}
ExecStatus Exor::propagate(Space& home, const ModEventDelta&) {
    LOG("^" << std::endl);
    //  x xor  y =  z
    // x0 xor x1 = x2
    BitType ux = x0.upper();
    BitType uy = x1.upper();
    BitType uz = x2.upper();
    BitType lx = x0.lower();
    BitType ly = x1.lower();
    BitType lz = x2.lower();

    BitType upx  = ux bitand (uz bitor uy) bitand (compl (ly bitand lz));
    BitType lowx = lx bitor ((compl uz) bitand ly) bitor (lz bitand (compl uy));
    BIT_ME_CHECK(x0.bounds(home, upx, lowx));

    BitType upy  = uy bitand (uz bitor ux) bitand (compl (lx bitand lz));
    BitType lowy = ly bitor ((compl uz) bitand lx) bitor (lz bitand (compl ux));
    BIT_ME_CHECK(x1.bounds(home, upy, lowy));

    BitType upz  = uz bitand (ux bitor uy) bitand (compl (lx bitand ly));
    BitType lowz = lz bitor ((compl ux) bitand ly) bitor (lx bitand (compl uy));
    BIT_ME_CHECK(x2.bounds(home, upz, lowz));

    if(x0.assigned() && x1.assigned() && x2.assigned()) {
        LOG("b:" << std::bitset<NUMBITS>(x0.lower()) << "^" << std::bitset<NUMBITS>(x1.lower()) << " = " << std::bitset<NUMBITS>(x2.lower()) );
        LOG("s," << std::endl);
        return home.ES_SUBSUMED(*this);
    } else {
        LOG("b:" 
                << std::bitset<NUMBITS>(x0.lower()) 
                << " -- "
                << std::bitset<NUMBITS>(x0.upper()) 
                << std::endl
                << " ^" 
                << std::bitset<NUMBITS>(x1.lower()) 
                << " -- "
                << std::bitset<NUMBITS>(x1.upper()) 
                << std::endl
                << " =" 
                << std::bitset<NUMBITS>(x2.lower()) 
                << " -- "
                << std::bitset<NUMBITS>(x2.upper()) 
           );
    }
    LOG("f," << std::endl );
    return ES_FIX;
}
void exor(Space& home, BitVar x0, BitVar x1, BitVar x2) {
    if(home.failed()) {
        return;
    }
    GECODE_ES_FAIL(Exor::post(home, x0, x1, x2));
}

// parity: channel the parity of x0 to x1
Parity::Parity(Home home, BitView x0, BoolView x1) : MixBinaryPropagator<BitView,PC_BIT_VAL,BoolView,PC_BOOL_NONE>(home, x0, x1) {}
Parity::Parity(Space& home, bool share, Parity& p) : MixBinaryPropagator<BitView,PC_BIT_VAL,BoolView,PC_BOOL_NONE>(home, share, p) {}
ExecStatus Parity::post(Home home, BitView x0, BoolView x1) {
    (void) new (home) Parity(home, x0, x1);
    return ES_OK;
}
Propagator* Parity::copy(Space& home, bool share) {
    return new (home) Parity(home, share, *this);
}
ExecStatus Parity::propagate(Space& home, const ModEventDelta&) {
    if(x0.assigned()) {
        if(parity(x0.val()) == 1) {
            BIT_ME_CHECK(x1.one(home));
        } else {
            BIT_ME_CHECK(x1.zero(home));
        }
    } else {
        if(x1.assigned()) {
            if(__builtin_popcount(free(x0.upper(), x0.lower())) == 1) {
                int current_parity = parity(x0.lower());
                if(x1.val() == current_parity) {
                    BIT_ME_CHECK(x0.upper(home, x0.lower()));
                } else {
                    BIT_ME_CHECK(x0.lower(home, x0.upper()));
                }
            }
        }
    }
    if(x0.assigned() && x1.assigned()) {
        return home.ES_SUBSUMED(*this);
    }
    return ES_FIX;

}
void parity(Space& home, BitVar x0, BoolVar x1) {
    if(home.failed()) {
        return;
    }
    GECODE_ES_FAIL(Parity::post(home, x0, x1));
}

// weight: channel the weight of x0 to x1
Weight::Weight(Home home, BitView x0, IntView x1) : MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND>(home, x0, x1) {}
Weight::Weight(Space& home, bool share, Weight& p) : MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND>(home, share, p) {}
ExecStatus Weight::post(Home home, BitView x0, IntView x1) {
    (void) new (home) Weight(home, x0, x1);
    return ES_OK;
}
Propagator* Weight::copy(Space& home, bool share) {
    return new (home) Weight(home, share, *this);
}
ExecStatus Weight::propagate(Space& home, const ModEventDelta&) {
    LOG("w" << std::endl);
    int max = x0.max_weight();
    int min = x0.min_weight();
    LOG("pre: (" << x1.min() << "," << x1.max() << ") changing to (" << min << "," << max << ")" << std::endl);
    BIT_ME_CHECK(x1.lq(home, max));
    BIT_ME_CHECK(x1.gq(home, min));
    // if the integer var is assigned, 
    if(x1.assigned()) {
        // and the value is exactly equal to the min or the max
        // then the bit-vector should be exactly its lower or upper bound
        if(x1.val() == max) {
            BIT_ME_CHECK(x0.lower(home, x0.upper()));
        } else {
            BIT_ME_CHECK(x0.upper(home, x0.lower()));
        }
    }
    if(x0.assigned() && x1.assigned()) {
        LOG("b:" << std::bitset<NUMBITS>(x0.lower()) << " = " << x1.val() );
        LOG("s," << std::endl);
        return home.ES_SUBSUMED(*this);
    } else {
        LOG("b:" 
                << std::bitset<NUMBITS>(x0.lower()) 
                << " -- "
                << std::bitset<NUMBITS>(x0.upper()) 
                << " = " 
                << x1.min() 
                << " -- " 
                << x1.max()
                << " minweight/maxweight: (" << min << "," << max << ") "
           );
    }
    LOG("f," << std::endl);
    return ES_FIX;
}
void weight(Space& home, BitVar x0, IntVar x1) {
    if(home.failed()) {
        return;
    }
    GECODE_ES_FAIL(Weight::post(home, x0, x1));
}

// membership in an integer domain
Membership::Membership(Home home, BitView x0, IntView x1) : MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND>(home, x0, x1) {}
Membership::Membership(Space& home, bool share, Membership& p) : MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND>(home, share, p) {}
ExecStatus Membership::post(Home home, BitView x0, IntView x1) {
    (void) new (home) Membership(home, x0, x1);
    return ES_OK;
}
Propagator* Membership::copy(Space& home, bool share) {
    return new (home) Membership(home, share, *this);
}
ExecStatus Membership::propagate(Space& home, const ModEventDelta&) {
    LOG("m");
    // figure 2, Michel et al
    BIT_ME_CHECK(_membership(home, x0, to_bit_type(x1.max()), to_bit_type(x1.min())));
    if(x0.assigned() && x1.assigned()) {
        LOG(std::endl << "b:" << std::bitset<NUMBITS>(x0.lower()) << " = " << x1.val() );
        LOG("s,");
        return home.ES_SUBSUMED(*this);
    }
    LOG(",");
    return ES_FIX;
}
void membership(Space& home, BitVar x0, IntVar x1) {
    if(home.failed()) {
        return;
    }
    GECODE_ES_FAIL(Membership::post(home, x0, x1));
}
// propagating membership is used by both channeling and the membership propagators
// I don't really like this solution, but it seemed more sensible than copy-pasting the code
// although perhaps the constraing could be posted from channel()... i dunno.
ExecStatus _membership(Space& home, BitView x0, unsigned int U, unsigned int L) {
    BV::BitType ux = x0.upper();
    BV::BitType lx = x0.lower();
    unsigned int i = x0.num_bits() - 1;
    BV::BitType freebits = free(ux, lx);
    while (true) {
        if(ux < L || lx > U) {
            return ES_FAILED;
        }
        if (isfree(freebits, i)) {
            //if((ux - pow(2,i)) < L) {
            if((ux - (1U << i)) < L) {
                // set bit i of l to 1
                lx = lx bitor (1U << i);
                //} else if((lx + pow(2,i)) > U) {
        } else if((lx + (1U << i)) > U) {
            // set bit i of u to 0
            ux = ux bitand (compl (((BV::BitType) 1U) << i));
        } else {
            break;
        }
        }
        if(i == 0) {
            break;
        }
        i = i - 1;
        }
        BIT_ME_CHECK(x0.bounds(home, ux, lx));
        return ES_FIX;
    }

    // channeling to integers
    Channel::Channel(Home home, BitView x0, IntView x1) : MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND>(home, x0, x1) {}
    Channel::Channel(Space& home, bool share, Channel& p) : MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND>(home, share, p) {}
    ExecStatus Channel::post(Home home, BitView x0, IntView x1) {
        (void) new (home) Channel(home, x0, x1);
        return ES_OK;
    }
    Propagator* Channel::copy(Space& home, bool share) {
        return new (home) Channel(home, share, *this);
    }
    ExecStatus Channel::propagate(Space& home, const ModEventDelta&) {
        LOG("c");
        // figure 6. in Michel et al
        // propagate to the integer range
        BIT_ME_CHECK(x1.lq(home, to_int(x0.upper())));
        BIT_ME_CHECK(x1.gq(home, to_int(x0.lower())));
        // propagate back to the bitvector
        BIT_ME_CHECK(_membership(home, x0, to_bit_type(x1.max()), to_bit_type(x1.min())));
        // propagate to the integer once more 
        BIT_ME_CHECK(x1.lq(home, to_int(x0.upper())));
        BIT_ME_CHECK(x1.gq(home, to_int(x0.lower())));
        if(x0.assigned() && x1.assigned()) {
            LOG(std::endl << "b:" << std::bitset<NUMBITS>(x0.lower()) << " = " << x1.val() );
            LOG("s,");
            return home.ES_SUBSUMED(*this);
        }
        LOG(",");
        return ES_FIX;
    }
    void channel(Space& home, BitVar x0, IntVar x1) {
        if(home.failed()) {
            return;
        }
        GECODE_ES_FAIL(Channel::post(home, x0, x1));
    }
    void channel(Space& home, BitVarArray x0, IntVarArray x1) {
        if(home.failed()) {
            return;
        }
        if(x0.size() != x1.size()) {
            throw GeneralError("Channel: BitVarArray is not the same size as the IntVarArray");
        }
        for(int i=0; i<x0.size(); i++) {
            channel(home, x0[i], x1[i]);
        }
    }

    // conjunction: x0 and x1 = x2
    Conjunction::Conjunction(Home home, BitView x0, BitView x1, BitView x2) : TernaryPropagator<BitView,PC_BIT_BND>(home, x0, x1, x2) {}
    Conjunction::Conjunction(Space& home, bool share, Conjunction& p) : TernaryPropagator<BitView,PC_BIT_BND>(home, share, p) {}
    ExecStatus Conjunction::post(Home home, BitView x0, BitView x1, BitView x2) {
        (void) new (home) Conjunction(home, x0, x1, x2);
        return ES_OK;
    }
    Propagator* Conjunction::copy(Space& home, bool share) {
        return new (home) Conjunction(home, share, *this);
    }
    ExecStatus Conjunction::propagate(Space& home, const ModEventDelta&) {
        //  x xor  y =  z
        // x0 & x1 = x2
        BitType ux = x0.upper();
        BitType uy = x1.upper();
        BitType uz = x2.upper();
        BitType lx = x0.lower();
        BitType ly = x1.lower();
        BitType lz = x2.lower();

        BitType upx  = ux bitand (compl ((compl uz) bitand ly));
        BitType lowx = lx bitor lz;
        BIT_ME_CHECK(x0.bounds(home, upx, lowx));

        BitType upy  = uy bitand (compl ((compl uz) bitand lx));
        BitType lowy = ly bitor lz;
        BIT_ME_CHECK(x1.bounds(home, upy, lowy));

        BitType upz  = uz bitand ux bitand uy;
        BitType lowz = lz bitor (lx bitand ly);
        BIT_ME_CHECK(x2.bounds(home, upz, lowz));

        if(x0.assigned() && x1.assigned() && x2.assigned()) {
            return home.ES_SUBSUMED(*this);
        }
        return ES_FIX;
    }
    void conjunction(Space& home, BitVar x0, BitVar x1, BitVar x2) {
        if(home.failed()) {
            return;
        }
        GECODE_ES_FAIL(Conjunction::post(home, x0, x1, x2));
    }
