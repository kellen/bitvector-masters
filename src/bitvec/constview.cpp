#include "bitvec/constview.h"

// constant view
BV::Bit::ConstBitView::ConstBitView(void) : x(0) {}
BV::Bit::ConstBitView::ConstBitView(BV::BitType n) : x(n) {}
// access
BV::BitType BV::Bit::ConstBitView::lower(void) const {
    return x;
}
BV::BitType BV::Bit::ConstBitView::upper(void) const {
    return x;
}
// modification 
// TODO constview: implement modevents
// ModEvent lq(Space& home, int n) {
//      return (x <= n) ? ME_INT_NONE : ME_INT_FAILED;
// }
// delta
BV::BitType BV::Bit::ConstBitView::lower(const Delta& d) const {
    GECODE_NEVER;
    return 0;
}
BV::BitType BV::Bit::ConstBitView::upper(const Delta& d) const {
    GECODE_NEVER;
    return 0;
}
// update
void BV::Bit::ConstBitView::update(Space& home, bool share, ConstBitView& y) {
    ConstView<BV::Bit::BitView>::update(home, share, y);
    x = y.x;
}
// view tests
bool BV::Bit::same(const ConstBitView& x, const ConstBitView& y) {
    return x.lower() == y.lower();
}
bool BV::Bit::before(const ConstBitView& x, const ConstBitView& y) {
    return x.lower() < y.lower();
}
template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
BV::Bit::operator <<(std::basic_ostream<Char,Traits>& os, const ConstBitView& x) {
    return os << x.lower();
}
