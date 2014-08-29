#ifndef SETSBOX_H
#define SETSBOX_H

#include <gecode/set.hh>
#include "sboxbase.h"
#include "options.h"

namespace SetSBox {
    class SetSBox : public SBoxBase::SBoxBase {
        public:
            unsigned int n;					// number of input bits e.g. 6
            unsigned int m;					// number of output bits e.g. 4
            Gecode::SetVarArray x;			// the s-box contents
			Gecode::IntVarArray intvals;	// the channeled integer representation of the s-box
			int threshold;					// scores should be <= threshold

        public:
            SetSBox(const SBoxOptions::SBoxOptions& opt);
            SetSBox(bool share, SetSBox& sb);
            virtual Gecode::Space* copy(bool share);
            void print(std::ostream& out) const;
    };
}
#endif
