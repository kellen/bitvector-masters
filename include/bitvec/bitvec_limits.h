#ifndef __BITVEC_LIMITS_H_
#define __BITVEC_LIMITS_H_

namespace BV {
    namespace Bit {
        // limits
        namespace Limits {
            // Unclear if these are necessary; it's unlikely that the machine
            // can even represent numbers greater than BV::BitType::Max()
            const BV::BitType upper = std::numeric_limits<BV::BitType>::max();
            const BV::BitType lower = std::numeric_limits<BV::BitType>::min();
        };
    }
}
#endif
