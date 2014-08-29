#ifndef __BITVEC_CONSISTENCY_H__
#define __BITVEC_CONSISTENCY_H__

/**
 * Consistency levels
 */
namespace Gecode {
    enum BitConLevel {
        BCL_DOM, // domain: "standard definition lifted to bit-vector variables"
        BCL_BIT  // bit
    };
}
#endif
