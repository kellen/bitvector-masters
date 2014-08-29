#include <iostream>
#include <climits>
#include <string>
#include <cmath>
#include <bitset>
#include <iomanip>
#include <sstream>
#include "util.h"

void checkit(int n, int m, int threshold, unsigned int* solution) {
    std::ostream& out = std::cout;
    bool fail = false;
    // BLAH
#include "check/checks2.cpp"
#include "check/checks3.cpp"
#include "check/checks4.cpp"
#include "check/checks5.cpp"
#include "check/checks6.cpp"
#include "check/checks7.cpp"
    if(fail) {
        printscores(scores, n, m, out);
        printbox(solution, n, out);
    }
    //printbits(solution, n, m, out);
    //ENDBLAH
}

void showscorecals(unsigned int* toshow, int n, int m, std::ostream& out) {
    unsigned int a = 21;
    //int b = 4;
    for(unsigned int b=1; b<pow(2,m); b=b*2) {
        showscorecalc(toshow, n, a, b, out);
    }
}

void printbits(int offset, unsigned int a, int n, std::ostream& out) {
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        out << (offset + i) << "\t";
    }
    out << std::endl;
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        out << ((offset+i) & a) << "\t";
    }
    out << std::endl;
    for(unsigned int i=0; i<pow(2,n-1); i=i+2) {
        out << parity((offset+i) & a) << "\t";
    }
    out << std::endl << std::endl;
}

void printcombos(unsigned int a, int n, std::ostream& out) {
    out << std::endl << "--------------" << std::endl << "a&x, a=" << a << std::endl << std::endl;
    printbits(0, a, n, out);
    printbits(1, a, n, out);
    printbits(32, a, n, out);
    printbits(33, a, n, out);
}


void showrows(unsigned int* orig, unsigned int* flip, int n, int m, std::ostream& out) {
    int offset;

    for(unsigned int alp=0; alp<pow(2,n); alp++) {
        printcombos(alp, n, out);
    }

    unsigned int a = 20;
    for(unsigned int b=1; b<pow(2,m); b=b*2) {
        out << std::endl << "------------------" << std::endl;
        out << "a: " << a << " b: " << b << std::endl;

        out << std::endl << "r1:" << std::endl; offset = 0; comparerow(&(orig[offset]), &(flip[offset]), n, offset, a, b, out);
        out << std::endl << "r2:" << std::endl; offset = 1; comparerow(&(orig[offset]), &(flip[offset]), n, offset, a, b, out);
        out << std::endl << "r3:" << std::endl; offset = 32; comparerow(&(orig[offset]), &(flip[offset]), n, offset, a, b, out);
        out << std::endl << "r4:" << std::endl; offset = 33; comparerow(&(orig[offset]), &(flip[offset]), n, offset, a, b, out);
    }
}

// flip x
// flip y
// rotate 180
// exchange the 1st and 2nd, 3rd and 4th rows
// exchange only the 1st and 2nd, fails for s-4
// exchange only the 3rd and 4th: fails for s-4

void flipx(int n, unsigned int* solution, unsigned int* fx) {
    int src_offset, dest_offset;
    // row 1 = row 4
    dest_offset = 0;
    src_offset = (pow(2,n-2)*2)+1;
    // weird bounds because we're skipping 2
    for(int i=0; i<pow(2,n-1); i=i+2) {
        fx[dest_offset + i] = solution[src_offset + i];
    }
    // row 2 = row 3
    dest_offset = 1;
    src_offset = (pow(2,n-2)*2);
    for(int i=0; i<pow(2,n-1); i=i+2) {
        fx[dest_offset + i] = solution[src_offset + i];
    }

    // row 3 = row 2
    dest_offset = (pow(2,n-2)*2);
    src_offset = 1;
    for(int i=0; i<pow(2,n-1); i=i+2) {
        fx[dest_offset + i] = solution[src_offset + i];
    }
    // row 4 = row 1
    dest_offset = (pow(2,n-2)*2)+1;
    src_offset = 0;
    for(int i=0; i<pow(2,n-1); i=i+2) {
        fx[dest_offset + i] = solution[src_offset + i];
    }
}

