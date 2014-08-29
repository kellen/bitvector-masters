#ifndef SBOXOPTS_H
#define SBOXOPTS_H

#include <gecode/driver.hh>
#include <iostream>
#include <sstream>

using namespace Gecode;
using namespace Driver;

namespace SBoxOptions {
	class SBoxOptions : public Options {
		protected:
            StringOption _s2;
            StringOption _s7;
			unsigned int _n;	// number of input bits
			unsigned int _m;	// number of output bits
			unsigned int _t;	// threshold for nonlinearity constraint
		public:
			SBoxOptions(void);
			virtual void help(void);
			void parse(int& argc, char* argv[]);

			void n(unsigned int n);
			void m(unsigned int m);
			void t(unsigned int t);
			unsigned int n(void) const;
			unsigned int m(void) const;
			unsigned int t(void) const;

            void s2(int v);
            void s2(int v, const char* o, const char* h = NULL);
            int s2(void) const;

            void s7(int v);
            void s7(int v, const char* o, const char* h = NULL);
            int s7(void) const;
	};
    enum BranchType {
        BRANCH_VAR_NONE,
        BRANCH_VAR_RND,
        BRANCH_VAR_DEGREE_MAX,
        BRANCH_VAR_ACTIVITY_MAX
    };
    enum BitVecS2 {
        S2_HC, S2_HC_INT, S2_DECOMP
    };
    enum BitVecS7 {
        S7_DECOMP, S7_GLOBAL
    };
}
#endif
