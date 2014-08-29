#ifndef TESTBOX_H
#define TESTBOX_H

#include <gecode/set.hh>
#include "sboxbase.h"
#include "options.h"
#include "bitvec/bitvec.h"
#include "bitvec/array.h"

#include "gecodeutil.h"
#include "util.h"

using BV::BitVarArray;
using Gecode::IntVarArray;
using Gecode::IntVarArgs;

namespace TestSBox {
	/* solution given by paper for a 6x4 s-box
	0 3 5 6 9 10 15 12 7 4 14 13 2 1 8 11
	3 0 6 5 10 9 12 15 4 7 13 14 1 2 11 8
	3 15 0 12 5 9 10 6 4 8 11 7 14 2 1 13
	9 5 15 3 12 0 6 10 7 11 8 4 2 14 13 1

	in correct order:

	>>> from itertools import chain, izip
	>>> v1 = [0, 3, 5, 6, 9, 10, 15, 12, 7, 4, 14, 13, 2, 1, 8, 11]
	>>> v2 = [3, 0, 6, 5, 10, 9, 12, 15, 4, 7, 13, 14, 1, 2, 11, 8]
	>>> v3 = [3, 15, 0, 12, 5, 9, 10, 6, 4, 8, 11, 7, 14, 2, 1, 13]
	>>> v4 = [9, 5, 15, 3, 12, 0, 6, 10, 7, 11, 8, 4, 2, 14, 13, 1]
	>>> list(chain.from_iterable(izip(v1, v2))) + list(chain.from_iterable(izip(v3, v4)))
	*/
	/*
	const
	unsigned
	int solution[64]  = {0, 3, 3, 0, 5, 6, 6, 5, 9, 10, 10, 9, 15, 12, 12, 15,
						 7, 4, 4, 7, 14, 13, 13, 14, 2, 1, 1, 2, 8, 11, 11, 8,
						 3, 9, 15, 5, 0, 15, 12, 3, 5, 12, 9, 0, 10, 6, 6, 10,
						 4, 7, 8, 11, 11, 8, 7, 4, 14, 2, 2, 14, 1, 13, 13, 1};
						 */

	const unsigned
    int solution[64] = {
        /* table S1 for DES, as published in biham */
        //14,	0,	4,	15,	13,	7,	1,	4,	2,	14,	15,	2,	11,	13,	8,	1,	3,	10,	10,	6,	6,	12,	12,	11,	5,	9,	9,	5,	0,	3,	7,	8,	4,	15,	1,	12,	14,	8,	8,	2,	13,	4,	6,	9,	2,	1,	11,	7,	15,	5,	12,	11,	9,	3,	7,	14,	3,	10,	10,	0,	5,	6,	0,	13
        //15, 15, 13, 3, 7, 12, 11, 5, 14, 9, 0, 14, 9, 10, 6, 0, 4, 1, 3, 6, 8, 7, 5, 8, 1, 2, 10, 13, 2, 4, 12, 11, 15, 9, 10, 15, 2, 7, 7, 8, 9, 12, 12, 5, 5, 0, 0, 6, 3, 14, 4, 3, 13, 2, 14, 4, 6, 1, 1, 10, 8, 11, 11, 13
        // fucked up s-4 constraint, should fail
        //15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 5, 15, 13, 4, 1, 2, 4, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
        // table s2, should work
        //15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
/* symmetry s-3 */  0, 14, 11, 4, 12, 3, 1, 13, 10, 5, 4, 10, 3, 8, 13, 1, 5, 9, 14, 2, 15, 6, 2, 11, 6, 0, 9, 7, 8, 15, 7, 12, 7, 2, 13, 14, 1, 8, 10, 3, 4, 11, 3, 4, 14, 7, 0, 13, 8, 15, 2, 5, 6, 1, 15, 6, 11, 12, 12, 9, 5, 10, 9, 0
    };

    // randomly chosen
    const unsigned int skip[16] = {
        //35, 3, 36, 37, 6, 33, 40, 43, 45, 14, 11, 8, 19, 58, 26, 30
        39, 26, 5,  57
    };

    class TestBase : public  SBoxBase::SBoxBase {
        public:
            unsigned int n;     // number of input bits e.g. 6
            unsigned int m;     // number of output bits e.g. 4
			Gecode::IntVarArray intvals;	// the channeled integer representation of the s-box
			int threshold;					// scores should be <= threshold
        public:
            TestBase(void) :
                SBoxBase::SBoxBase(), n(6), m(4), threshold(14),
                intvals(*this, std::pow(2,n), 0, std::pow(2,m)) {}
            TestBase(bool share, TestBase& sb) :
                SBoxBase::SBoxBase(share, sb),
                n(sb.n), m(sb.m), threshold(sb.threshold) {
                    intvals.update(*this, share, sb.intvals);
                }
            bool verify(std::ostream& out) {
                for(int i=0; i<std::pow(2,n); i++) {
                    if(intvals[i].assigned()) {
                        if(intvals[i].val() != solution[i]) {
                            out << "intvals[" << i << "]: " << intvals[i].val()
                                << " != " << solution[i] << ", FAIL." << std::endl;
                        }
                    } else {
                        out << "intvals[" << i << "] not assigned, FAIL." << std::endl;
                    }
                }
            }
            void sol(std::ostream& out) const {
                out << "using solution:" << std::endl;
                for(int i=0; i<pow(2,n); i++) {
                    out << " " << solution[i] << ",";
                }
                out << std::endl;
            }
            //void print(std::ostream& out) const { return; }
            void print(std::ostream&  out) const {
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
    };

