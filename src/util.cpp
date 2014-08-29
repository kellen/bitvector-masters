#include "util.h"
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <climits>

/**
 * Calculate parity if `v` per:
 * http://graphics.stanford.edu/~seander/bithacks.html#ParityNaive
 */
/*
   bool parity(unsigned int v) {
   bool p = false;
   while(v) {
   p = !p;
   v = v & (v - 1);
   }
   return p;
   }
 */
// FIXME not portable! assumes 32-bit unsigned ints!!! also from bit-hacks
/*
static const bool ParityTable256[256] = 
{
#   define P2(n) n, n^1, n^1, n
#   define P4(n) P2(n), P2(n^1), P2(n^1), P2(n)
#   define P6(n) P4(n), P4(n^1), P4(n^1), P4(n)
    P6(0), P6(1), P6(1), P6(0)
};
bool parity(unsigned int v) {
    v ^= v >> 16;
    v ^= v >> 8;
    return ParityTable256[v & 0xff];
}
*/
bool parity(unsigned int v) {
    return __builtin_parity(v) == 1;
}

/** 
 * print the `n*m` array `scores` to `out`, also print the minimum and 
 * maximum values and the score for the entire a,b space, ignoring
 * values of `-1` as unassigned variables.
 */
void printscores(int* scores, int n, int m, std::ostream& out) {
    int min = pow(2,n);
    int max = 0;

    out << std::endl;
    // use the more naive indexing to avoid fuckups
    int num_scores = numscores(n,m);
    for(int i=0; i<num_scores; i++) {
        if(i == 0) {
            out << "V A                    B -->" << std::endl;
            for(unsigned int b=1; b<pow(2,m); b*=2) {
                out << "\t" << b;
            }
            out << std::endl;
            for(int j=0; j<5; j++) {
                out << "--------------------";
            }
            out << std::endl;
        }
        if(i%m == 0) {
            out << std::endl << ((i/m)+1) << ":";
        }
        out << "\t" << scores[i];
        if(scores[i] != -1 && scores[i] < min) {
            min = scores[i];
        }
        if(scores[i] != -1 && scores[i] > max) {
            max = scores[i];
        }
    }
    out << std::endl;
    out << "max: " << max << " min: " << min << " score: " << std::max(std::abs(min - pow(2,n-1)), std::abs(max - pow(2,n-1))) << std::endl;
}

/**
 * print the `n` values of `intvals` to `out` in s-box format
 * e.g. x00 x02 ... x30 for a 6x4 s-box
 *      x01 x03 ... x31
 *      x32 x34 ... x62
 *      x33 x35 ... x63
 */
void printbox(int* intvals, int n, std::ostream& out) {
    out << "---------- box ----------" << std::endl;
    int num_cols = pow(2,n-2);
    for(int i=0; i<4; i++) {
        out << i << ":";
        for(int j=0; j<num_cols; j++) {
            int pos = (((i/2) * 2 * num_cols) + (i%2)) + 2*j;
            if(intvals[pos] >= 0) {
                out << "\t" << intvals[pos];
            } else {
                out << "\t" << "--";
            }
        }
        out << std::endl;
    }
}

void printbits(unsigned int* intvals, int n, int m, std::ostream& out) {
    out << "---------- bits ----------" << std::endl;
    int num_cols = pow(2,n-2);
    for(unsigned int i=0; i<4; i++) {
        if(i==0) {
            out << "\\begin{tabular}{l|";
            for(unsigned int j=0; j<num_cols; j++) {
                out << "l";
            }
            out << "}" << std::endl;
            out << "\\hline" << std::endl;
            for(unsigned int j=0; j<num_cols; j++) {
                out << "\t&\t" << std::bitset<4>(j);
            }
            out << "\\\\" << std::endl;
        }
        out << std::bitset<2>(i);
        for(unsigned int j=0; j<num_cols; j++) {
            int pos = (((i/2) * 2 * num_cols) + (i%2)) + 2*j;
            if(intvals[pos] >= 0) {
                out << "\t&\t" << std::bitset<4>(intvals[pos]);
            } else {
                out << "\t" << "--";
            }
        }
        out << "\\\\" << std::endl;
    }
}

void printarray(unsigned int* intvals, int n, std::ostream& out) {
    int size = pow(2,n);
    int cpy[size];
    for(int i=0; i<pow(2,n); i++) { 
        cpy[i] = (int)intvals[i];
    }
    printarray(cpy, n, out);
}

/**
 * print the array representation of the `n` values of `intvals` to `out`
 */
void printarray(int* intvals, int n, std::ostream& out) {
    out << "---------- array --------" << std::endl;
    out << "{";
    for(int i=0; i<pow(2,n); i++) {
        if(intvals[i] >= 0) {
            out << "\t" << intvals[i];
        } else {
            out << "\t" << "--";
        }
        if(i < (pow(2,n) - 1)) {
            out << ",";
        }
    }
    out << "};" << std::endl;
}

// (1<=a<|X|) * (m bit choices for b)
// each bit in b (1<=b<|D|, where b is a power of 2)
unsigned int numscores(int n, int m) {
    return (pow(2,n) - 1)*m;
}

