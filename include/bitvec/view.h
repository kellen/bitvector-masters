#ifndef __BITVEC_BITVIEW_H_
#define __BITVEC_BITVIEW_H_

#include "bitvec/bitvec.h"
#include "var_imp.h"
#include "var.h"

using BV::BitVar;
using BV::Bit::BitVarImp;
using Gecode::Delta;
using Gecode::VarImpView;
using Gecode::Space;

namespace BV {
    namespace Bit {
        class BitView : public VarImpView<BitVar> {
            protected:
                using VarImpView<BitVar>::x;
            public:
                BitView(void);
                BitView(const BitVar& y);
                BitView(BitVarImp* y);
                BV::BitType mask(void) const;
                unsigned int num_bits(void) const;
                BV::BitType lower(void) const;
                BV::BitType upper(void) const;
                BV::BitType val(void) const;
                BV::BitType lower(const Delta& d) const;
                BV::BitType upper(const Delta& d) const;
                ModEvent lower(Space& home, BitType val);
                ModEvent upper(Space& home, BitType val);
                ModEvent bounds(Space& home, BitType upper_val, BitType lower_val);
                int max_weight();
                int min_weight();
        };

        template<class Char, class Traits>
            std::basic_ostream<Char,Traits>&
            operator <<(std::basic_ostream<Char,Traits>& os, const BitView& x) {
                std::basic_ostringstream<Char,Traits> s;
                s.copyfmt(os);
                s.width(0);
                if (x.assigned()) {
                    s << std::bitset<NUMBITS>(x.lower());
                } else {
                    s << '[' << std::bitset<NUMBITS>(x.lower()) << ".." << std::bitset<NUMBITS>(x.upper()) << ']';
                }
                return os << s.str();
            }
    }
}
#endif
