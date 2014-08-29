#include "benchmark.h"
#include "nonlinearity.h"

using namespace Gecode;

Benchmark::SetRndRnd::SetRndRnd(unsigned int* skip, int skipnum) : BenchmarkSet(skip, skipnum) {
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
	branch(*this, x, SET_VAR_RND(r), SET_VAL_RND_INC(r));
}
void Benchmark::SetRndRnd::name(std::ostream& out) const {
    out << "set, integer HC";
}
void Benchmark::SetRndRnd::varsel(std::ostream& out) const {
    out << "RND";
}
void Benchmark::SetRndRnd::shortname(std::ostream& out) const {
    out << "setinthcrnd";
}