void comparerow(unsigned int* orig, unsigned int* flip, int n, unsigned int offset, unsigned int a, unsigned int b, std::ostream& out) {
    std::stringstream origs;
    std::stringstream flips;
    std::stringstream diffs;
    bool op, fp;
    int o=0, f=0, d=0;
    int vo=0, vf=0, vd=0;
    out << "\t";
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        out << (offset+i) << "\t";
    }
    out << std::endl;
    origs << "o:\t";
    flips << "f:\t";
    diffs << "d:\t";
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        op = parity((offset+i) & a) == parity(((unsigned int)orig[i]) & b);
        fp = parity((offset+i) & a) == parity(((unsigned int)flip[i]) & b);
        origs << op << "\t";
        flips << fp << "\t";
        diffs << (op != fp ? "1" : "") << "\t";
        o = o + op;
        f = f + fp;
        d = d + (op != fp ? 1 : 0);
        if(op != fp) {
            vo = vo + (op == 1 ? -1 : 0);
            vf = vf + (fp == 1 ? 1 : 0);
        }
    }
    out << origs.str() << "= " << o << (o < 10 ? "\t" : "") << "\t" << vo << std::endl
        << flips.str() << "= " << f << (f < 10 ? "\t" : "") << "\t" << "+" << vf << std::endl
        << diffs.str() << "= " << d << (d < 10 ? "\t" : "") << "\t" << (vo + vf < 0 ? "" : "+") << (vo + vf) << std::endl;
}

void showscorecalc(unsigned int* solution, int n, unsigned int a, unsigned int b, std::ostream& out) {

    out << "a: " << a << " b: " << b << std::endl;
    // show only rows 1 and 4

    // row 1
    out << "r1" << std::endl;
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        out << i << "\t";
    }
    out << std::endl;
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        out << (parity(i & a) == parity(((unsigned int)solution[i]) & b)) << "\t";
    }
    out << std::endl;
    // row 2 
    out << "r2" << std::endl;
    for(unsigned int i=1; i<pow(2,n-1); i=i+2) {
        out << i << "\t";
    }
    out << std::endl;
    for(unsigned int i=1; i<pow(2,n-1); i=i+2) {
        out << (parity(i & a) == parity(((unsigned int)solution[i]) & b)) << "\t";
    }
    out << std::endl;
    // row 3
    out << "r3" << std::endl;
    for(unsigned int i=32; i<pow(2,n); i=i+2) {
        out << i << "\t";
    }
    out << std::endl;
    for(unsigned int i=32; i<pow(2,n); i=i+2) {
        out << (parity(i & a) == parity(((unsigned int)solution[i]) & b)) << "\t";
    }
    out << std::endl;
    // row 4
    out << "r4" << std::endl;
    for(unsigned int i=33; i<pow(2,n); i=i+2) {
        out << i << "\t";
    }
    out << std::endl;
    for(unsigned int i=33; i<pow(2,n); i=i+2) {
        out << (parity(i & a) == parity(((unsigned int)solution[i]) & b)) << "\t";
    }
    out << std::endl;
}

/**
 * calculate the scores for `solution` and save them in `scores`
 * `scores` must be of size `n*m`; scores are calculated for all
 * values of `solution` >= 0; 
 */
void calcscores(int* solution, int* scores, int n, int m) {
    // a,b restricted to powers of 2
    int num_scores = numscores(n,m);
    // initialization semantics? i dunno man.
    for(int i=0; i<num_scores; i++) {
        scores[i] = 0;
    }

    // for all vars
    for(unsigned int i=0; i<pow(2,n); i++) {
        if(solution[i] != -1) {
            // for 1<=a<|X|
            for(unsigned int a=1; a<pow(2,n); a++) {
                for(unsigned int bidx=0; bidx<m; bidx++) {
                    // for 1<=b<|D|, where b is a power of 2
                    unsigned int b = pow(2,bidx);
                    if(parity(i & a) == parity(((unsigned int)solution[i]) & b)) {
                        int pos = ((a-1)*m) + bidx;
                        scores[pos] = scores[pos] + 1;
                    }
                }
            }
        }
    }
}

/* TODO See if this method is necessary and if not, remove it.
 * I blame this method on c++ newbishness. The internet fails to give
 * a clear answer to my rather basic c++ type questions and it's easier
 * to write the method than to search more. 
 */
void calcscores(unsigned int* solution, int* scores, int n, int m) {
    int size = pow(2,n);
    int sol[size];
    for(int i=0; i<pow(2,n); i++) { 
        sol[i] = (int)solution[i];
    }
    calcscores(sol, scores, n, m);
}
void printbox(unsigned int* solution, int n, std::ostream& out) {
    int size = pow(2,n);
    int sol[size];
    for(int i=0; i<pow(2,n); i++) { 
        sol[i] = (int)solution[i];
    }
    printbox(sol, n, out);
}

/**
 * return the overall score for the `n*m` array `score`
 * i.e. max{a,b}(|N(a,b) - |X|/2|)
 * this will produce an invalid score for a partially-filled s-box
 */
int score(int* scores, int n, int m) {
    int min = pow(2,n);
    int max = 0;
    int num_scores = numscores(n,m);
    for(int i=0; i<num_scores; i++) {
        if(scores[i] != -1) {
            if(scores[i] < min) {
                min = scores[i];
            }
            if(scores[i] > max) {
                max = scores[i];
            }
        }
    }
    return std::max(std::abs(min - pow(2,n-1)), std::abs(max - pow(2,n-1)));
}

/**
 * knuth-style permutations
 */
void permute(unsigned int* a, int n) {
    for(int i=0; i<n; i++) {
        int j = randish_range(i, n-1);
        unsigned int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

// sort of random
int randish_range(int min, int max) {
    int range = max - min + 1;
    return rand() % range + min;
}

bool check(unsigned int* solution, int n, int m, int threshold, std::ostream& out) {
    bool fail = false;
#include "check/checks2.cpp"
#include "check/checks3.cpp"
#include "check/checks4.cpp"
#include "check/checks5.cpp"
#include "check/checks6.cpp"
#include "check/checks7.cpp"

    return fail;
}
