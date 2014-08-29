#ifndef __BITVEC_VALSEL_H_
#define __BITVEC_VALSEL_H_

#include "bitvec/bitvec.h"
#include "view.h"

using BV::BitType;
using BV::Bit::BitView;

using Gecode::ValSel;
using Gecode::Space;
using Gecode::ValBranch;

// value selection classes
namespace BV {
	namespace Bit {
		class ValSelMinBit : public ValSel<BitView, BitType> {
			public:
				ValSelMinBit(Space& home, const ValBranch& vb);
				ValSelMinBit(Space& home, bool share, ValSelMinBit& vs);
				BitType val(const Space& home, BitView x, int i);
		};
		class ValSelMaxBit : public ValSel<BitView, BitType> {
			public:
				ValSelMaxBit(Space& home, const ValBranch& vb);
				ValSelMaxBit(Space& home, bool share, ValSelMaxBit& vs);
				BitType val(const Space& home, BitView x, int i);
		};
		class ValSelRndBit : public ValSel<BitView, BitType> {
			protected:
				Rnd r;
			public:
				ValSelRndBit(Space& home, const ValBranch& vb);
				ValSelRndBit(Space& home, bool share, ValSelRndBit& vs);
				BitType val(const Space& home, BitView x, int i);
				bool notice(void) const;
				void dispose(Space& home);
		};
	}
}
#endif
