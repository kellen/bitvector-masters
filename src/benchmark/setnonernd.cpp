#include "benchmark.h"
#include "nonlinearity.h"

using namespace Gecode;

Benchmark::SetNoneRnd::SetNoneRnd(unsigned int* skip, int skipnum) : BenchmarkSet(skip, skipnum) {
    #include "model/setup.cpp"
    // in constructor
    //#include "model/set/channeling.cpp"
    #include "model/s2.cpp"
    #include "model/s3.cpp"
    #include "model/set/s4.cpp"
    #include "model/set/s5.cpp"
    #include "model/set/s6.cpp"
    #include "model/set/s7.cpp"
    #include "model/symmetry.cpp"
    Rnd r(1U);
	branch(*this, x, SET_VAR_NONE(), SET_VAL_RND_INC(r));
}
void Benchmark::SetNoneRnd::name(std::ostream& out) const {
    out << "set, integer HC";
}
void Benchmark::SetNoneRnd::varsel(std::ostream& out) const {
    out << "NONE";
}
void Benchmark::SetNoneRnd::shortname(std::ostream& out) const {
    out << "setinthcnone";
}
