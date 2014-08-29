#ifndef SBOX_H
#define SBOX_H

#include <gecode/int.hh>
#include "sboxbase.h"
#include "bitvec/bitvec.h"
#include "bitvec/array.h"
#include "options.h"

namespace SBox {
    class SBox : public SBoxBase::SBoxBase {
        public:
            unsigned int n;					// number of input bits e.g. 6
            unsigned int m;					// number of output bits e.g. 4
            BV::BitVarArray x;              // the s-box contents
            Gecode::IntVarArray intvals;	// the channeled integer representations of the s-box
            int threshold;					// scores should be <= threshold

        public:
            SBox(const SBoxOptions::SBoxOptions& opt);
            SBox(bool share, SBox& sb);
            virtual Gecode::Space* copy(bool share);
            void print(std::ostream& out) const;
    };
}
#endif
