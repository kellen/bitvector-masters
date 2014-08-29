#ifndef __BITVEC_EXCEPTION_H__
#define __BITVEC_EXCEPTION_H__

#include "bitvec/bitvec.h"
using Gecode::Exception;

namespace BV {
    namespace Bit {
        class GeneralError : public Exception {
            public:
                GeneralError(const char* l);
        };
        class VariableEmptyDomain : public Exception {
            public:
                VariableEmptyDomain(const char* l);
        };
        class OutOfLimits : public Exception {
            public:
                OutOfLimits(const char* l);
        };
        class InvalidBounds : public Exception {
            public:
                InvalidBounds(const char* l);
        };
        class UnknownBranching : public Exception {
            public:
                UnknownBranching(const char* l);
        };
    }
}
#endif
