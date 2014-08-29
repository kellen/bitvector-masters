#include "bitvec/branch.h"

using BV::BitVarBranch;
using BV::BitValBranch;
using BV::BitActivity;

using Gecode::Bit::ME_BIT_FAILED;
using Gecode::Bit::ME_BIT_NONE;
using Gecode::Bit::ME_BIT_VAL;
using Gecode::Bit::ME_BIT_LOWER;
using Gecode::Bit::ME_BIT_UPPER;
using Gecode::Bit::ME_BIT_BND;

#define DEBUG false

/* variable-value branchings */
namespace BV {
    // branch function
    BrancherHandle branch(Home home, const BitVarArgs& x, BitVarBranch vars,
            BitValBranch vals, BitBranchFilter bf, BitVarValPrint vvp) {
        using namespace Bit;
        if(home.failed()) {
            return BrancherHandle();
        }
        vars.expand(home, x);
        ViewArray<BitView> xv(home, x);
        ViewSel<BitView>* vs[1] = {viewsel(home, vars)};
        // XXX kill me. the 1 and 2 are documented on page 436 of MPG
        return ViewValBrancher<BitView, 1, BitType, 2>::post(home, xv, vs, valselcommit(home, vals), bf, vvp);
    }
    // branch function with tie-breaking
    BrancherHandle branch(Home home, const BitVarArgs& x, TieBreak<BitVarBranch> vars,
            BitValBranch vals, BitBranchFilter bf, BitVarValPrint vvp) {
        using namespace Bit;
        if(home.failed()) {
            return BrancherHandle();
        }
        vars.a.expand(home, x);
        // normalizing the tie-breaking
        if((vars.a.select() == BitVarBranch::SEL_NONE) || (vars.a.select() == BitVarBranch::SEL_RND)) {
            vars.b = BIT_VAR_NONE();
        }
        vars.b.expand(home, x);
        if((vars.b.select() == BitVarBranch::SEL_NONE) || (vars.b.select() == BitVarBranch::SEL_RND)) {
            vars.c = BIT_VAR_NONE();
        }
        vars.c.expand(home, x);
        if((vars.c.select() == BitVarBranch::SEL_NONE) || (vars.c.select() == BitVarBranch::SEL_RND)) {
            vars.d = BIT_VAR_NONE();
        }
        vars.d.expand(home, x);

        // yay magic numbers
        ViewArray<BitView> xv(home, x);
        if(vars.b.select() == BitVarBranch::SEL_NONE) {
            ViewSel<BitView>* vs[1] = {
                viewsel(home, vars.a)
            };
            return ViewValBrancher<BitView, 1, BitType, 2>::post(home, xv, vs, valselcommit(home, vals), bf, vvp);
        } else if (vars.c.select() == BitVarBranch::SEL_NONE) {
            ViewSel<BitView>* vs[2] = {
                viewsel(home, vars.a), viewsel(home, vars.b)
            };
            return ViewValBrancher<BitView, 2, BitType, 2>::post(home, xv, vs, valselcommit(home, vals), bf, vvp);
        } else if (vars.d.select() == BitVarBranch::SEL_NONE) {
            ViewSel<BitView>* vs[3] = {
                viewsel(home, vars.a), viewsel(home, vars.b), viewsel(home, vars.c)
            };
            return ViewValBrancher<BitView, 3, BitType, 2>::post(home, xv, vs, valselcommit(home, vals), bf, vvp);
        } else {
            ViewSel<BitView>* vs[4] = {
                viewsel(home, vars.a), viewsel(home, vars.b), viewsel(home, vars.c), viewsel(home, vars.d)
            };
            return ViewValBrancher<BitView, 4, BitType, 2>::post(home, xv, vs, valselcommit(home, vals), bf, vvp);
        }
    }
}


