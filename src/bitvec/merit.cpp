#include "bitvec/merit.h"

BV::Bit::MeritSize::MeritSize(Space& home, const VarBranch& vb) : MeritBase<BitView, unsigned int>(home, vb) {}
BV::Bit::MeritSize::MeritSize(Space& home, bool share, MeritSize& m) : MeritBase<BitView, unsigned int>(home, share, m) {}
unsigned int BV::Bit::MeritSize::operator ()(const Space& home, BitView x, int i) {
    // use the number of free bits as merit
    return std::bitset<NUMBITS>(free(x.upper(), x.lower())).count();
}
