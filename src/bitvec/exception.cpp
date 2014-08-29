#include "bitvec/exception.h"

namespace b = BV::Bit;

b::GeneralError::GeneralError(const char* l) : Exception(l, "OH NOES") {}
b::VariableEmptyDomain::VariableEmptyDomain(const char* l) : Exception(l, "Attempt to create a variable with an empty domain.") {}
b::OutOfLimits::OutOfLimits(const char* l) : Exception(l, "Out of limits.") {}
b::InvalidBounds::InvalidBounds(const char* l) : Exception(l, "Invalid bounds.") {}
b::UnknownBranching::UnknownBranching(const char* l) : Exception(l,"Unknown branching (variable or value)") {}
