/*
   linearity measure N(a,b) for a 6x4 s-box:
   n = 6, m = 4, pow(2, n) = 64
   N(a,b) = #{ i | 0<=i<pow(2,n),
   xor{s=0->n-1}(i[s] ^ a[s]) == xor{t=0->m-1}(x[t] ^ b[t]) 
   }
   that is:
   xor all bits of (input  (i) and a)
   xor all bits of (output (x) and b)
   if equal, add i to the set
   count the set size
   this is "the number of times an input bit corresponds with an output bit"

want: N(a,b)/|inputs| = 1/2, i.e.: N(a,b)/(2^n) = N(a,b)/64 = 1/2
or |N(a,b) - (2^n)/2| = 0 (or close to 0) 
which implies that all output bits depends equally on the input bits (I think)

define the effectiveness of an s-box phi over inputs:
score = effectiveness = sigma_inputs(phi) = 
max{|N(a,b) - (|inputs|/2)| : 1<=a<|inputs|, 1<=b<|possible-outputs|}
which is the "worst case" for all possible input/output combinations (i.e. the most linear)
we want the s-boxes with the smallest effectiveness values; so minimize sigma_inputs(phi)
that is: test all possible a, b (1<=a<|inputs|, 1<=b<|possible-outputs|)

for partially-filled s-boxes, use:
sigma_inputs'(phi') = max{|N_phi'_inputs'(a,b) - (|inputs|/2)|, 1<=a<|inputs|/2, 1<=b<|possible-outputs|}

instead of minimizing (a "soft constraint") we can use a threshold ("hard constraint"):
sigma_inputs(phi) <= T

use a heuristic, H_smallphi_T_I which rejects partially-assigned s-boxes with smallphi assigned variables 
which do not have a N_phi'_inputs'(a,b) >= T
I = incremental, I think.
in a 6x4, want N(a,b) = 64/2 = 32. A H_48_16_I heuristic will reject s-boxes with 48 vars without N(a,b) >= 16

a partial assignment should be rejected if it does not fulfill:
|inputs'| - T - (|inputs|/2) <= max_a,b{N_phi'_inputs'(a,b)} <= (|inputs|/2) + T
since it cannot achieve a score better than T
this is called H_smallphi_T_C (C = complete, I think)
 */

#include "nonlinearity.h"
#include <bitset>
#include "util.h"
#include "gecodeutil.h"
#define DEBUG false
#define FAIL_DEBUG false
#define SCORES_DEBUG false

using namespace Gecode;
using namespace Gecode::Int;