// view selection creation function
ViewSel<BitView>* BV::Bit::viewsel(Space& home, const BitVarBranch& bvb) {
    if(bvb.select() == BitVarBranch::SEL_NONE) {
        return new (home) ViewSelNone<BitView>(home, bvb);
    }
    if(bvb.select() == BitVarBranch::SEL_RND) {
        return new (home) ViewSelRnd<BitView>(home, bvb);
    }
    if(bvb.tbl() != NULL) {
        // view selection with table
        switch(bvb.select()) {
            case BitVarBranch::SEL_MERIT_MAX:
                return new (home) ViewSelMaxTbl<MeritFunction<BitView> >(home, bvb);
            case BitVarBranch::SEL_DEGREE_MAX:
                return new (home) ViewSelMaxTbl<MeritDegree<BitView> >(home, bvb);
            case BitVarBranch::SEL_ACTIVITY_MAX:
                return new (home) ViewSelMaxTbl<MeritActivity<BitView> >(home, bvb);
            case BitVarBranch::SEL_SIZE_MIN:
                return new (home) ViewSelMinTbl<MeritSize>(home, bvb);
            default: ;
        }
    } else {
        // view selection w/o table
        switch(bvb.select()) {
            case BitVarBranch::SEL_MERIT_MAX:
                return new (home) ViewSelMax<MeritFunction<BitView> >(home, bvb);
            case BitVarBranch::SEL_DEGREE_MAX:
                return new (home) ViewSelMax<MeritDegree<BitView> >(home, bvb);
            case BitVarBranch::SEL_ACTIVITY_MAX:
                return new (home) ViewSelMax<MeritActivity<BitView> >(home, bvb);
            case BitVarBranch::SEL_SIZE_MIN:
                return new (home) ViewSelMin<MeritSize>(home, bvb);
            default: ;
        }
    }
    throw UnknownBranching("Bit::branch");
}

// value selection and commit creation function
BV::Bit::ValCommitBitSplit::ValCommitBitSplit(Space& home, const ValBranch& vb) : ValCommit<BitView,BitType>(home, vb) {}
BV::Bit::ValCommitBitSplit::ValCommitBitSplit(Space& home, bool share, ValCommitBitSplit& vc) : ValCommit<BitView,BitType>(home, share, vc) {}
ModEvent BV::Bit::ValCommitBitSplit::commit(Space& home, unsigned int a, BitView x, int i, BitType n) {
    if(DEBUGGING_CHECKS) {
        // ensure n is within the mask
        if(!(n < x.mask())) {
            std::cerr << std::endl;
            print(home, a, x, i, n, std::cerr);
            std::cerr << std::endl;
            throw GeneralError("Value selected is outside of the available bits");
        }
        if((std::bitset<NUMBITS>(n)).count() > 1) {
            std::cerr << std::endl;
            print(home, a, x, i, n, std::cerr);
            std::cerr << std::endl;
            throw GeneralError("More than one bit set");
        }
    }
    if((fixed(x.upper(), x.lower()) bitand n) != 0) {
        // if this bit is already fixed, check its value

        if(!(((a!=0) && ((x.upper() bitand (compl n)) == x.upper())) || ((a==0) && ((x.lower() bitor n) == x.lower())))) {
            return ME_BIT_FAILED;
        } else {
            return ME_BIT_NONE;
        }
    }
    // for option 0, fix bit to 1 (i.e. set it to 1 in the lower bound)
    // for option 1, fix bit to 0 (i.e. set it to 0 in the upper bound), 
    if(DEBUG) {
        std::cerr << std::endl;
        print(home, a, x, i, n, std::cerr);
        std::cerr << std::endl;
    }
    return (a == 0) ? 
        x.bounds(home, x.upper(), x.lower() bitor n)
        : 
        x.bounds(home, x.upper() bitand (compl n), x.lower()) 
        ;
}
void BV::Bit::ValCommitBitSplit::print(const Space&, unsigned int a, BitView x, int i, int n, std::ostream& o) const {
    o << "x[" << i << "] " << std::endl << "\t"
        << std::bitset<CHAR_BIT * sizeof(unsigned int)>(x.lower())  << std::endl << "\t"
        << std::bitset<CHAR_BIT * sizeof(unsigned int)>(x.upper()) << std::endl << "\t"
        << std::bitset<CHAR_BIT * sizeof(unsigned int)>(n) 
        << " = " << ((a == 0) ? 1 : 0);
}
NGL* BV::Bit::ValCommitBitSplit::ngl(Space& home, unsigned int a, BitView x, BitType n) const {
    // TODO nogoods: implement NGLs later if necessary
    return NULL;
}

ValSelCommitBase<BitView, BitType>* BV::Bit::valselcommit(Space& home, const BitValBranch& bvb) {
    switch(bvb.select()) {
        case BitValBranch::SEL_MIN_BIT:
            return new (home) ValSelCommit<ValSelMinBit, ValCommitBitSplit>(home, bvb);
        case BitValBranch::SEL_MAX_BIT:
            return new (home) ValSelCommit<ValSelMaxBit, ValCommitBitSplit>(home, bvb);
        case BitValBranch::SEL_RND_BIT:
            return new (home) ValSelCommit<ValSelRndBit, ValCommitBitSplit>(home, bvb);
        case BitValBranch::SEL_COMMIT:
            // user defined value/commit functions
            if(bvb.commit() == NULL) {
                return new (home) ValSelCommit<ValSelFunction<BitView>, ValCommitBitSplit>(home, bvb);
            } else {
                return new (home) ValSelCommit<ValSelFunction<BitView>, ValCommitFunction<BitView> >(home, bvb);
            }
        default:
            throw UnknownBranching("Bit::branch");
    }
}

