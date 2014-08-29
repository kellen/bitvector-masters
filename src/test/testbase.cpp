#include <bitset>
#include "test.h"
#include "gecodeutil.h"
#include "util.h"

using namespace Gecode;

TestSBox::TestSBox::TestSBox(void) : TestBase(),
   // n(6), m(4),
	//threshold(14),
    x(*this, std::pow(2, n)) //,
	//intvals(*this, std::pow(2,n), 0, std::pow(2,m))
{
    // copy the solution into set variables
	for(int i=0; i<std::pow(2,n); i++) {
		std::vector<int> bits_set;
		// find which bits are set in the solution;
		std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(solution[i]);
		int count = 0;
		for(int j=0; j<std::pow(2,m); j++) {
			if(bs.test(j)) {
				bits_set.push_back(j + 1);	// set uses 1-based numbering
				count++;
			}
		}
		// create an intset with these positions 
		IntSet pos(&bits_set[0], count);
		x[i] = SetVar(*this, pos, pos);
	}

    #include "model/setup.cpp"

    // should use channeling.cpp here, BUT the weights
    // are also used by s-7, so they need to get init'd
    // weights for each element in a set
    // in order to calculate the value stored in wach set
    std::vector<int> bitpos;    // elements (m)
    std::vector<int> bitvals;   // weights (2^m)
    for(int i=0; i<m; i++) {
        bitvals.push_back(std::pow(2, i));
        bitpos.push_back(i+1);  // the sets are 1-based
    }
    weight = IntArgs(bitvals);
    elements = IntArgs(bitpos);

    // constrain intvals to be the integer representaiton of x
    for(int i=0; i<std::pow(2,n); i++) {
        weights(*this, elements, weight, x[i], intvals[i]);
    }

}

// search support
TestSBox::TestSBox::TestSBox(bool share, TestSBox& sb) : 
    TestSBox::TestBase(share, sb), 
    //n(sb.n), m(sb.m), threshold(sb.threshold), 
    weight(sb.weight), elements(sb.elements) {
    x.update(*this, share, sb.x);
	//intvals.update(*this, share, sb.intvals);
}

Gecode::Space* TestSBox::TestSBox::copy(bool share) {
    return new TestSBox(share, *this);
}
/*
void TestSBox::TestSBox::print(std::ostream& out) const {
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
