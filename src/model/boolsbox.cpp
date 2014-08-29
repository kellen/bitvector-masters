#include <cmath>
#include <bitset>
#include "boolsbox.h"
#include "nonlinearity.h"
#include "util.h"
#include "gecodeutil.h"

using namespace Gecode;

BoolSBox::BoolSBox::BoolSBox(const SBoxOptions::SBoxOptions& opt) :
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
    x(*this, std::pow(2, n) * m, 0, 1),
    // the channeled int variants of x
    intvals(*this, std::pow(2,n), 0, std::pow(2,m))
{
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"

    // channel the bools vars to their integer representation
    #include "model/boolean/channeling.cpp"

    // constraint S-1; implicit by choice of variables (pg 61)
    // constraint S-2; nonlinearity constraint
    #include "model/s2.cpp"

    // constraint S-3; distinct per-row
    #include "model/s3.cpp"

    // constraints S-4, S-5, S-6
    #include "model/boolean/s4.cpp"
    #include "model/boolean/s5.cpp"
    #include "model/boolean/s6.cpp"

    // constraint S-7
    #include "model/boolean/s7.cpp"

    // potential symmetry breaking constraints
    #include "model/symmetry.cpp"

    // branching
    Rnd r(1U);
	branch(*this, x, INT_VAR_NONE(), INT_VAL_RND(r));
}

// search support
BoolSBox::BoolSBox::BoolSBox(bool share, BoolSBox& sb) : SBoxBase::SBoxBase(share, sb), n(sb.n), m(sb.m), threshold(sb.threshold) {
    x.update(*this, share, sb.x);
    intvals.update(*this, share, sb.intvals);
}

Gecode::Space* BoolSBox::BoolSBox::copy(bool share) {
    return new BoolSBox(share, *this);
}

void BoolSBox::BoolSBox::print(std::ostream& out) const {
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