	class TestSBox : public TestBase {
        public:
            Gecode::SetVarArray x;   // the s-box contents
			IntArgs weight;
			IntArgs elements;
		public:
            TestSBox(void);
            TestSBox(bool share, TestSBox& sb);
            virtual Gecode::Space* copy(bool share);
            //void print(std::ostream& out) const;
	};

    class TestS2 : public TestSBox {
        public:
            TestS2(void);
    };
    class TestS3 : public TestSBox {
        public:
            TestS3(void);
    };
    class TestS4 : public TestSBox {
        public:
            TestS4(void);
    };
    class TestS5 : public TestSBox {
        public:
            TestS5(void);
    };
    class TestS6 : public TestSBox {
        public:
            TestS6(void);
    };
    class TestS7 : public TestSBox {
        public:
            TestS7(void);
    };

    class TestBit : public TestBase {
        public:
            //unsigned int n;
            //unsigned int m;
            BitVarArray x;
            //IntVarArray intvals;
            //int threshold;
        public:
            TestBit(void);
            TestBit(bool share, TestBit& sb);
            virtual Gecode::Space* copy(bool share);
            //void print(std::ostream& out) const;
    };
    class BitS2 : public TestBit {
        public:
            BitS2(void);
    };
    class BitS2Alt : public TestBit {
        public:
            BitS2Alt(void);
    };
    class BitS3 : public TestBit {
        public:
            BitS3(void);
    };
    class BitS4 : public TestBit {
        public:
            BitS4(void);
    };
    class BitS5 : public TestBit {
        public:
            BitS5(void);
    };
    class BitS6 : public TestBit {
        public:
            BitS6(void);
    };
    class BitS7 : public TestBit {
        public:
            BitS7(void);
    };
    class BitS7Global : public TestBit {
        public:
            BitS7Global(void);
    };

    class BitLast : public TestBit {
        public:
            BitLast(void);
    };

	class TestBool : public TestBase {
        public:
            //unsigned int n;     // number of input bits e.g. 6
            //unsigned int m;     // number of output bits e.g. 4
            Gecode::BoolVarArray x;   // the s-box contents
			//Gecode::IntVarArray intvals;	// the channeled integer representation of the s-box
			//int threshold;					// scores should be <= threshold
		public:
            TestBool(void);
            TestBool(bool share, TestBool& sb);
            virtual Gecode::Space* copy(bool share);
            //void print(std::ostream& out) const;
	};

    class BoolS1 : public TestBool {
        public:
            BoolS1(void);
    };
    class BoolS2 : public TestBool {
        public:
            BoolS2(void);
    };
    class BoolS3 : public TestBool {
        public:
            BoolS3(void);
    };
    class BoolS4 : public TestBool {
        public:
            BoolS4(void);
    };
    class BoolS5 : public TestBool {
        public:
            BoolS5(void);
    };
    class BoolS6 : public TestBool {
        public:
            BoolS6(void);
    };
    class BoolS7 : public TestBool {
        public:
            BoolS7(void);
    };

    class FullTestBase : public  SBoxBase::SBoxBase {
        public:
            FullTestBase(void) : SBoxBase::SBoxBase() {}
            FullTestBase(bool share, FullTestBase& sb) : SBoxBase::SBoxBase(share, sb) {}
            virtual bool after(std::ostream& out) const = 0;
            void print(std::ostream& out) const { return; }
    };

    class BoolChannelTest : public FullTestBase {
        public:
            BoolVarArray x;
            IntVarArray intvals;
        public:
            BoolChannelTest(void);
            BoolChannelTest(bool share, BoolChannelTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };

    class ExorTest : public FullTestBase {
        public:
            BitVarArray v;
        public:
            ExorTest(void);
            ExorTest(bool share, ExorTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class EqualTest : public FullTestBase {
        public:
            BitVarArray v;
        public:
            EqualTest(void);
            EqualTest(bool share, EqualTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class DisequalTest : public FullTestBase {
        public:
            BitVarArray v;
        public:
            DisequalTest(void);
            DisequalTest(bool share, DisequalTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class WeightTest : public FullTestBase {
        public:
            BitVarArray v;
            IntVarArray w;
        public:
            WeightTest(void);
            WeightTest(bool share, WeightTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class MembershipTest : public FullTestBase {
        public:
            BitVarArray v;
            IntVarArray w;
        public:
            MembershipTest(void);
            MembershipTest(bool share, MembershipTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class ChannelTest : public FullTestBase {
        public:
            BitVarArray v;
            IntVarArray w;
        public:
            ChannelTest(void);
            ChannelTest(bool share, ChannelTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class ParityTest : public FullTestBase {
        public:
            BitVarArray v;
            BoolVarArray w;
        public:
            ParityTest(void);
            ParityTest(bool share, ParityTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
    class ConjunctionTest : public FullTestBase {
        public:
            BitVarArray v;
        public:
            ConjunctionTest(void);
            ConjunctionTest(bool share, ConjunctionTest& sb);
            virtual Gecode::Space* copy(bool share);
            bool after(std::ostream& out) const;
    };
}
#endif
