#include "bitvec/view.h"

using BV::BitType;

BV::Bit::BitView::BitView(void) {}
BV::Bit::BitView::BitView(const BitVar& y) : VarImpView<BitVar>(y.varimp()) {}
BV::Bit::BitView::BitView(BitVarImp* y) : VarImpView<BitVar>(y) {}
// access
BV::BitType BV::Bit::BitView::mask(void) const {
    return x->mask();
}
unsigned int BV::Bit::BitView::num_bits(void) const {
    return x->num_bits();
}
BV::BitType BV::Bit::BitView::lower(void) const {
    return x->lower();
}
BV::BitType BV::Bit::BitView::upper(void) const {
    return x->upper();
}
BV::BitType BV::Bit::BitView::val(void) const {
    return x->val();
}
// modification events
ModEvent BV::Bit::BitView::lower(Space& home, BitType val) {
    return x->lower(home, val);
}
ModEvent BV::Bit::BitView::upper(Space& home, BitType val) {
    return x->upper(home, val);
}
ModEvent BV::Bit::BitView::bounds(Space& home, BitType upper_val, BitType lower_val) {
    return x->bounds(home, upper_val, lower_val);
}
// delta
BV::BitType BV::Bit::BitView::lower(const Delta& d) const {
    return BitVarImp::lower(d);
}
BV::BitType BV::Bit::BitView::upper(const Delta& d) const {
    return BitVarImp::upper(d);
}
// weight
int BV::Bit::BitView::max_weight() {
    return x->max_weight();
}
int BV::Bit::BitView::min_weight() {
    return x->min_weight();
}
