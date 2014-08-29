#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <locale>

#include <gecode/set.hh>
#include <gecode/int.hh>
#include "bitvec/bitvec.h"

#include "sboxbase.h"
#include "options.h"
#include "bitvec/array.h"

using BV::BitVarArray;
using Gecode::IntVarArray;
using Gecode::IntVarArgs;

namespace Benchmark {
	const unsigned
    int solution[64] = {
/* s2 */            //15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
/* inverted s2 */   //0, 12, 14, 2, 7, 11, 1, 8, 9, 0, 4, 13, 12, 7, 11, 1, 6, 3, 8, 15, 13, 14, 2, 5, 3, 9, 15, 6, 10, 4, 5, 10, 15, 2, 1, 7, 8, 5, 4, 14, 5, 12, 11, 0, 2, 11, 14, 13, 10, 4, 7, 9, 3, 8, 9, 3, 6, 15, 12, 10, 13, 1, 0, 6
/* inverted s1 */   //1, 15, 11, 0, 2, 8, 14, 11, 13, 1, 0, 13, 4, 2, 7, 14, 12, 5, 5, 9, 9, 3, 3, 4, 10, 6, 6, 10, 15, 12, 8, 7, 11, 0, 14, 3, 1, 7, 7, 13, 2, 11, 9, 6, 13, 14, 4, 8, 0, 10, 3, 4, 6, 12, 8, 1, 12, 5, 5, 15, 10, 9, 15, 2

/* symmetry s-3 */  0, 14, 11, 4, 12, 3, 1, 13, 10, 5, 4, 10, 3, 8, 13, 1, 5, 9, 14, 2, 15, 6, 2, 11, 6, 0, 9, 7, 8, 15, 7, 12, 7, 2, 13, 14, 1, 8, 10, 3, 4, 11, 3, 4, 14, 7, 0, 13, 8, 15, 2, 5, 6, 1, 15, 6, 11, 12, 12, 9, 5, 10, 9, 0



        // fucked up s-4 constraint, should fail
        //15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 5, 15, 13, 4, 1, 2, 4, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
        //                                                                                                                                               ^                   ^
    };

    class BenchmarkBase : public SBoxBase::SBoxBase {
        public:
            unsigned int n;     // number of input bits e.g. 6
            unsigned int m;     // number of output bits e.g. 4
			int threshold;					// scores should be <= threshold
			Gecode::IntVarArray intvals;	// the channeled integer representation of the s-box
        public:
            BenchmarkBase(void);
            BenchmarkBase(bool share, BenchmarkBase& sb);
            virtual Gecode::Space* copy(bool share) = 0;
            void print(std::ostream& out) const;
            virtual void name(std::ostream& out) const = 0;
            virtual void varsel(std::ostream& out) const = 0;
            virtual void shortname(std::ostream& out) const = 0;
            bool verify(std::ostream& out) const;
            bool matches_solution(std::ostream& out) const;
    };

    class BenchmarkBool : public BenchmarkBase {
        public:
            Gecode::BoolVarArray x;   // the s-box contents
        public:
            BenchmarkBool(unsigned int* skip, int skipnum);
            BenchmarkBool(bool share, BenchmarkBool& sb);
            virtual Gecode::Space* copy(bool share);
            virtual void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BenchmarkSet : public BenchmarkBase {
        public:
            Gecode::SetVarArray x;   // the s-box contents
			IntArgs weight;
			IntArgs elements;
        public:
            BenchmarkSet(unsigned int* skip, int skipnum);
            BenchmarkSet(bool share, BenchmarkSet& sb);
            virtual Gecode::Space* copy(bool share);
            virtual void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class SetNoneRnd : public BenchmarkSet {
        public:
            SetNoneRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class SetRndRnd : public BenchmarkSet {
        public:
            SetRndRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class SetActivityRnd : public BenchmarkSet {
        public:
            SetActivityRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class SetDegreeRnd : public BenchmarkSet {
        public:
            SetDegreeRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BoolNoneRnd : public BenchmarkBool {
        public:
            BoolNoneRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BoolRndRnd : public BenchmarkBool {
        public:
            BoolRndRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BoolActivityRnd : public BenchmarkBool {
        public:
            BoolActivityRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BoolDegreeRnd : public BenchmarkBool {
        public:
            BoolDegreeRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BenchmarkBit : public BenchmarkBase {
        public:
            BitVarArray x;
        public:
            BenchmarkBit(unsigned int* skip, int skipnum);
            BenchmarkBit(bool share, BenchmarkBit& sb);
            virtual Gecode::Space* copy(bool share);
            virtual void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitDecompNoneRnd : public BenchmarkBit {
        public:
            BitDecompNoneRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitDecompRndRnd : public BenchmarkBit {
        public:
            BitDecompRndRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitDecompActivityRnd : public BenchmarkBit {
        public:
            BitDecompActivityRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitDecompDegreeRnd : public BenchmarkBit {
        public:
            BitDecompDegreeRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCNoneRnd : public BenchmarkBit {
        public:
            BitHCNoneRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCRndRnd : public BenchmarkBit {
        public:
            BitHCRndRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCActivityRnd : public BenchmarkBit {
        public:
            BitHCActivityRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCDegreeRnd : public BenchmarkBit {
        public:
            BitHCDegreeRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCIntNoneRnd : public BenchmarkBit {
        public:
            BitHCIntNoneRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCIntRndRnd : public BenchmarkBit {
        public:
            BitHCIntRndRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCIntActivityRnd : public BenchmarkBit {
        public:
            BitHCIntActivityRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCIntDegreeRnd : public BenchmarkBit {
        public:
            BitHCIntDegreeRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCS7NoneRnd : public BenchmarkBit {
        public:
            BitHCS7NoneRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCS7RndRnd : public BenchmarkBit {
        public:
            BitHCS7RndRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCS7ActivityRnd : public BenchmarkBit {
        public:
            BitHCS7ActivityRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };

    class BitHCS7DegreeRnd : public BenchmarkBit {
        public:
            BitHCS7DegreeRnd(unsigned int* skip, int skipnum);
            void name(std::ostream& out) const;
            virtual void varsel(std::ostream& out) const;
            virtual void shortname(std::ostream& out) const;
    };
}
#endif
