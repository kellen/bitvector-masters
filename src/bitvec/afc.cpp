#include "bitvec/afc.h"

using BV::BitVarArgs;

using Gecode::AFC;
using Gecode::Home;

BV::BitAFC::BitAFC(void) {}
BV::BitAFC::BitAFC(const BitAFC& a) : AFC(a) {}
BV::BitAFC& BV::BitAFC::operator =(const BitAFC& a) {
    return static_cast<BV::BitAFC&>(AFC::operator =(a));
}
BV::BitAFC::BitAFC(Home home, const BitVarArgs& x, double d) {
    AFC::init(home, x, d);
}
void BV::BitAFC::init(Home home, const BitVarArgs& x, double d) {
    AFC::init(home, x, d);
}
