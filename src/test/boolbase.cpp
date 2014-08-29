#include <bitset>
#include "test.h"
#include "gecodeutil.h"
#include "util.h"

using namespace Gecode;

TestSBox::TestBool::TestBool() : TestBase(),
//, n(6), m(4),
//	threshold(14),
    x(*this, std::pow(2,n) * m)
//                               ,
//	intvals(*this, std::pow(2,n), 0, std::pow(2,m))
{
    // jam the values into x
	for(int i=0; i<std::pow(2,n); i++) {
        for(int bitidx=0; bitidx<m; bitidx++) {
            int b = (solution[i] bitand (1U << bitidx)) > 0 ? 1 : 0;
            x[(i*m)+bitidx] = BoolVar(*this, b, b);
        }
	}

    #include "model/boolean/setup.cpp"
    #include "model/boolean/channeling.cpp"
}

TestSBox::TestBool::TestBool(bool share, TestBool& sb) : 
    TestSBox::TestBase(share, sb)//, 
    //n(sb.n), m(sb.m), threshold(sb.threshold) 
{
    x.update(*this, share, sb.x);
    //intvals.update(*this, share, sb.intvals);
}
Gecode::Space* TestSBox::TestBool::copy(bool share) {
    return new TestBool(share, *this);
}
/*
void TestSBox::TestBool::print(std::ostream&  out) const {
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
