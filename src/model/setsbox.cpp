#include <cmath>
#include <bitset>
#include "setsbox.h"
#include "nonlinearity.h"
#include "util.h"
#include "gecodeutil.h"

using namespace Gecode;

SetSBox::SetSBox::SetSBox(const SBoxOptions::SBoxOptions& opt) :
    SBoxBase(),
    n(opt.n()),
    m(opt.m()),
    threshold(opt.t()),
    // x is arranged (for a 6x4 s-box):
    //    0000 0001 ...
    // 00  x0  x2 ... x30
    // 01  x1  x3 ... x31
    // 10 x32 x34 ... x62
    // 11 x33 x35 ... x63
    // this seems unusual at first, but this is due to the rows being the first and last bits 
    // of the input pattern, which means that input i0 = 000000 (row 00, col 0000)
    // and input i1 = 000001 (row 01, col 0000), i.e. the same column, next row. This pattern
    // continues until the first bit is flipped, i.e. at i32 for a 6x4 s-box
    x(*this, std::pow(2, opt.n()), IntSet::empty, 1, opt.m()),
    // the channeled int variants of x
    intvals(*this, std::pow(2,n), 0, std::pow(2,m))
{
    #include "model/setup.cpp"

    // channel the set vars to their integer representations
    #include "model/set/channeling.cpp"

    // constraint S-1; implicit by choice of variables (pg 61)
    // constraint S-2; nonlinearity constraint
    #include "model/s2.cpp"

    // constraint S-3; distinct per-row
    #include "model/s3.cpp"

    // constraints S-4, S-5, S-6
    #include "model/set/s4.cpp"
    #include "model/set/s5.cpp"
    #include "model/set/s6.cpp"

    // constraint S-7
    #include "model/set/s7.cpp"

    // potential symmetry breaking constraints
    #include "model/symmetry.cpp"

    // branching
    Rnd r(1U);
	branch(*this, x, SET_VAR_NONE(), SET_VAL_RND_INC(r));
}

// search support
SetSBox::SetSBox::SetSBox(bool share, SetSBox& sb) : SBoxBase::SBoxBase(share, sb), n(sb.n), m(sb.m), threshold(sb.threshold) {
    x.update(*this, share, sb.x);
    intvals.update(*this, share, sb.intvals);
}

Gecode::Space* SetSBox::SetSBox::copy(bool share) {
    return new SetSBox(share, *this);
}

void SetSBox::SetSBox::print(std::ostream& out) const {
    int size = std::pow(2,n);
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
