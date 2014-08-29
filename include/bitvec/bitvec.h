#ifndef __BITVEC_BIT_H__
#define __BITVEC_BIT_H__

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <bitset>
#include <stdexcept>

#include <gecode/kernel.hh>

using Gecode::Advisor;
using Gecode::ConstView;
using Gecode::Delta;
using Gecode::DerivedView;
using Gecode::Exception;
using Gecode::ModEvent;
using Gecode::ModEventDelta;
using Gecode::PropCond;
using Gecode::Propagator;
using Gecode::Space;
using Gecode::VarArgArray;
using Gecode::VarArray;
using Gecode::VarImpVar;
using Gecode::VarImpView;
using Gecode::AFC;
using Gecode::Activity;
using Gecode::Home;
using Gecode::ViewArray;
using Gecode::Rnd;
using Gecode::VarBranch;
using Gecode::ValBranch;
using Gecode::BranchTbl;
using Gecode::VoidFunction;
using Gecode::MeritBase;
using Gecode::ViewSel;
using Gecode::ViewSelMaxTbl;
using Gecode::ViewSelMinTbl;
using Gecode::ViewSelMax;
using Gecode::ViewSelMin;
using Gecode::ViewSelNone;
using Gecode::ViewSelRnd;
using Gecode::MeritFunction;
using Gecode::MeritDegree;
using Gecode::MeritActivity;
using Gecode::ValSel;
using Gecode::ValCommit;
using Gecode::NGL;
using Gecode::ExecStatus;
using Gecode::ES_FAILED;
using Gecode::ES_OK;
using Gecode::me_failed;
using Gecode::ViewValNGL;
using Gecode::ValSelCommit;
using Gecode::ValSelCommitBase;
using Gecode::ValSelFunction;
using Gecode::ValCommitFunction;
using Gecode::ViewValBrancher;
using Gecode::TieBreak;
using Gecode::function_cast;
using Gecode::BrancherHandle;

#define DEBUGGING_CHECKS false

// OBS! this must be an unsigned type
namespace BV {
    // according to http://www.makelinux.net/books/lkd2/ch19lev1sec2
    // "The size of the C long type is guaranteed to be the machine's word size"
    // typedef unsigned long BitType;
    // TODO Gecode complains about some undefined function in the brancher when using long.
    // Since we don't need that many bits for our purposes, just use an unsigned int for now.
    typedef unsigned int BitType;
}

static inline int to_int(BV::BitType x) {
    if(x > INT_MAX) throw std::out_of_range ("val greater than INT_MAX");
    return static_cast<int>(x);
}
static inline BV::BitType to_bit_type(int x) {
    // BitType is >= int always, but we need to check the lower bound
    // TODO debatable if this should get thrown or silently converted to 0
    if(x < 0) throw std::out_of_range ("val less than 0");
    return static_cast<BV::BitType>(x);
}

/**
 * Produces a bitmask with the x least significant bits set
 * http://stackoverflow.com/questions/8774567/c-macro-to-create-a-bit-mask-possible-and-have-i-found-a-gcc-bug
 */
static inline BV::BitType bit_mask(unsigned int x) {
    return (x >= sizeof(BV::BitType) * CHAR_BIT) ? (BV::BitType) -1 : (1U << x) - 1;
}

// determines if a set of vectors is valid
#define NUMBITS (CHAR_BIT * sizeof(BV::BitType))

// bit operations
static inline bool valid(BV::BitType u, BV::BitType l) {
    return (compl ((compl (l xor u)) bitor u)) == ((BV::BitType)0U);
}
/*
   110110
   010010
   100100 = xor = free
 */
static inline BV::BitType free(BV::BitType u, BV::BitType l) {
    return (u xor l);
}
/* fixed = ~free */
static inline BV::BitType fixed(BV::BitType u, BV::BitType l) {
    return (compl free(u,l));
}
/* in bounds of mask */
static inline BV::BitType fits_mask(BV::BitType val, BV::BitType mask) {
    return ((val bitand (compl mask)) == 0);
}
/* is bit i a free bit? */
static inline bool isfree(BV::BitType freebits, unsigned int i) {
    return (freebits bitand (((BV::BitType) 1U) << i)) > 0;
}

// from http://rosettacode.org/wiki/Find_first_and_last_set_bit_of_a_long_integer#C
// TODO ffs and fls are not particularly performant. they could be replaced by some fast library functions.
/* find the first set bit; return a BitType with this bit set */
static inline BV::BitType ffs(BV::BitType x) {
    return x ?  1U << (__builtin_ffs(x) - 1) : x;
    /*
    if(x == 0) {
        return 0;
    }
    BV::BitType t = 1U;
    while((x bitand t) == 0) {
        t = t << 1;
    }
    return t;
    */

}
/* find the last set bit; return a BitType with this bit set */
static inline BV::BitType fls(BV::BitType x) {
    int ret = sizeof(BV::BitType) * CHAR_BIT - 1;
    return x ? (1U << (ret - __builtin_clz(x))) : x;
    /*
    if(x == 0) {
        return 0;
    }
    BV::BitType t = 1U << (NUMBITS - 1);
    while((x bitand t) == 0) {
        t = t >> 1;
    }
    return t;
    */
}
/* find the nth bit set; return a BitType with this bit set */
static inline BV::BitType fnths(BV::BitType x, unsigned int n) {
    // TODO potentially use
    // http://stackoverflow.com/questions/2001597/how-do-you-randomly-zero-a-bit-in-an-integer
    // FIXME what if n > len(unsigned int) ?
    bool DEBUG = false;
    bool FAILLOG = true;
    if(x == 0) {
        if(FAILLOG) {
            std::cerr << "x is 0!?; " << "x:" << std::bitset<NUMBITS>(x) << ", n:" << n << std::endl;
        }
        return 0;
    }
    if(DEBUG) {
        std::cerr << std::endl;
    }
    BV::BitType t = 1U;
    unsigned int r = 0;
    while(r < n && t != 0) {
        if(DEBUG) {
            std::cerr << "x:" << std::bitset<NUMBITS>(x) << std::endl;
            std::cerr << "t:" << std::bitset<NUMBITS>(t) << ",r:" << r << ",n:" << n << std::endl;
            std::cerr << "&:" << std::bitset<NUMBITS>(x bitand t) << std::endl;
            std::cerr << "!=0:" << (x bitand t != 0 ? "true" : "false") << std::endl;
            std::cerr << "()!=0:" << ((x bitand t) != 0 ? "true" : "false") << std::endl << std::endl << std::endl;
        }
        if((x bitand t) != 0) {
            r = r + 1;
            if(r == n) {
                break;
            }
        }
        t = t << 1;
    }
    if(DEBUG) {
        if(t == 0) {
            std::cerr << "t is 0?? wtf?" << std::endl;
        }
        std::cerr << "x:" << std::bitset<NUMBITS>(x) << std::endl;
        std::cerr << "t:" << std::bitset<NUMBITS>(t) << ",r:" << r << ",n:" << n << std::endl;
    }
    if(FAILLOG) {
        if((t bitand x) == 0) {
            std::cerr << "a non-fixed bit selected: " << std::endl;
            std::cerr << "x:" << std::bitset<NUMBITS>(x) << std::endl;
            std::cerr << "t:" << std::bitset<NUMBITS>(t) << ",r:" << r << ",n:" << n << std::endl;
        }
    }
    return t;
}
#endif