class NonLinearity : public Propagator {
    protected:
        // the bit vectors
        ViewArray<IntView> x;
        // their scores
        int* scores;
        // the assigned vars
        bool* assigned;
        // the input/output sizes
        int n;
        int m;
        int pow2n;
        int pow2m;
        // parameters to the partial heuristic
        int threshold;		// the maximum acceptable score before rejection; i.e. the threshold
        // convenience; size of scores array
        int size;
    public:
        NonLinearity(Home home, ViewArray<IntView>& x0, int s[], bool a[], int n0, int m0, int t0, int sz) : 
            Propagator(home), x(x0), scores(s), assigned(a), n(n0), m(m0), threshold(t0), size(sz),
            pow2n(1U << n), pow2m(1U << m) {
                // only do something when a vector becomes assigned
                x.subscribe(home, *this, PC_INT_VAL);
            }
        // copy constructor used during cloning
        NonLinearity(Space& home, bool share, NonLinearity& p) : 
            Propagator(home,share,p), n(p.n), m(p.m), threshold(p.threshold), size(p.size),
            pow2n(p.pow2n), pow2m(p.pow2m) {
                // update the view
                x.update(home, share, p.x);
                // copy the scores
                scores = home.alloc<int>(size);
                std::copy(p.scores, p.scores+size, scores);
                /*
                for(int i=0; i<size; i++) {
                    scores[i] = p.scores[i];
                }
                */
                // copy the completed assignments
                assigned = home.alloc<bool>(pow2n);
                std::copy(p.assigned,p.assigned+pow2n,assigned);
                /*
                for(int i=0; i<pow2n; i++) {
                    assigned[i] = p.assigned[i];
                }
                */
            }
        static ExecStatus post(Home home, ViewArray<IntView>& x, int s[], bool a[], int n, int m, int t, int sz) {
            if(DEBUG) { std::cerr << "p"; }
            // only post if there is something to propagate
            if(x.size() > 1) {
                (void) new (home) NonLinearity(home,x,s,a,n,m,t,sz);
            }
            return ES_OK;
        }
        // create copy during cloning
        virtual Propagator* copy(Space& home, bool share) {
            return new (home) NonLinearity(home, share, *this);
        }
        // return the cost
        virtual PropCost cost(const Space&, const ModEventDelta&) const {
            if(DEBUG) { std::cerr << "$"; }
            return PropCost::linear(PropCost::LO, x.size());
        }
        // perform the propagation
        virtual ExecStatus propagate(Space& home, const ModEventDelta& med) {
            if(DEBUG) {
                print(std::cerr);
                printscores(scores, n, m, std::cerr);
            }

            int assigned_count = 0;
            // calculate the scores for the newly assigned vars:
            for(unsigned int i=0; i<pow2n; i++) {
                if(DEBUG) { std::cerr << std::endl; }
                // if this variable has been assigned since the last run
                if(assigned[i]) {
                    assigned_count += 1;
                }
                // if this variable is newly assigned
                if((!assigned[i]) && x[i].assigned()) {
                    if(DEBUG) { std::cerr << "new " << i << std::endl; }
                    assigned[i] = true;
                    assigned_count +=1 ;
                    // for 1<=a<|X|
                    for(int a=1; a<pow2n; a++) {
                        if(DEBUG) { std::cerr << "--------" << std::endl; }
                        // for 1<=b<|D|, where b is a power of 2
                        for(unsigned int bidx=0; bidx<m; bidx++) {
                            unsigned int b = 1U << bidx;
                            //unsigned int b = pow(2,bidx);
                            if(parity(i & a) == parity(((unsigned int)x[i].val()) & b)) {
                                int pos = ((a-1)*m) + bidx;
                                scores[pos] = scores[pos] + 1;
                            }
                        }
                    }
                }
            }
            if(DEBUG) {
                std::cerr << "assigned count: " << assigned_count << std::endl;
                printscores(scores, n, m, std::cerr);
            }
            // calculate the min and max score (max{a,b}N{X',phi'}(a,b)) 
            int max = 0;
            int min = pow2n;
            for(int i=0; i<size; i++) {
                if(scores[i] > max) {
                    max = scores[i];
                }
                if(scores[i] < min) {
                    min = scores[i];
                }
            }

            // upper = (|X|/2) + T
            int upper = (1U << (n-1)) + threshold;
            if(DEBUG) {
                std::cerr << std::endl;
                std::cerr << "[" << assigned_count << "] " << max << "<=" << upper;
            }
            // if the inequality does not hold, fail the space
            if(!(max <= upper)) {
                if(FAIL_DEBUG) { 
                    std::cerr << "[" << assigned_count << "] " << max << "<=" << upper;
                    printscores(scores, n, m, std::cerr);
                }
                if(FAIL_DEBUG || DEBUG) {
                    std::cerr << "F" << std::endl;
                }
                return ES_FAILED;
            }

            // lower bounds
            // fail if all the variables are assigned but the min score is under the acceptable threshold
            // there is probably a better way to do this;
            // 
            // minlower = (|X|/2) - T - (|X| - |X'|)
            int minlower = (1U << (n-1)) - threshold - (pow2n - assigned_count);
            if(!(min >= minlower)) {
                if(FAIL_DEBUG) { 
                    std::cerr << "[" << assigned_count << "] " << minlower << "<=" << min ;
                    printscores(scores, n, m, std::cerr);
                }
                if(FAIL_DEBUG || DEBUG) {
                    std::cerr << "FL" << std::endl;
                }
                return ES_FAILED;
            }

            // when all vars are assigned, we are done.
            if(assigned_count >= pow2n) {
                if(DEBUG) { 
                    std::cerr << "S"; 
                }
                return home.ES_SUBSUMED(*this);
            }
            if(SCORES_DEBUG) { 
                std::cerr << ".";
                if(rand()%100 == 0) {
                    std::cerr << std::endl;
                }
            }
            return ES_FIX;
        }
        // dispose of the propagator and return its memory
        virtual size_t dispose(Space& home) {
            if(DEBUG) { std::cerr << "d"; }
            x.cancel(home, *this, PC_INT_VAL);
            (void) Propagator::dispose(home);
            return sizeof(*this);
        }
    private:
        void print(std::ostream& out) const {
            int ints[pow2n];
            for(int i=0; i<pow2n; i++) {
                if(x[i].assigned()) { 
                    ints[i] = x[i].val();
                } else {
                    ints[i] = -1;
                }
            }
            printbox(ints, n, out);
        }
};

void nonlinearity(Home home, const int n, const int m, const int threshold, const IntVarArgs& x) {
    if(DEBUG) { std::cerr << "P"; }
    // argument validation
    if(x.size() != pow(2,n)) {
        throw ArgumentSizeMismatch("setnonlinearity,x");
    } else if(threshold > pow(2,n)) {
        throw ArgumentSizeMismatch("setnonlinearity,threshold,high");
    } else if(!(n > 0)) {
        throw ArgumentSizeMismatch("setnonlinearity,n");
    } else if(!(m > 0)) {
        throw ArgumentSizeMismatch("setnonlinearity,m");
    } else if(!(threshold >= 0)) {
        throw ArgumentSizeMismatch("setnonlinearity,threshold,low");
    }
    // if space is already failed, don't post the propagator
    if(home.failed()) {
        return;
    }
    // create views
    ViewArray<IntView> vx(home,x);
    // size of the a,b array, for convenience 
    int size = numscores(n,m);
    // create storage for scoring
    int* scores = static_cast<Space&>(home).alloc<int>(size);
    // create storage for allocated list
    bool* assigned = static_cast<Space&>(home).alloc<bool>((int)pow(2,n));
    // not strictly sure this is ncessary, but...
    for(int i=0; i<pow(2,n); i++) {
        assigned[i] = false;
    }
    // if posting failed, fail the space
    if(NonLinearity::post(home,vx,scores,assigned,n,m,threshold,size) != ES_OK) {
        home.fail();
    }
}
