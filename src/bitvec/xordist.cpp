#include "bitvec/propagator.h"
#include "bitvec/bitvec.h"
#include "bitvec/array.h"
#include <bitset>
#include "util.h"
#include "gecodeutil.h"
#define DEBUG false
#define FAIL_DEBUG false
#define SCORES_DEBUG false

using BV::BitType;
using BV::Bit::BitView;
using BV::BitVarArgs;

using Gecode::Space;
using Gecode::Propagator;
using Gecode::Int::ArgumentSizeMismatch;

using Gecode::Bit::PC_BIT_VAL;

class XorDist : public Propagator {
    protected:
        ViewArray<BitView> x;
        unsigned int* table;
        bool* assigned;
        unsigned int n;
        unsigned int m;
        unsigned int max_count;
        unsigned int pow2n;
        unsigned int pow2m;

    public:
        XorDist(Home home, ViewArray<BitView>& x0, unsigned int tab[], bool a[], unsigned int n0, unsigned int m0, unsigned int t0) : 
            Propagator(home), x(x0), table(tab), assigned(a), n(n0), m(m0), max_count(t0),
            pow2n(1U << n), pow2m(1U << m) {
                x.subscribe(home, *this, PC_BIT_VAL);
            }
        XorDist(Space& home, bool share, XorDist& p) : 
            Propagator(home,share,p), n(p.n), m(p.m), max_count(p.max_count),
            pow2n(p.pow2n), pow2m(p.pow2m) {
                x.update(home, share, p.x);
                // copy the table
                table = home.alloc<unsigned int>(pow2n * pow2m);
                std::copy(p.table, p.table+(pow2n*pow2m), table);
                /*
                for(int i=0; i<pow2n; i++) {
                    for(int j=0; j<pow2m; j++) {
                        int idx = (i*pow2m)+j;
                        table[idx] = p.table[idx];
                    }
                }
                */
                // copy the completed assignments
                assigned = home.alloc<bool>(pow2n);
                std::copy(p.assigned, p.assigned+pow2n, assigned);
                /*
                for(int i=0; i<pow2n; i++) {
                    assigned[i] = p.assigned[i];
                }
                */
            }
        static ExecStatus post(Home home, ViewArray<BitView>& x, unsigned int s[], bool a[], unsigned int n, unsigned int m, unsigned int t) {
            if(x.size() > 1) (void) new (home) XorDist(home,x,s,a,n,m,t);
            return ES_OK;
        }
        // create copy during cloning
        virtual Propagator* copy(Space& home, bool share) {
            return new (home) XorDist(home, share, *this);
        }
        // return the cost
        virtual PropCost cost(const Space&, const ModEventDelta&) const {
            return PropCost::linear(PropCost::LO, x.size());
        }
        // perform the propagation
        virtual ExecStatus propagate(Space& home, const ModEventDelta& med) {
            int assigned_count = 0;

            // create the xor distribution table
            for(unsigned int i=0; i<pow2n; i++) {
                if(!assigned[i] && x[i].assigned()) {
                    assigned_count += 1;
                    // if x[i] is newly assigned, then count everything
                    assigned[i] = true;
                    for(unsigned int j=i+1; j<pow2n; j++) {
                        if(x[j].assigned()) {
                            unsigned int idx = ((i xor j)*pow2m)+(x[i].val() xor x[j].val());
                            table[idx] = table[idx] + 1;
                        }
                    }
                } else if(x[i].assigned()) {
                    assigned_count += 1;
                    // if x[i] was already assigned, only add the newly assigned vars
                    for(unsigned int j=i+1; j<pow2n; j++) {
                        if(!assigned[j] && x[j].assigned()) {
                            unsigned int idx = ((i xor j)*pow2m)+(x[i].val() xor x[j].val());
                            table[idx] = table[idx] + 1;
                        }
                    }
                }
            }
            //std::cout << "assigned cnt: " << assigned_count << std::endl;
            // ensure nothing is over the threshold
            for(unsigned int i=0; i<pow2n; i++) {
                for(unsigned int j=0; j<pow2m; j++) {
                    unsigned int idx = (i*pow2m)+j;
                    if(table[idx] > max_count) {
                        if(DEBUG) {
                            std::cout << "failed on (" << i << "," << j << ")" << std::endl;
                            print(std::cerr);
                        }
                        return ES_FAILED;
                    }
                }
            }

            // when all vars are assigned, we are done.
            if(assigned_count >= pow2n) {
                return home.ES_SUBSUMED(*this);
            }
            return ES_FIX;
        }
        // dispose of the propagator and return its memory
        virtual size_t dispose(Space& home) {
            if(DEBUG) { std::cerr << "d"; }
            x.cancel(home, *this, PC_BIT_VAL);
            (void) Propagator::dispose(home);
            return sizeof(*this);
        }
    private:
        void print(std::ostream& out) const {
            out << "\t";
            for(int j=0; j<pow2m; j++) {
                out << j << "\t";
            }
            out << std::endl;
            for(int j=0;j<4;j++) {
                out << "------------------";
            }
            out << std::endl;
            for(int i=0; i<pow2n; i++) {
                out << i << ":\t";
                for(int j=0; j<pow2m; j++) {
                    int idx = (i*pow2m)+j;
                    out << table[idx] << "\t"; 
                }
                out << std::endl;
            }
        }
};

void xordist(Home home, const unsigned int n, const unsigned int m, const unsigned int max_count, const BitVarArgs& x) {
    // argument validation
    if(x.size() != pow(2,n)) {
        throw ArgumentSizeMismatch("xordist,x");
    } else if(max_count > pow(2,n)) {
        throw ArgumentSizeMismatch("xordist,max_count,high");
    } else if(!(n > 0)) {
        throw ArgumentSizeMismatch("xordist,n");
    } else if(!(m > 0)) {
        throw ArgumentSizeMismatch("xordist,m");
    } else if(!(max_count >= 0)) {
        throw ArgumentSizeMismatch("xordist,max_count,low");
    }
    // if space is already failed, don't post the propagator
    if(home.failed()) {
        return;
    }
    ViewArray<BitView> vx(home,x);
    // create storage for scoring
    unsigned int* table = static_cast<Space&>(home).alloc<unsigned int>((int)(pow(2,n) * pow(2,m)));
    for(int i=0; i<pow(2,n); i++) {
        for(int j=0; j<pow(2,m); j++) {
            int idx = (i*pow(2,m))+j;
            table[idx] = 0;
            //table[i][j] = 0;
        }
    }
    // create storage for allocated list
    bool* assigned = static_cast<Space&>(home).alloc<bool>((int)pow(2,n));
    for(int i=0; i<pow(2,n); i++) {
        assigned[i] = false;
    }
    // if posting failed, fail the space
    if(XorDist::post(home,vx,table,assigned,n,m,max_count) != ES_OK) {
        home.fail();
    }
}
