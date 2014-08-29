#ifndef BOOLSBOX_H
#define BOOLSBOX_H

#include <gecode/int.hh>
#include "sboxbase.h"
#include "options.h"

namespace BoolSBox {
    class BoolSBox : public SBoxBase::SBoxBase {
        public:
            unsigned int n;					// number of input bits e.g. 6
            unsigned int m;					// number of output bits e.g. 4
            Gecode::BoolVarArray x;			// the s-box contents
			Gecode::IntVarArray intvals;	// the channeled integer representation of the s-box
			int threshold;					// scores should be <= threshold

        public:
            BoolSBox(const SBoxOptions::SBoxOptions& opt);
            BoolSBox(bool share, BoolSBox& sb);
            virtual Gecode::Space* copy(bool share);
            void print(std::ostream& out) const;
    };
}
#endif
