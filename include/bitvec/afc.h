#ifndef __BITVEC_AFC_H_
#define __BITVEC_AFC_H_

#include "bitvec/bitvec.h"
#include "bitvec/array.h"

using BV::BitVarArgs;

using Gecode::AFC;
using Gecode::Home;

namespace BV {
    class BitAFC : public AFC {
        public:
            BitAFC(void);
            BitAFC(const BitAFC& a);
            BitAFC& operator =(const BitAFC& a);
            BitAFC(Home home, const BitVarArgs& x, double d=1.0);
            void init(Home home, const BitVarArgs& x, double d=1.0);
    };
}
#endif
