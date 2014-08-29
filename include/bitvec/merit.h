#ifndef __BITVEC_MERIT_H_
#define __BITVEC_MERIT_H_

#include "bitvec/bitvec.h"
#include "view.h"

using BV::Bit::BitView;

using Gecode::Space;
using Gecode::VarBranch;
using Gecode::MeritBase;

namespace BV {
    namespace Bit {
        class MeritSize : public MeritBase<BitView, unsigned int> {
            public:
                MeritSize(Space& home, const VarBranch& vb);
                MeritSize(Space& home, bool share, MeritSize& m);
                unsigned int operator ()(const Space& home, BitView x, int i);
        };
    }
}
#endif
