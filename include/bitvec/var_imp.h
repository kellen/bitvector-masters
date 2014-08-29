#ifndef __BITVEC_VARIMP_H__
#define __BITVEC_VARIMP_H__

#include "bitvec/bitvec.h"
#include "bitvec_limits.h"

using Gecode::Delta;
using Gecode::Propagator;
using Gecode::PropCond;
using Gecode::Advisor;
using Gecode::Space;

namespace BV {
    namespace Bit {
        class BitVarImp : public Gecode::Bit::BitVarImpBase {
            protected:
                BV::BitType u, l, m;
                unsigned int n;
            public:
                BitVarImp(Space& home, unsigned int num_bits);
                BitVarImp(Space& home, unsigned int num_bits, BV::BitType lower, BV::BitType upper);
                // assignment
                bool assigned(void) const;
                // access
                BV::BitType lower(void) const;
                BV::BitType upper(void) const;
                BV::BitType val(void) const;
                unsigned int num_bits(void) const;
                BV::BitType mask(void) const;
                // delta info
                static BV::BitType lower(const Delta& d);
                static BV::BitType upper(const Delta& d);
                // subscriptions
                void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule=true);
                void subscribe(Space & home, Advisor& a);
                void cancel(Space& home, Propagator& p, PropCond pc);
                void cancel(Space& home, Advisor& a);
                // copying
                BitVarImp(Space& home, bool share, BitVarImp& y);
                BitVarImp* copy(Space& home, bool share);
                // modification events
                ModEvent lower(Space& home, BitType val);
                ModEvent upper(Space& home, BitType val);
                ModEvent bounds(Space& home, BitType upper_val, BitType lower_val);
                // >= and <= for channeling
                //ModEvent gq(Space& home, int val);
                //ModEvent lq(Space& home, int val);
                // hamming weight
                int max_weight();
                int min_weight();

        };
    }
}
#endif
