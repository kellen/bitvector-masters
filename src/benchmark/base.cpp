#include <bitset>
#include "benchmark.h"
#include "gecodeutil.h"
#include "bitvec/propagator.h"
#include "util.h"
using namespace Gecode;

// base
Benchmark::BenchmarkBase::BenchmarkBase() : SBoxBase(), n(6), m(4), threshold(14), intvals(*this, std::pow(2,n), 0, std::pow(2,m)) {}
Benchmark::BenchmarkBase::BenchmarkBase(bool share, BenchmarkBase& sb) : SBoxBase(share, sb), n(sb.n), m(sb.m), threshold(sb.threshold) {
    intvals.update(*this, share, sb.intvals);
}
bool Benchmark::BenchmarkBase::matches_solution(std::ostream& out) const {
    bool success = true;
    for(int i=0; i<std::pow(2,n); i++) {
        if(intvals[i].assigned()) {
            if(intvals[i].val() != solution[i]) {
                success = false;
                out << "intvals[" << i << "]: " << intvals[i].val()
                    << " != " << solution[i] << ", FAIL." << std::endl;
            }
        } else {
            success = false;
            out << "intvals[" << i << "] not assigned, FAIL." << std::endl;
        }
    }
    return success;
}
bool Benchmark::BenchmarkBase::verify(std::ostream& out) const {
    bool success = true;
    int size = pow(2,n);
    unsigned int ints[size];
    bool ok = copyvars(intvals, ints, size);
    if(!ok) {
        success = false;
    } else {
        bool fail = check(ints, n, m, threshold, out);
        if(fail) {
            success = false;
        }
    }
    return success;

}
void Benchmark::BenchmarkBase::print(std::ostream& out) const {
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

// set base
Benchmark::BenchmarkSet::BenchmarkSet(unsigned int* skip, int skipnum) : BenchmarkBase(), x(*this, std::pow(2, n)) {
    // copy the solution into set variables
    for(int i=0; i<std::pow(2,n); i++) {
        // skip the values in skip
        bool skipthis = false;
        for(int s=0; s<skipnum; s++) {
            if (i == skip[s]) {
                skipthis = true;
            }
        }
        if(skipthis) {
            x[i] = SetVar(*this, IntSet::empty, 1, m);
            continue;
        }

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
Benchmark::BenchmarkSet::BenchmarkSet(bool share, BenchmarkSet& sb) : BenchmarkBase(share, sb) {
    x.update(*this, share, sb.x);
}
Gecode::Space* Benchmark::BenchmarkSet::copy(bool share) {
    return new BenchmarkSet(share, *this);
}
void Benchmark::BenchmarkSet::name(std::ostream& out) const {}
void Benchmark::BenchmarkSet::varsel(std::ostream& out) const {}
void Benchmark::BenchmarkSet::shortname(std::ostream& out) const {}

// bit base
Benchmark::BenchmarkBit::BenchmarkBit(unsigned int* skip, int skipnum) : BenchmarkBase(), x(*this, std::pow(2,n)) {
    // jam the values into x
    /*
    std::cout << "using solution: " << std::endl;
    for(int i=0; i<std::pow(2,n); i++) {
        std::cout << "\t" << solution[i] << ",";
    }
    std::cout << std::endl;
    std::cout << "skipping: ";
    for(int s=0; s<skipnum; s++) {
        std::cout << " " << skip[s] << ",";
    }
    std::cout << std::endl;

    std::cout << "starting point: " << std::endl;
    */
    for(int i=0; i<std::pow(2,n); i++) {
        // skip the values in skip
        bool skipthis = false;
        for(int s=0; s<skipnum; s++) {
            if (i == skip[s]) {
                skipthis = true;
                //std::cout << "skipping " << i << std::endl;
            }
        }
        if(skipthis) {
        //    std::cout << "\t-,";
            x[i] = BitVar(*this, m);
            continue;
        }
        //std::cout << "\t" << solution[i] << ",";
        x[i] = BitVar(*this, m, solution[i], solution[i]);
    }
    //std::cout << std::endl;

#include "model/bitvec/channeling.cpp"
}
Benchmark::BenchmarkBit::BenchmarkBit(bool share, BenchmarkBit& sb) : BenchmarkBase(share, sb) {
    x.update(*this, share, sb.x);
}
Gecode::Space* Benchmark::BenchmarkBit::copy(bool share) {
    return new BenchmarkBit(share, *this);
}
void Benchmark::BenchmarkBit::name(std::ostream& out) const {}
void Benchmark::BenchmarkBit::varsel(std::ostream& out) const {}
void Benchmark::BenchmarkBit::shortname(std::ostream& out) const {}
/*
bool Benchmark::BenchmarkBit::matches_solution(std::ostream& out) const {
    bool success = true;
    for(int i=0; i<std::pow(2,n); i++) {
        if(x[i].assigned()) {
            if(x[i].val() != solution[i]) {
                success = false;
                out << "x[" << i << "]: " << x[i].val()
                    << " != " << solution[i] << ", FAIL." << std::endl;
            }
        } else {
            success = false;
            out << "x[" << i << "] not assigned, FAIL." << std::endl;
        }
    }
    return success;
}
*/

// bool base
Benchmark::BenchmarkBool::BenchmarkBool(unsigned int* skip, int skipnum) : BenchmarkBase(), x(*this, std::pow(2,n)*m) {
#include "model/boolean/setup.cpp"
    // jam the values into x
    for(int i=0; i<pow(2,n); i++) {
        // skip the values in skip
        bool skipthis = false;
        for(int s=0; s<skipnum; s++) {
            if (i == skip[s]) {
                skipthis = true;
                break;
            }
        }
        if(skipthis) {
            for(int bitidx=0; bitidx<m; bitidx++) {
                x[(i*m)+bitidx] = BoolVar(*this, 0, 1);
            }
            continue;
        }

        for(int bitidx=0; bitidx<m; bitidx++) {
            int b = (solution[i] bitand (1U << bitidx)) > 0 ? 1 : 0;
            x[(i*m)+bitidx] = BoolVar(*this, b, b);
        }
    }
#include "model/boolean/channeling.cpp"
}
Benchmark::BenchmarkBool::BenchmarkBool(bool share, BenchmarkBool& sb) : BenchmarkBase(share, sb) {
    x.update(*this, share, sb.x);
}
Gecode::Space* Benchmark::BenchmarkBool::copy(bool share) {
    return new BenchmarkBool(share, *this);
}
void Benchmark::BenchmarkBool::name(std::ostream& out) const {}
void Benchmark::BenchmarkBool::varsel(std::ostream& out) const {}
void Benchmark::BenchmarkBool::shortname(std::ostream& out) const {}
