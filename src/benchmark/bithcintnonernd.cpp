#include "benchmark.h"

#include <bitset>
#include "nonlinearity.h"

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

Benchmark::BitHCIntNoneRnd::BitHCIntNoneRnd(unsigned int* skip, int skipnum) : BenchmarkBit(skip, skipnum) {

    #include "model/setup.cpp"
    // already included in constructor
    //#include "model/bitvec/channeling.cpp"
    #include "model/s2.cpp"
    #include "model/s3.cpp"
    #include "model/bitvec/s4.cpp"
    #include "model/bitvec/s5.cpp"
    #include "model/bitvec/s6.cpp"
    #include "model/bitvec/s7.cpp"
    #include "model/symmetry.cpp"

    Rnd r(1U);
    branch(*this, x, BIT_VAR_NONE(), BIT_VAL_RND_BIT(r));


    /*
    for(int i=0;i<pow(2,n);i++) {
        std::cout << "i:" << i;
        if(x[i].assigned()) {
            std::cout << ",v:"<<x[i].upper();
        } else {
            std::cout << ",v: unassigned" ;
        }
        std::cout << ",iv:";
        if(intvals[i].assigned()) {
            std::cout << intvals[i].val() <<std::endl;
        } else {
            std::cout << intvals[i].min() << "-" <<  intvals[i].max() << std::endl;
        }
    }
    print(std::cout);
    */
}
void Benchmark::BitHCIntNoneRnd::name(std::ostream& out) const {
    out << "bitvector, integer HC";
}
void Benchmark::BitHCIntNoneRnd::varsel(std::ostream& out) const {
    out << "NONE";
}
void Benchmark::BitHCIntNoneRnd::shortname(std::ostream& out) const {
    out << "bitinthcnone";
}
