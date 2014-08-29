#include <bitset>
#include "sbox.h"
#include "nonlinearity.h"
#include "util.h"
#include "gecodeutil.h"
#include "options.h"

// bitvec
#include "bitvec/bitvec.h"
#include "bitvec/branch.h"
#include "bitvec/propagator.h"

using BV::BitVarBranch;
using BV::BIT_VAR_NONE;
using BV::BIT_VAR_RND;
using BV::BIT_VAR_DEGREE_MAX;
using BV::BIT_VAR_ACTIVITY_MAX;
using BV::BIT_VAL_RND_BIT;

using Gecode::BOT_EQV;

#define RELS false
#define DEBUG false
#define LOG(x) if(DEBUG){std::cerr << x;}
#define REL(x,y) if(RELS){std::cerr << std::setfill('0') << std::setw(2) << x << "->" << std::setfill('0') << std::setw(2) << y << std::endl << std::setfill('0') << std::setw(2) << y << "->" << std::setfill('0') << std::setw(2) << x << std::endl;}

SBox::SBox::SBox(const SBoxOptions::SBoxOptions& opt) :
    n(opt.n()),
    m(opt.m()),
    threshold(opt.t()),
    // order as specified in setsbox.cpp
    x(*this, pow(2,n), m),
    intvals(*this, pow(2,n), 0, pow(2,m))
{
    #include "model/setup.cpp"
    LOG("init,");

    #include "model/bitvec/channeling.cpp"
    LOG("channeled,");

    // constraint S-1; implicit by choice of variables (pg 61)
    // constraint S-2; nonlinearity constraint
    if(opt.s2() == SBoxOptions::S2_DECOMP) {
            #include "model/bitvec/s2decomp.cpp"
    } else if(opt.s2() == SBoxOptions::S2_HC) {
            #include "model/bitvec/s2.cpp"
    } else {
        if (opt.s2() != SBoxOptions::S2_HC_INT) {
            std::cerr << "Invalid S2 type, using integer S2_HC fallback." << std::endl;
        }
        #include "model/s2.cpp"
    }
    LOG("nonlinearity,");

    // constraint S-3; distinct per-row
    #include "model/s3.cpp"
    LOG("s-3,");

    // constraints S-4, S-5, S-6
    #include "model/bitvec/s4.cpp"
    #include "model/bitvec/s5.cpp"
    #include "model/bitvec/s6.cpp"
    LOG("s-4&s-5&s-6");

    // constraint S-7
    if(opt.s2() == SBoxOptions::S7_GLOBAL) {
        #include "model/bitvec/s7global.cpp"
    } else {
        if(opt.s2() != SBoxOptions::S7_DECOMP) {
            std::cerr << "Invalid S7 type, using decomposed s7.";
        }
        #include "model/bitvec/s7.cpp"
    }
    LOG("s-7");

    // potential symmetry breaking constraints
    #include "model/symmetry.cpp"
    LOG("symmetry");

    LOG("branching now...");
    Rnd r(1U);
    // branching

    BitVarBranch var_sel;
    switch(opt.branching()) {
        case SBoxOptions::BRANCH_VAR_NONE:
            var_sel = BIT_VAR_NONE();
            break;
        case SBoxOptions::BRANCH_VAR_RND:
            var_sel = BIT_VAR_RND(r);
            break;
        case SBoxOptions::BRANCH_VAR_DEGREE_MAX:
            var_sel = BIT_VAR_DEGREE_MAX();
            break;
        case SBoxOptions::BRANCH_VAR_ACTIVITY_MAX:
            var_sel = BIT_VAR_ACTIVITY_MAX(1.0, NULL);
            break;
        default:
            std::cerr << "Unknown branching chosen, using fallback BIT_VAR_NONE()." << std::endl;
            var_sel = BIT_VAR_NONE();
    }
    branch(*this, x, var_sel, BIT_VAL_RND_BIT(r));
}

// search support
SBox::SBox::SBox(bool share, SBox& sb) : SBoxBase::SBoxBase(share, sb), n(sb.n), m(sb.m), threshold(sb.threshold) {
    x.update(*this, share, sb.x);
    intvals.update(*this, share, sb.intvals);
}

Gecode::Space* SBox::SBox::copy(bool share) {
    return new SBox(share, *this);
}

void SBox::SBox::print(std::ostream& out) const {
    int size = pow(2,n);
    int ints[size];
    copyvars(intvals, ints, size);

    int num_scores = numscores(n,m);
    int scores[num_scores];
    calcscores(ints, scores, n, m);

    out << "threshold: " << threshold << std::endl;
    out << "score: " << score(scores, n, m) << std::endl;
    printbox(ints, n, out);
    printarray(ints, n, out);
}
