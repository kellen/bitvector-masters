#ifndef __BITVEC_BRANCHER_H_
#define __BITVEC_BRANCHER_H_

#include "bitvec/bitvec.h"
#include "var.h"
#include "bitvec/array.h"
#include "view.h"
#include "merit.h"
#include "exception.h"
#include "valsel.h"

using BV::BitVar;
using BV::BitType;
using BV::BitVarArgs;
using BV::Bit::BitView;
using BV::Bit::MeritSize;
using BV::Bit::UnknownBranching;

using Gecode::ViewArray;
using Gecode::Space;
using Gecode::BrancherHandle;
using Gecode::ViewSelNone;
using Gecode::ViewSelMaxTbl;
using Gecode::ViewSelMinTbl;
using Gecode::ViewSelRnd;
using Gecode::ValSelFunction;
using Gecode::ValCommitFunction;
using Gecode::Activity;
using Gecode::BranchTbl;
using Gecode::Home;
using Gecode::Rnd;
using Gecode::VarBranch;
using Gecode::VoidFunction;

namespace BV {
    class BitVarBranch;
    class BitValBranch;
    class BitActivity;
}

// branch function types
namespace BV {
    typedef bool    (*BitBranchFilter) (const Space& home, BitVar x, int i);
    typedef double  (*BitBranchMerit)  (const Space& home, BitVar x, int i);
    typedef void    (*BitBranchCommit) (Space& home, unsigned int a, BitVar x, int i, BitType n);
    typedef BitType (*BitBranchVal)    (const Space& home, BitVar x, int i);
    typedef void    (*BitVarValPrint)  (const Space& home, const BrancherHandle& bh, unsigned int a, BitVar x, int i, const BitType& n, std::ostream& o);
}

// branch traits
namespace Gecode {
    template<> class BranchTraits<BitVar> {
        public:
            typedef BV::BitBranchFilter Filter;
            typedef BV::BitBranchMerit Merit;
            typedef BV::BitBranchVal Val;
            // In MPG, ValType is: typedef int ValType; assuming BitType instead.
            typedef BitType ValType;
            typedef BV::BitBranchCommit Commit;
    };
}

namespace BV {
    BrancherHandle branch(Home home, const BitVarArgs& x, BitVarBranch vars,
            BitValBranch vals, BitBranchFilter bf=NULL, BitVarValPrint vvp=NULL);
    BrancherHandle branch(Home home, const BitVarArgs& x, TieBreak<BitVarBranch> vars,
            BitValBranch vals, BitBranchFilter bf=NULL, BitVarValPrint vvp=NULL);
    // TODO brancher: write a variant for single variables... this doesn't seem to be built-in to gecode
}

namespace BV {
    namespace Bit {
        // view selection creation function
        ViewSel<BitView>* viewsel(Space& home, const BitVarBranch& bvb);
        // value selection commit
        ValSelCommitBase<BitView, BitType>* valselcommit(Space& home, const BitValBranch& bvb);
    }
}

namespace BV {
    class BitVarBranch : public VarBranch {
        public:
            enum Select {
                SEL_NONE, SEL_RND, SEL_MERIT_MAX, SEL_DEGREE_MAX, SEL_ACTIVITY_MAX, SEL_SIZE_MIN
            };
        protected:
            Select s;
        public:
            BitVarBranch(void);
            BitVarBranch(Rnd r);
            BitVarBranch(Select s0, BranchTbl t);
            BitVarBranch(Select s0, double d, BranchTbl t);
            BitVarBranch(Select s0, Activity a, BranchTbl t);
            BitVarBranch(Select s0, VoidFunction mf, BranchTbl t);
            Select select(void) const;
            void expand(Home home, const BitVarArgs& x);
    };
    // variable selection functions
    BitVarBranch BIT_VAR_NONE(void);
    BitVarBranch BIT_VAR_RND(Rnd r);
    BitVarBranch BIT_VAR_MERIT_MAX(BitBranchMerit bm, BranchTbl tbl=NULL);
    BitVarBranch BIT_VAR_DEGREE_MAX(BranchTbl tbl=NULL);
    BitVarBranch BIT_VAR_ACTIVITY_MAX(double d=1.0, BranchTbl tbl=NULL);
    BitVarBranch BIT_VAR_ACTIVITY_MAX(BitActivity a, BranchTbl tbl=NULL);
    BitVarBranch BIT_VAR_SIZE_MIN(BranchTbl tbl=NULL);
}

namespace BV {
    namespace Bit {
        class ValCommitBitSplit: public ValCommit<BitView,BitType> {
            public:
                ValCommitBitSplit(Space& home, const ValBranch& vb);
                ValCommitBitSplit(Space& home, bool share, ValCommitBitSplit& vc);
                ModEvent commit(Space& home, unsigned int a, BitView x, int i, BitType n);
                void print(const Space&, unsigned int a, BitView, int i, int n, std::ostream& o) const;
                NGL* ngl(Space& home, unsigned int a, BitView x, BitType n) const;
        };
    }
}

// was branch_val.h
namespace BV {
    // value selection class
    class BitValBranch : public ValBranch {
        public:
            enum Select {
                SEL_MIN_BIT,
                SEL_MAX_BIT,
                SEL_RND_BIT,
                SEL_COMMIT
            };
        protected:
            Select s;
        public:
            BitValBranch(Select s0 = SEL_MIN_BIT);
            BitValBranch(Rnd r);
            BitValBranch(VoidFunction v, VoidFunction c);
            Select select(void) const;
    };

    // value selection functions
    BitValBranch BIT_VAL_MIN_BIT(void);
    BitValBranch BIT_VAL_MAX_BIT(void);
    BitValBranch BIT_VAL_RND_BIT(Rnd r);
    BitValBranch BIT_VAL(BitBranchVal v, BitBranchCommit c=NULL);
}

// was activity.h
namespace BV {
    class BitActivity : public Activity {
        public:
            BitActivity(void);
            BitActivity(const BitActivity& a);
            BitActivity& operator =(const BitActivity& a);
            BitActivity(Home home, const BitVarArgs& x, double d=1.0, BitBranchMerit bm=NULL);
            void init(Home home, const BitVarArgs& x, double d=1.0, BitBranchMerit bm=NULL);
    };
}
#endif
