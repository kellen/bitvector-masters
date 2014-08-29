#include "bitvec/var_imp.h"

#include "bitvec/delta.h"

#include "bitvec/exception.h"
using BV::Bit::BitDelta;
using BV::Bit::OutOfLimits;
using BV::Bit::InvalidBounds;

using Gecode::Bit::ME_BIT_FAILED;
using Gecode::Bit::ME_BIT_NONE;
using Gecode::Bit::ME_BIT_VAL;
using Gecode::Bit::ME_BIT_LOWER;
using Gecode::Bit::ME_BIT_UPPER;
using Gecode::Bit::ME_BIT_BND;

// variable implementation
BV::Bit::BitVarImp::BitVarImp(Space& home, unsigned int num_bits) : BitVarImpBase(home), n(num_bits) {
    if(n > NUMBITS) {
        throw OutOfLimits("BitVarImp");
    }
    m = bit_mask(num_bits);
    l = (BV::BitType) 0;
    u = (BV::BitType) m;
}
BV::Bit::BitVarImp::BitVarImp(Space& home, unsigned int num_bits, BV::BitType lower, BV::BitType upper) : BitVarImpBase(home), n(num_bits), l(lower), u(upper) {
    // ensure n is within the representable values
    if(n > NUMBITS) {
        throw OutOfLimits("BitVarImp");
    }
    // these are probably impossible to actually trigger since we're using the full representable
    // range and any constant larger than this should trigger a compilation warning
    if((lower < Bit::Limits::lower) || (upper > Bit::Limits::upper)) {
        throw OutOfLimits("BitVar");
    }
    if(!valid(u,l)) {
        throw InvalidBounds("BitVar");
    }
    // this may also be unnecessary due to calling valid() above
    if(lower > upper) {
        throw VariableEmptyDomain("BitVar");
    }
    // make mask
    m = bit_mask(n);
    // check that lower and upper have none of these bits set
    if((l bitand (compl m)) != 0 || (u bitand (compl m)) != 0) {
        throw InvalidBounds("BitVarImp");
    }
}

// assignment test
bool BV::Bit::BitVarImp::assigned(void) const {
    return l == u;
}
// access operations
unsigned int BV::Bit::BitVarImp::num_bits(void) const {
    return n;
}
BV::BitType BV::Bit::BitVarImp::mask(void) const {
    return m;
}
BV::BitType BV::Bit::BitVarImp::lower(void) const {
    return l;
}
BV::BitType BV::Bit::BitVarImp::upper(void) const {
    return u;
}
BV::BitType BV::Bit::BitVarImp::val(void) const {
    if(assigned()) return u;
    throw GeneralError("BitVarImp: variable not assigned");
}
// hamming weight 
int BV::Bit::BitVarImp::max_weight() {
    // the number of currently assigned bits == 1
    int cur_weight = (std::bitset<NUMBITS>(fixed(u, l) & u)).count();
    // the number of unassigned/free bits
    int free_weight = (std::bitset<NUMBITS>(free(u, l))).count();
    // max weight is the weight of the currently assigned bits + all free bits
    return cur_weight + free_weight;
}
int BV::Bit::BitVarImp::min_weight() {
    // min weight is the weight of the currently assigned bits, all others == 0
    return (std::bitset<NUMBITS>(fixed(u, l) & u)).count();
}
// modification operations
// assume these should be called for only one variable at a time and that they take constants
// sets the lower bound, failing if it tries to change fixed bits
ModEvent BV::Bit::BitVarImp::lower(Space& home, BitType val) {
    // nothing would change
    if(val == l) {
        return ME_BIT_NONE;
    }
   // fail if the bound is outside the allowable set bits
    if(!fits_mask(val, m)) {
        return ME_BIT_FAILED;
    }
    // fail if this would not be a valid bound
    if(!valid(u, val)) {
        return ME_BIT_FAILED;
    }
    // fail if the new lower bound would unset some fixed bits
    BitType oldfixed = fixed(u, l);
    BitType newfixed = fixed(u, val);
    if(oldfixed != (oldfixed bitand newfixed)) {
        return ME_BIT_FAILED;
    }
    // TODO delta: BitDelta should probably be updated with interesting values...
    l = val;
    //BitDelta d();
    Delta d;
    return notify(home, assigned() ? ME_BIT_VAL : ME_BIT_LOWER, d);
}
ModEvent BV::Bit::BitVarImp::upper(Space& home, BitType val) {
    // nothing changed
    if(val == u) {
        return ME_BIT_NONE;
    }
    // fail if the bound is outside the allowable set bits
    if(!fits_mask(val, m)) {
        return ME_BIT_FAILED;
    }
    // fail if this would not be a valid bound
    if(!valid(val, l)) {
        return ME_BIT_FAILED;
    }
    // fail if the new upper bound would unset already-fixed bits
    BitType oldfixed = fixed(u,l);
    BitType newfixed = fixed(val, l);
    if(oldfixed != (oldfixed & newfixed)) {
        return ME_BIT_FAILED;
    }
    u = val;
    // TODO delta: BitDelta should probably be updated with interesting values...
    //BitDelta d();
    Delta d;
    return notify(home, assigned() ? ME_BIT_VAL : ME_BIT_UPPER, d);
}

ModEvent BV::Bit::BitVarImp::bounds(Space& home, BitType upper_val, BitType lower_val) {
    ModEvent ue = upper(home, upper_val);
    ModEvent le = lower(home, lower_val);
    if(ue == ME_BIT_FAILED || le == ME_BIT_FAILED) {
        return ME_BIT_FAILED;
    }
    if(ue == ME_BIT_VAL || le == ME_BIT_VAL) {
        return ME_BIT_VAL;
    }
    if(ue == ME_BIT_UPPER) {
        if(le == ME_BIT_LOWER) {
            return ME_BIT_BND;
        }
        return ue;
    } 
    return le;
}

// TODO delta: delta information
BV::BitType BV::Bit::BitVarImp::lower(const Delta& d) {
    //return static_cast<const BitDelta&>(d).lower();
    GECODE_NEVER; 
    return 0;
}
BV::BitType BV::Bit::BitVarImp::upper(const Delta& d) {
    //return static_cast<const BitDelta&>(d).upper();
    GECODE_NEVER; 
    return 0;
}
// subscriptions
void BV::Bit::BitVarImp::subscribe(Space& home, Propagator& p, PropCond pc, bool schedule) {
    BitVarImpBase::subscribe(home, p, pc, assigned(), schedule);
}
void BV::Bit::BitVarImp::subscribe(Space & home, Advisor& a) {
    BitVarImpBase::subscribe(home, a, assigned());
}
void BV::Bit::BitVarImp::cancel(Space& home, Propagator& p, PropCond pc) {
    BitVarImpBase::cancel(home, p, pc, assigned());
}
void BV::Bit::BitVarImp::cancel(Space& home, Advisor& a) {
    BitVarImpBase::cancel(home, a, assigned());
}
// copying
BV::Bit::BitVarImp::BitVarImp(Space& home, bool share, BV::Bit::BitVarImp& y) 
    : BitVarImpBase(home, share, y), l(y.l), u(y.u), n(y.n), m(y.m) {}

BV::Bit::BitVarImp* BV::Bit::BitVarImp::copy(Space& home, bool share) {
    if(copied()) {
        return static_cast<BV::Bit::BitVarImp*>(forward());
    } else {
        return new (home) BV::Bit::BitVarImp(home, share, *this);
    }
}
