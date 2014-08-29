#ifndef __BITVEC_DELTA_H__
#define __BITVEC_DELTA_H__

#include "bitvec/bitvec.h"
using Gecode::Delta;

namespace BV{
    namespace Bit {
        class BitDelta : public Delta {
            // TODO deltas: implement!
            /*
            private:
                BV::BitType l, u;
                */
            public:
                BitDelta(void);
                /*
                BitDelta(BV::BitType lower, BV::BitType upper);
                BV::BitType lower(void) const;
                BV::BitType upper(void) const;
                */
        };
    }
}
#endif
