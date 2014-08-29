#ifndef __BIT_PROP_H__
#define __BIT_PROP_H__

#include "bitvec/bitvec.h"
#include "var.h"
#include "view.h"
#include "bitvec/array.h"

#include <gecode/int.hh>

using Gecode::Propagator;
using Gecode::BinaryPropagator;
using Gecode::TernaryPropagator;
using Gecode::MixBinaryPropagator;
using Gecode::MixTernaryPropagator;
using Gecode::Home;
using Gecode::ExecStatus;
using Gecode::ModEventDelta;
using Gecode::PropCost;
using Gecode::ES_OK;
using Gecode::ES_FIX;

using Gecode::IntVar;
using Gecode::BoolVar;
using Gecode::IntVarArray;
using Gecode::Int::IntView;
using Gecode::Int::BoolView;

using BV::BitType;
using BV::Bit::BitView;
using BV::BitVar;
using BV::BitVarArray;
using BV::BitVarArgs;

using Gecode::Bit::PC_BIT_BND;
using Gecode::Bit::PC_BIT_VAL;
using Gecode::Int::PC_INT_BND;
using Gecode::Int::PC_BOOL_NONE;

/*
class BitNonLinearity : public Propagator {
    public:
        BitNonLinearity(Home home, ViewArray<BitView>& x0, int s[], bool a[], int n0, int m0, int t0, int sz);
        BitNonLinearity(Space& home, bool share, BitNonLinearity& p);
        static ExecStatus post(Home home, ViewArray<BitView>& x, int s[], bool a[], int n, int m, int t, int sz);
        Propagator* copy(Space& home, bool share);
        PropCost cost(const Space&, const ModEventDelta&) const;
        ExecStatus propagate(Space& home, const ModEventDelta& med);
        size_t dispose(Space& home);
};
*/
void xordist(
        Gecode::Home home,
        const unsigned int n,
        const unsigned int m,
        const unsigned int max_count,
        const BitVarArgs& x
        );
void nonlinearity(
		Gecode::Home home,
		const int n,
        const int m,
		const int max_score,
		const BitVarArgs& x
		);

class Equal : public BinaryPropagator<BitView,PC_BIT_BND> {
    public:
        Equal(Home home, BitView x0, BitView x1);
        Equal(Space& home, bool share, Equal& p);
        static ExecStatus post(Home home, BitView x0, BitView x1);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void equal(Space& home, BitVar x, BitVar y);

class Disequal : public BinaryPropagator<BitView,PC_BIT_BND> {
    public:
        Disequal(Home home, BitView x0, BitView x1);
        Disequal(Space& home, bool share, Disequal& p);
        static ExecStatus post(Home home, BitView x0, BitView x1);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void disequal(Space& home, BitVar x, BitVar y);

class Exor : public TernaryPropagator<BitView,PC_BIT_BND> {
    public:
        Exor(Home home, BitView x0, BitView x1, BitView x2);
        Exor(Space& home, bool share, Exor& p);
        static ExecStatus post(Home home, BitView x0, BitView x1, BitView x2);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void exor(Space& home, BitVar x0, BitVar x1, BitVar x2);

class Weight : public MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND> {
    public:
        Weight(Home home, BitView x0, IntView x1);
        Weight(Space& home, bool share, Weight& p);
        static ExecStatus post(Home home, BitView x0, IntView x1);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void weight(Space& home, BitVar x0, IntVar x1);

class Membership : public MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND> {
    public:
        Membership(Home home, BitView x0, IntView x1);
        Membership(Space& home, bool share, Membership& p);
        static ExecStatus post(Home home, BitView x0, IntView x1);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void membership(Space& home, BitVar x0, IntVar x1);
ExecStatus _membership(Space& home, BitView x0, unsigned int U, unsigned int L);

class Channel : public MixBinaryPropagator<BitView,PC_BIT_BND,IntView,PC_INT_BND> {
    public:
        Channel(Home home, BitView x0, IntView x1);
        Channel(Space& home, bool share, Channel& p);
        static ExecStatus post(Home home, BitView x0, IntView x1);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void channel(Space& home, BitVar x0, IntVar x1);
void channel(Space& home, BitVarArray x0, IntVarArray x1);

class Parity : public MixBinaryPropagator<BitView,PC_BIT_VAL,BoolView,PC_BOOL_NONE> {
    public:
        Parity(Home home, BitView x0, BoolView x1);
        Parity(Space& home, bool share, Parity& p);
        static ExecStatus post(Home home, BitView x0, BoolView x1);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void parity(Space& home, BitVar x0, BoolVar x1);

class Conjunction : public TernaryPropagator<BitView,PC_BIT_BND> {
    public:
        Conjunction(Home home, BitView x0, BitView x1, BitView x2);
        Conjunction(Space& home, bool share, Conjunction& p);
        static ExecStatus post(Home home, BitView x0, BitView x1, BitView x2);
        Propagator* copy(Space& home, bool share);
        ExecStatus propagate(Space& home, const ModEventDelta&);
};
void conjunction(Space& home, BitVar x0, BitVar x1, BitVar x2);
#endif