void flipy(int n, unsigned int* solution, unsigned int* fy) {
    int src, dest;
    for(int i=0; i<=32; i=i+32) {
        for(int j=0; j<2; j++) {
            for(int k=0; k<8; k++) {
                src = i + j + (2*k);
                dest = i + j + (30 - (2*k));

                //std::cerr << src << "->" << dest << std::endl;
                fy[src] = solution[dest];
                fy[dest] = solution[src];
            }
        }
    }
}

void bitinvert(int n, unsigned int* solution, unsigned int* bi) {
    for(int i=0; i<64; i++) {
        bi[i] = (compl solution[i]) bitand 0b1111U;
    }
}

void rotate(int n, unsigned int* solution, unsigned int* rot) {
    for(unsigned int i=0; i<64; i++) {
        rot[i] = solution[(compl i) bitand 0b111111U];
    }
}

void fliprows(int n, unsigned int* solution, unsigned int* fy) {
    for(int i=0; i<64; i = i + 32) {
        for(int j=0; j<32; j = j + 2) {
            fy[i + j] = solution[i + j + 1];
            fy[i + j + 1] = solution[i + j];
        }
    }
}

void flipdiag(int n, unsigned int* solution, unsigned int* fd) {
    unsigned int src, dest;
    for(unsigned int i=0; i<32; i++) {
        src = i;
        dest = src ^ 0b110000;
        fd[src] = solution[dest];
        fd[dest] = solution[src];
    }
}

void flipcols(int n, unsigned int* solution, unsigned int* fc) {
    // Column 0 with 6
    // 1 with 7
    // 2 with 4
    // 3 with 5, 8 with 14, 9 with 15, 10 with 12, and 11
    //with column 13,

    unsigned int src, dest;
    for(unsigned int i=0; i<64; i=i+32) {
        for(unsigned int j=0; j<32; j=j+2) {
            src = i + j;
            dest = src xor 0b001100U;

            /*
               std::cerr << src << "->" << dest << std::endl
               << (src+1) << "->" << (dest+1) << std::endl;
             */

            fc[src] = solution[dest];
            fc[dest] = solution[src];
            fc[src + 1] = solution[dest + 1];
            fc[dest + 1] = solution[src + 1];
        }
    }
}


void printssix(unsigned int k, int n, std::ostream& out) {
    std::stringstream even;
    std::stringstream odd;
    out << std::endl << std::bitset<6>(k) << "?" << std::endl;
    unsigned int s6sum = 3 * std::pow(2,n-2);          // for s-6, e.g. 110000
    unsigned int s6and = 3 * (std::pow(2,n-2) + 1);    // for s-6, e.g. 110011
    for(unsigned int i=0; i<pow(2,n); i++) {
        if(i == pow(2,n-1)) {
            out << even.str() << std::endl << odd.str() << std::endl;
            even.str(std::string());
            even.clear();
            odd.str(std::string());
            odd.clear();
        }
        std::stringstream cur;
        if(k == i) {
            cur << "xxxxxx" << "\t";
        } else if(((i xor k) bitand s6and) == s6sum) {
            cur << std::bitset<6>(i) << "\t";
        } else {
            cur << std::setw(6) << i << "\t";
        }
        if(i%2 == 0) {
            even << cur.str();
        } else {
            odd << cur.str();
        }
    }
    out << even.str() << std::endl << odd.str() << std::endl;
}

void printsfive(unsigned int k, int n, std::ostream& out) {
    std::stringstream even;
    std::stringstream odd;
    out << std::endl << std::bitset<6>(k) << "?" << std::endl;
    unsigned int s5sum = 3 * std::pow(2, (n / 2) - 1);    // for s-5, e.g. 001100
    for(unsigned int i=0; i<pow(2,n); i++) {
        if(i == pow(2,n-1)) {
            out << even.str() << std::endl << odd.str() << std::endl;
            even.str(std::string());
            even.clear();
            odd.str(std::string());
            odd.clear();
        }
        std::stringstream cur;
        if(k == i) {
            cur << "xxxxxx" << "\t";
        } else if ((i xor k) == s5sum) {
            cur << std::bitset<6>(i) << "\t";
        } else {
            cur << std::setw(6) << i << "\t";
        }
        if(i%2 == 0) {
            even << cur.str();
        } else {
            odd << cur.str();
        }
    }
    out << even.str() << std::endl << odd.str() << std::endl;
}

