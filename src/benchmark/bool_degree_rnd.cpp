#include "benchmark.h"
#include "nonlinearity.h"

using namespace Gecode;

Benchmark::BoolDegreeRnd::BoolDegreeRnd(unsigned int* skip, int skipnum) : BenchmarkBool(skip, skipnum) {
    #include "model/setup.cpp"
    #include "model/boolean/setup.cpp"
    // in constructor
    //#include "model/boolean/channeling.cpp"
    #include "model/s2.cpp"
    #include "model/s3.cpp"
    #include "model/boolean/s4.cpp"
    #include "model/boolean/s5.cpp"
    #include "model/boolean/s6.cpp"
    #include "model/boolean/s7.cpp"
    #include "model/symmetry.cpp"
    Rnd r(1U);
	branch(*this, x, INT_VAR_DEGREE_MAX(), INT_VAL_RND(r));
}
void Benchmark::BoolDegreeRnd::name(std::ostream& out) const {
    out << "bool, integer HC";
}
void Benchmark::BoolDegreeRnd::varsel(std::ostream& out) const {
    out << "DEGREE";
}
void Benchmark::BoolDegreeRnd::shortname(std::ostream& out) const {
    out << "boolinthcdegree";
}
