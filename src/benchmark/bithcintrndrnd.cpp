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

Benchmark::BitHCIntRndRnd::BitHCIntRndRnd(unsigned int* skip, int skipnum) : BenchmarkBit(skip, skipnum) {

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
    branch(*this, x, BIT_VAR_RND(r), BIT_VAL_RND_BIT(r));
}
void Benchmark::BitHCIntRndRnd::name(std::ostream& out) const {
    out << "bitvector, integer HC";
}
void Benchmark::BitHCIntRndRnd::varsel(std::ostream& out) const {
    out << "RND";
}
void Benchmark::BitHCIntRndRnd::shortname(std::ostream& out) const {
    out << "bitinthcrnd";
}
