#ifndef __BITVEC_CONSTBITVIEW_H_
#define __BITVEC_CONSTBITVIEW_H_

#include "bitvec/bitvec.h"
#include "view.h"

using BV::Bit::BitView;

using Gecode::ConstView;
using Gecode::Delta;
using Gecode::Space;

namespace BV {
    namespace Bit {
        class ConstBitView : public ConstView<BitView> {
            protected:
                BV::BitType x;
            public:
                ConstBitView(void);
                ConstBitView(BV::BitType n);
                BV::BitType lower(void) const;
                BV::BitType upper(void) const;
                BV::BitType lower(const Delta& d) const;
                BV::BitType upper(const Delta& d) const;
                void update(Space& home, bool share, ConstBitView& y);
        };
        bool same(const ConstBitView& x, const ConstBitView& y);
        bool before(const ConstBitView& x, const ConstBitView& y);
        template<class Char, class Traits>
            std::basic_ostream<Char,Traits>&
            operator <<(std::basic_ostream<Char,Traits>& os, const ConstBitView& x);
    }
}
#endif
