#include <bitset>
#include "test.h"
#include "gecodeutil.h"
#include "bitvec/propagator.h"
#include "util.h"

using namespace Gecode;

TestSBox::TestBit::TestBit() : TestBase(),// n(6), m(4),
	//threshold(14),
    x(*this, std::pow(2,n))//,
	//intvals(*this, std::pow(2,n), 0, std::pow(2,m))
{
    // jam the values into x
	for(int i=0; i<std::pow(2,n); i++) {
        bool skipthis = false;
        for(int j=0; j<4; j++) {
            if(i == skip[j]) {
                skipthis = true;
            }
        }
        if(skipthis) {
            x[i] = BitVar(*this, m);
            continue;
        }
        x[i] = BitVar(*this, m, solution[i], solution[i]);
	}

    #include "model/bitvec/channeling.cpp"
}

TestSBox::TestBit::TestBit(bool share, TestBit& sb) : TestSBox::TestBase(share, sb)
//, n(sb.n), m(sb.m), threshold(sb.threshold) 
{
    x.update(*this, share, sb.x);
    //intvals.update(*this, share, sb.intvals);
}
Gecode::Space* TestSBox::TestBit::copy(bool share) {
    return new TestBit(share, *this);
}
/*
void TestSBox::TestBit::print(std::ostream&  out) const {
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
*/