// was branch_var.cpp
BV::BitVarBranch::BitVarBranch(void) : VarBranch(NULL), s(SEL_NONE) {}
BV::BitVarBranch::BitVarBranch(Rnd r) : VarBranch(r), s(SEL_RND) {}
BV::BitVarBranch::BitVarBranch(Select s0, BranchTbl t) : VarBranch(t), s(s0) {}
BV::BitVarBranch::BitVarBranch(Select s0, double d, BranchTbl t) : VarBranch(d, t), s(s0) {}
BV::BitVarBranch::BitVarBranch(Select s0, Activity a, BranchTbl t) : VarBranch(a, t), s(s0) {}
BV::BitVarBranch::BitVarBranch(Select s0, VoidFunction mf, BranchTbl t) : VarBranch(mf, t), s(s0) {}
BV::BitVarBranch::Select BV::BitVarBranch::select(void) const {
    return s;
}

void BV::BitVarBranch::expand(Home home, const BitVarArgs& x) {
    if((select() == SEL_ACTIVITY_MAX) && !activity().initialized()) {
        activity(BitActivity(home, x, decay()));
    }
}

// variable selection function implementations
BV::BitVarBranch BV::BIT_VAR_NONE(void) {
    return BitVarBranch(BitVarBranch::SEL_NONE, NULL);
}
BV::BitVarBranch BV::BIT_VAR_RND(Rnd r) {
    return BitVarBranch(r);
}
BV::BitVarBranch BV::BIT_VAR_MERIT_MAX(BitBranchMerit bm, BranchTbl tbl) {
    return BitVarBranch(BitVarBranch::SEL_MERIT_MAX, function_cast<VoidFunction>(bm), tbl);
}
BV::BitVarBranch BV::BIT_VAR_DEGREE_MAX(BranchTbl tbl) {
    return BitVarBranch(BitVarBranch::SEL_DEGREE_MAX, tbl);
}
BV::BitVarBranch BV::BIT_VAR_ACTIVITY_MAX(double d, BranchTbl tbl) {
    return BitVarBranch(BitVarBranch::SEL_ACTIVITY_MAX, d, tbl);
}
BV::BitVarBranch BV::BIT_VAR_ACTIVITY_MAX(BitActivity a, BranchTbl tbl) {
    return BitVarBranch(BitVarBranch::SEL_ACTIVITY_MAX, a, tbl);
}
BV::BitVarBranch BV::BIT_VAR_SIZE_MIN(BranchTbl tbl) {
    return BitVarBranch(BitVarBranch::SEL_SIZE_MIN, tbl);
}

// value selection class
BV::BitValBranch::BitValBranch(Select s0) : s(s0) {}
BV::BitValBranch::BitValBranch(Rnd r) : ValBranch(r), s(BitValBranch::SEL_RND_BIT) {}
BV::BitValBranch::BitValBranch(VoidFunction v, VoidFunction c) : ValBranch(v, c), s(SEL_COMMIT) {}
BV::BitValBranch::Select BV::BitValBranch::select(void) const {
    return s;
}

// value selection functions
BV::BitValBranch BV::BIT_VAL_MIN_BIT() {
    return BitValBranch(BitValBranch::SEL_MIN_BIT);
}
BV::BitValBranch BV::BIT_VAL_MAX_BIT() {
    return BitValBranch(BitValBranch::SEL_MAX_BIT);
}
BV::BitValBranch BV::BIT_VAL_RND_BIT(Rnd r) {
    return BitValBranch(r);
}
BV::BitValBranch BV::BIT_VAL(BitBranchVal v, BitBranchCommit c) {
    return BitValBranch(function_cast<VoidFunction>(v), function_cast<VoidFunction>(c));
}

// variable activity
BV::BitActivity::BitActivity(void) {}
BV::BitActivity::BitActivity(const BitActivity& a) : Activity(a) {}
BV::BitActivity& BV::BitActivity::operator =(const BitActivity& a) {
    return static_cast<BitActivity&>(Activity::operator =(a));
}
BV::BitActivity::BitActivity(Home home, const BitVarArgs& x, double d, BitBranchMerit bm) {
    ViewArray<Bit::BitView> y(home, x);
    Activity::init(home, y, d, bm);
}
void BV::BitActivity::init(Home home, const BitVarArgs& x, double d, BitBranchMerit bm) {
    ViewArray<Bit::BitView> y(home, x);
    Activity::init(home, y, d, bm);
}
