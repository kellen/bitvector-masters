#ifndef __BITVEC_ARRAY_H_
#define __BITVEC_ARRAY_H_

#include "var.h"

using BV::BitVar;

// array traits
namespace BV {
    class BitVarArgs;
    class BitVarArray;
}

namespace Gecode {
    template<> class ArrayTraits<Gecode::VarArray<BV::BitVar> > {
        public:
            typedef BV::BitVarArray  StorageType;
            typedef BV::BitVar       ValueType;
            typedef BV::BitVarArgs   ArgsType;
    };
    template<> class ArrayTraits<BV::BitVarArray> {
        public:
            typedef BV::BitVarArray  StorageType;
            typedef BV::BitVar       ValueType;
            typedef BV::BitVarArgs   ArgsType;
    };
    template<> class ArrayTraits<Gecode::VarArgArray<BV::BitVar> > {
        public:
            typedef BV::BitVarArgs   StorageType;
            typedef BV::BitVar       ValueType;
            typedef BV::BitVarArgs   ArgsType;
    };
    template<> class ArrayTraits<BV::BitVarArgs> {
        public:
            typedef BV::BitVarArgs   StorageType;
            typedef BV::BitVar       ValueType;
            typedef BV::BitVarArgs   ArgsType;
    };
}

// variable arrays
namespace BV {
    class BitVarArgs : public VarArgArray<BitVar> {
        public:
            BitVarArgs(void) {}
            BitVarArgs(const BitVarArgs& a) : VarArgArray<BitVar>(a) {}
            BitVarArgs(const VarArray<BitVar>& a) : VarArgArray<BitVar>(a) {}
            BitVarArgs(Space& home, int n, unsigned int num_bits) : VarArgArray<BitVar>(n) {
                for(int i=0; i<n; i++) {
                    (*this)[i] = BitVar(home, num_bits);
                }
            }
            BitVarArgs(Space& home, int n, unsigned int num_bits, BV::BitType lower, BV::BitType upper) : VarArgArray<BitVar>(n) {
                for(int i=0; i<n; i++) {
                    (*this)[i] = BitVar(home, num_bits, lower, upper);
                }
            }
    };
    class BitVarArray : public VarArray<BitVar> {
        public:
            BitVarArray(void) {}
            BitVarArray(const BitVarArray& a) : VarArray<BitVar>(a) {}
            BitVarArray(Space& home, int n) : VarArray<BitVar>(home, n) {}
            BitVarArray(Space& home, int n, unsigned int num_bits) : VarArray<BitVar>(home, n) {
                for(int i=0; i<n; i++) {
                    (*this)[i] = BitVar(home, num_bits);
                }
            }
            BitVarArray(Space& home, int n, unsigned int num_bits, BV::BitType upper, BV::BitType lower) : VarArray<BitVar>(home, n) {
                for(int i=0; i<n; i++) {
                    (*this)[i] = BitVar(home, num_bits, lower, upper);
                }
            }
    };
}
#endif