void printsfour(unsigned int k, int n, std::ostream& out) {
    std::stringstream even;
    std::stringstream odd;
    out << std::endl << std::bitset<6>(k) << "?" << std::endl;
    for(unsigned int i=0; i<pow(2,n); i++) {
        if(i == pow(2,n-1)) {
            out << even.str() << std::endl << odd.str() << std::endl;
            even.str(std::string());
            even.clear();
            odd.str(std::string());
            odd.clear();
        }
        std::stringstream cur;
        if(k == i) {
            cur << "xxxxxx" << "\t";
        } else if ((std::bitset<6>(k^i)).count() == 1) {
            cur << std::bitset<6>(i) << "\t";
        } else {
            cur << std::setw(6) << i << "\t";
        }
        if(i%2 == 0) {
            even << cur.str();
        } else {
            odd << cur.str();
        }
    }
    out << even.str() << std::endl << odd.str() << std::endl;
}

void printsym(unsigned int n, unsigned int m, unsigned int* altered, int boxnum) {
        bool x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        bool y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        bool f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        bool r = (altered[0] <= altered[1]);
        bool c = (altered[0] <= altered[12]);
        bool d = (altered[0] <= altered[48]);
        bool b = (altered[0] < ((int)std::pow(2,m-1)-1));
        std::cerr << "alt " << boxnum << ": "
            << "x: " << x << ", "
            << "y: " << y << ", "
            << "f: " << f << ", "
            << "r: " << r << ", "
            << "c: " << c << ", "
            << "d: " << d << ", "
            << "b: " << b << std::endl;
}



