#include "bitvec/var.h"
#include "bitvec/bitvec_limits.h"
#include "bitvec/exception.h"

using BV::Bit::VariableEmptyDomain;
using BV::Bit::OutOfLimits;
using BV::Bit::InvalidBounds;

// variable creation
BV::BitVar::BitVar(void) {}
BV::BitVar::BitVar(const BV::BitVar& y) : VarImpVar<BV::Bit::BitVarImp>(y.varimp()) {}
BV::BitVar::BitVar(BV::Bit::BitVarImp* y) : VarImpVar<BV::Bit::BitVarImp>(y) {}
BV::BitVar::BitVar(Space& home, unsigned int n) : VarImpVar<BV::Bit::BitVarImp>(new (home) BV::Bit::BitVarImp(home, n)) {}
BV::BitVar::BitVar(Space& home, unsigned int n, BV::BitType lower, BV::BitType upper) : VarImpVar<BV::Bit::BitVarImp>(new (home) BV::Bit::BitVarImp(home, n, lower, upper)) {}

// access operations
BV::BitType BV::BitVar::lower(void) const {
    return x->lower();
}
BV::BitType BV::BitVar::upper(void) const {
    return x->upper();
}
BV::BitType BV::BitVar::val(void) const {
    return x->val();
}
