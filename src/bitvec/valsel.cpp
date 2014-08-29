#include "bitvec/valsel.h"
#include "bitvec/exception.h"

#define DEBUG false
#define LOG(x) if(DEBUG){std::cerr << x;}

using BV::Bit::GeneralError;

// min
BV::Bit::ValSelMinBit::ValSelMinBit(Space& home, const ValBranch& vb) : ValSel<BitView, BV::BitType>(home, vb) {}
BV::Bit::ValSelMinBit::ValSelMinBit(Space& home, bool share, ValSelMinBit& vs) : ValSel<BitView, BV::BitType>(home, share, vs) {}
BV::BitType BV::Bit::ValSelMinBit::val(const Space& home, BitView x, int i) {
    BitType b = ffs(free(x.upper(), x.lower()));
    if(b == 0) {
        throw GeneralError("no min bit chosen");
    }
    return b;
}

// max
BV::Bit::ValSelMaxBit::ValSelMaxBit(Space& home, const ValBranch& vb) : ValSel<BitView, BV::BitType>(home, vb) {}
BV::Bit::ValSelMaxBit::ValSelMaxBit(Space& home, bool share, ValSelMaxBit& vs) : ValSel<BitView, BV::BitType>(home, share, vs) {}
BV::BitType BV::Bit::ValSelMaxBit::val(const Space& home, BitView x, int i) {
    BitType b = fls(free(x.upper(), x.lower()));
    if(b == 0) {
        throw GeneralError("no max bit chosen");
    }
    return b;
}

// rnd
BV::Bit::ValSelRndBit::ValSelRndBit(Space& home, const ValBranch& vb) : ValSel<BitView, BV::BitType>(home, vb), r(vb.rnd()) {}
BV::Bit::ValSelRndBit::ValSelRndBit(Space& home, bool share, ValSelRndBit& vs) : ValSel<BitView, BV::BitType>(home, share, vs) {
    r.update(home, share, vs.r);
}
BV::BitType BV::Bit::ValSelRndBit::val(const Space& home, BitView x, int i) {
    BitType f = free(x.upper(), x.lower());
    LOG("free:" << std::bitset<NUMBITS>(f) << ",");
    if(f == 0) {
        throw GeneralError("no bits to choose: this var is assigned.");
    }
    unsigned int num = (unsigned int)(std::bitset<NUMBITS>(f)).count();
    LOG("num:" << num << ",");
    BitType b;
    if(num == 1) {
        b = f;
    } else {
        unsigned int randnum = r(num-1)+1;
        LOG("bitnum:" << randnum << ",");
        b = fnths(f, randnum);
    }
    LOG("chosen:" << std::bitset<NUMBITS>(b) << std::endl);
    if(b == 0) {
        throw GeneralError("no rnd bit chosen");
    }
    return b;
}
bool BV::Bit::ValSelRndBit::notice(void) const {
    return true;
}
void BV::Bit::ValSelRndBit::dispose(Space& home) {
    r.~Rnd();
}
