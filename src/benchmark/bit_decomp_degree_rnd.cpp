#include "benchmark.h"

#include <bitset>
#include "nonlinearity.h"
#include "util.h"

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

Benchmark::BitDecompDegreeRnd::BitDecompDegreeRnd(unsigned int* skip, int skipnum) : BenchmarkBit(skip, skipnum) {

    #include "model/setup.cpp"
    // already included in constructor
    //#include "model/bitvec/channeling.cpp"
    #include "model/bitvec/s2decomp.cpp"
    #include "model/s3.cpp"
    #include "model/bitvec/s4.cpp"
    #include "model/bitvec/s5.cpp"
    #include "model/bitvec/s6.cpp"
    #include "model/bitvec/s7.cpp"
    #include "model/symmetry.cpp"

    Rnd r(1U);
    branch(*this, x, BIT_VAR_DEGREE_MAX(), BIT_VAL_RND_BIT(r));
}
void Benchmark::BitDecompDegreeRnd::name(std::ostream& out) const {
    out << "bitvector, decomp HC";
}
void Benchmark::BitDecompDegreeRnd::varsel(std::ostream& out) const {
    out << "DEGREE";
}
void Benchmark::BitDecompDegreeRnd::shortname(std::ostream& out) const {
    out << "bitdecomphcdegree";
}

