#ifndef __BIT_VAR_H__
#define __BIT_VAR_H__

#include "bitvec/bitvec.h"
#include "var_imp.h"

using Gecode::VarImpVar;
using Gecode::Space;

namespace BV {
    class BitVar : public VarImpVar<BV::Bit::BitVarImp> {
        protected:
            using VarImpVar<BV::Bit::BitVarImp>::x;
        public:
            BitVar(void);
            BitVar(const BitVar& y);
            BitVar(BV::Bit::BitVarImp* y);
            BitVar(Space& home, unsigned int n);
            BitVar(Space& home, unsigned int n, BV::BitType lower, BV::BitType upper);
            BV::BitType lower(void) const;
            BV::BitType upper(void) const;
            BV::BitType val(void) const;
    };

    template<class Char, class Traits>
        std::basic_ostream<Char,Traits>&
        operator <<(std::basic_ostream<Char,Traits>& os, const BV::BitVar& x) {
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
#endif