int main(int argc, char* argv[]) {
    std::ostream& out = std::cout;
    //std::cerr << "CHAR_BIT: " << CHAR_BIT << " sizeof(unsigned int): " << sizeof(unsigned int) << " total: " << (CHAR_BIT * sizeof(unsigned int)) << std::endl;

    int n = 6;
    int m = 4;
    int threshold = 18;
    int NUM_BOXES=8;
    unsigned int NUM_ITEMS=64;//(unsigned int)pow(2,n);

    unsigned int all_boxes[512] = {
        // S1, score 14
        14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1, 3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8, 4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7, 15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13,
        // S2, score 14
        15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9,
        // S3, score 14
        10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10, 1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1, 13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7, 11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12,
        // S4, score 10
        7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3, 1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9, 10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8, 15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14,
        // S5, score 14
        2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1, 8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6, 4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13, 15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3,
        // S6, score 12
        12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5, 0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8, 9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10, 7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13,
        // S7, score 18
        4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10, 3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6, 1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7, 10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12,
        // S8, score 12
        13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4, 10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2, 7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13, 0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11
    };

    for(unsigned int boxnum=0; boxnum<NUM_BOXES; boxnum++) {
        //out << "Box " << (boxnum+1) << ":" << std::endl;
        unsigned int* cur = &(all_boxes[boxnum * NUM_ITEMS]);
        checkit(n, m, threshold, cur);
        bool x = (cur[0] <= cur[(int)std::pow(2,n-1)+1]);
        bool y = (cur[0] <= cur[(int)std::pow(2,n-1)-2]);
        bool f = (cur[0] <= cur[(int)std::pow(2,n)-1]);
        bool r = (cur[0] <= cur[1]);
        bool c = (cur[0] <= cur[12]);
        bool d = (cur[0] <= cur[48]);
        bool b = (cur[0] < ((int)std::pow(2,m-1)-1));


        std::cerr << "box " << boxnum << ": "
            << "x: " << x << ", "
            << "y: " << y << ", "
            << "f: " << f << ", "
            << "r: " << r << ", "
            << "c: " << c << ", "
            << "d: " << d << ", "
            << "b: " << b << std::endl;

        unsigned int tmp[NUM_ITEMS];
        unsigned int altered[NUM_ITEMS];
        std::copy(cur, cur+NUM_ITEMS, altered);
        if(!b) {
            bitinvert(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        r = (altered[0] <= altered[1]);
        c = (altered[0] <= altered[12]);
        d = (altered[0] <= altered[48]);
        b = (altered[0] < ((int)std::pow(2,m-1)-1));
        if(!x) {
            flipx(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        r = (altered[0] <= altered[1]);
        c = (altered[0] <= altered[12]);
        d = (altered[0] <= altered[48]);
        b = (altered[0] < ((int)std::pow(2,m-1)-1));
        if(!y) {
            flipy(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        r = (altered[0] <= altered[1]);
        c = (altered[0] <= altered[12]);
        d = (altered[0] <= altered[48]);
        b = (altered[0] < ((int)std::pow(2,m-1)-1));
        if(!f) {
            rotate(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        r = (altered[0] <= altered[1]);
        c = (altered[0] <= altered[12]);
        d = (altered[0] <= altered[48]);
        b = (altered[0] < ((int)std::pow(2,m-1)-1));
        if(!r) {
            fliprows(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        r = (altered[0] <= altered[1]);
        c = (altered[0] <= altered[12]);
        d = (altered[0] <= altered[48]);
        b = (altered[0] < ((int)std::pow(2,m-1)-1));
        if(!c) {
            flipcols(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        x = (altered[0] <= altered[(int)std::pow(2,n-1)+1]);
        y = (altered[0] <= altered[(int)std::pow(2,n-1)-2]);
        f = (altered[0] <= altered[(int)std::pow(2,n)-1]);
        r = (altered[0] <= altered[1]);
        c = (altered[0] <= altered[12]);
        d = (altered[0] <= altered[48]);
        b = (altered[0] < ((int)std::pow(2,m-1)-1));
        if(!d) {
            flipdiag(n, altered, tmp);
            std::copy(tmp, tmp+NUM_ITEMS, altered);
            printsym(n, m, altered, boxnum);
        }
        checkit(n, m, threshold, altered);
        std::cerr << std::endl << "SYMMETRIZED S-" << (boxnum+1) << std::endl;
        printbox(altered, n, std::cerr);
        printarray(altered, n, std::cerr);


        /*
           if(boxnum == 7) {
           showscorecals(cur, n, m, out);
           }
         */

        unsigned int flipped[NUM_ITEMS];
        flipx(n, cur, flipped); 
        checkit(n, m, threshold, flipped);

        /*
           if(boxnum == 7) {
           out << "S4 --------------" << std::endl;
           printsfour(0, n, out);
           printsfour(33, n, out);
           printsfour(7, n, out);
           printsfour(38, n, out);
           out << "S5 --------------" << std::endl;
           printsfive(0, n, out);
           printsfive(33, n, out);
           printsfive(7, n, out);
           printsfive(38, n, out);
           out << "S6 --------------" << std::endl;
           printssix(0, n, out);
           printssix(33, n, out);
           printssix(7, n, out);
           printssix(38, n, out);
           printssix(27, n, out);
           printssix(58, n, out);
           printssix(28, n, out);
           printssix(61, n, out);
           printssix(30, n, out);
           printssix(63, n, out);
           showscorecals(flipped, n, m, out);
           }
           if(boxnum == 7) {
           showrows(cur, flipped, n, m, out);
           }
         */

        flipy(n, cur, flipped); 
        /*
           printbox(cur, n, std::cerr);
           printbox(flipped, n, std::cerr);
         */
        checkit(n, m, threshold, flipped);
        //std::cerr << "====" << std::endl << std::endl;

        fliprows(n, cur, flipped);
        checkit(n, m, threshold, flipped);
        //printbox(cur, n, std::cerr);
        //printbox(flipped, n, std::cerr);
        flipcols(n, cur, flipped);
        checkit(n, m, threshold, flipped);
        //printbox(cur, n, std::cerr);
        //printbox(flipped, n, std::cerr);
        flipdiag(n, cur, flipped);
        checkit(n, m, threshold, flipped);
    }
}
