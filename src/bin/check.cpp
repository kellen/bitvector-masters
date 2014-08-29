#include <iostream>
#include <climits>
#include <string>
#include <cmath>
#include <bitset>
#include "util.h"

int main(int argc, char* argv[]) {
    std::cerr << "CHAR_BIT: " << CHAR_BIT << " sizeof(unsigned int): " << sizeof(unsigned int) << " total: " << (CHAR_BIT * sizeof(unsigned int)) << std::endl;
    unsigned int solution[64] = {
        /* S-1 ****/
        // table S1 for DES, as published in biham, seems to be score 14 
        //14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1, 3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8, 4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7, 15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13
        // flip x
        //15, 4, 12, 1, 8, 14, 2, 8, 4, 13, 9, 6, 1, 2, 7, 11, 5, 15, 11, 12, 3, 9, 14, 7, 10, 3, 0, 10, 6, 5, 13, 0, 0, 14, 15, 4, 7, 13, 4, 1, 14, 2, 2, 15, 13, 11, 1, 8, 10, 3, 6, 10, 12, 6, 11, 12, 9, 5, 5, 9, 3, 0, 8, 7
        // flip y
        //7, 8, 0, 3, 9, 5, 5, 9, 12, 11, 6, 12, 10, 6, 3, 10, 8, 1, 11, 13, 15, 2, 2, 14, 1, 4, 13, 7, 4, 15, 14, 0, 0, 13, 5, 6, 10, 0, 3, 10, 7, 14, 9, 3, 12, 11, 15, 5, 11, 7, 2, 1, 6, 9, 13, 4, 8, 2, 14, 8, 1, 12, 4, 15
        // rotate 180
        //13, 0, 6, 5, 0, 10, 10, 3, 14, 7, 3, 9, 11, 12, 5, 15, 7, 11, 1, 2, 9, 6, 4, 13, 2, 8, 8, 14, 12, 1, 15, 4, 8, 7, 3, 0, 5, 9, 9, 5, 11, 12, 12, 6, 6, 10, 10, 3, 1, 8, 13, 11, 2, 15, 14, 2, 4, 1, 7, 13, 15, 4, 0, 14
        // exchange the 1st and 2nd, 3rd and 4th rows
        //0, 14, 15, 4, 7, 13, 4, 1, 14, 2, 2, 15, 13, 11, 1, 8, 10, 3, 6, 10, 12, 6, 11, 12, 9, 5, 5, 9, 3, 0, 8, 7, 15, 4, 12, 1, 8, 14, 2, 8, 4, 13, 9, 6, 1, 2, 7, 11, 5, 15, 11, 12, 3, 9, 14, 7, 10, 3, 0, 10, 6, 5, 13, 0
        // exchange only the 1st and 2nd, fails for s-4
        //0, 14, 15, 4, 7, 13, 4, 1, 14, 2, 2, 15, 13, 11, 1, 8, 10, 3, 6, 10, 12, 6, 11, 12, 9, 5, 5, 9, 3, 0, 8, 7, 4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7, 15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13
        // exchange only the 3rd and 4th: fails for s-4
        //14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1, 3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8, 15, 4, 12, 1, 8, 14, 2, 8, 4, 13, 9, 6, 1, 2, 7, 11, 5, 15, 11, 12, 3, 9, 14, 7, 10, 3, 0, 10, 6, 5, 13, 0

        // S2, score 14
        //15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
        // S3, score 14
        //10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10, 1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1, 13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7, 11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12
        // S4, score 10
        //7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3, 1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9, 10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8, 15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14
        // S5, score 14
        //2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1, 8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6, 4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13, 15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3
        // S6, score 12
        //12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5, 0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8, 9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10, 7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13
        // S7, score 18
        //4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10, 3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6, 1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7, 10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12
        // S8, score 12
        //13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4, 10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2, 7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13, 0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11


        /* published score-8 solution */
        //0,	3,	3,	0,	5,	6,	6,	5,	9,	10,	10,	9,	15,	12,	12,	15,	7,	4,	4,	7,	14,	13,	13,	14,	2,	1,	1,	2,	8,	11,	11,	8,	3,	9,	15,	5,	0,	15,	12,	3,	5,	12,	9,	0,	10,	6,	6,	10,	4,	7,	8,	11,	11,	8,	7,	4,	14,	2,	2,	14,	1,	13,	13,	1
        /* possible score 6 solution, fails s-4, updated s-7 */
        //15,	15,	13,	3,	7,	12,	11,	5,	14,	9,	0,	14,	9,	10,	6,	0,	4,	1,	3,	6,	8,	7,	5,	8,	1,	2,	10,	13,	2,	4,	12,	11,	15,	9,	10,	15,	2,	7,	7,	8,	9,	12,	12,	5,	5,	0,	0,	6,	3,	14,	4,	3,	13,	2,	14,	4,	6,	1,	1,	10,	8,	11,	11,	13
        /* possible score 0 solution by Jean-Noel, fails updated s-7 */
        //4,	7,	1,	14,	7,	11,	8,	13,	14,	4,	13,	1,	11,	2,	2,	8,	10,	0,	15,	3,	9,	5,	5,	6,	3,	9,	6,	10,	0,	12,	12,	15,	11,	8,	14,	1,	8,	4,	7,	2,	1,	11,	2,	14,	4,	13,	13,	7,	5,	15,	0,	12,	6,	10,	10,	9,	12,	6,	9,	5,	15,	3,	3,	0

        // s1
        //14,	0,	4,	15,	13,	7,	1,	4,	2,	14,	15,	2,	11,	13,	8,	1,	3,	10,	10,	6,	6,	12,	12,	11,	5,	9,	9,	5,	0,	3,	7,	8,	4,	15,	1,	12,	14,	8,	8,	2,	13,	4,	6,	9,	2,	1,	11,	7,	15,	5,	12,	11,	9,	3,	7,	14,	3,	10,	10,	0,	5,	6,	0,	13
        // new, score 14, passes.
		//14,	0,	4,	15,	13,	7,	1,	4,	2,	14,	8,	2,	11,	13,	7,	8,	0,	12,	10,	9,	6,	1,	15,	10,	12,	11,	3,	5,	5,	6,	9,	3,	8,	14,	2,	8,	4,	2,	13,	11,	7,	9,	11,	4,	1,	7,	14,	1,	15,	5,	5,	6,	9,	15,	10,	0,	0,	12,	12,	3,	6,	10,	3,	13
        // TEST, fails s-7
		//14,	0,	4,	15,	13,	7,	1,	4,	2,	14,	8,	2,	11,	13,	7,	8,	0,	12,	10,	9,	6,	1,	15,	10,	12,	11,	3,	5,	5,	6,	9,	3,	8,	14,	2,	8,	4,	2,	14,	1,	1,	13,	13,	11,	7,	4,	11,	7,	6,	9,	12,	5,	9,	15,	3,	12,	15,	6,	10,	0,	0,	3,	5,	10
        // BITVEC:
        // new, score 12, passes
	    //8,	2,	1,	8,	4,	14,	2,	4,	3,	9,	12,	3,	14,	7,	11,	13,	6,	12,	13,	1,	9,	0,	7,	10,	15,	5,	10,	15,	5,	11,	0,	6,	2,	8,	8,	4,	7,	1,	11,	7,	13,	14,	6,	13,	4,2,	1,	11,	15,	5,	3,	10,	12,	15,	0,	9,	10,	3,	5,	6,	9,	12,	14,	0
        // new, score 12, passes
	    //8,	2,	1,	8,	4,	14,	2,	4,	3,	9,	12,	3,	14,	7,	11,	13, 6,	12,	13,	1,	9,	0,	7,	10,	15,	5,	10,	15,	5,	11,	0,	6,	2,	8,	4,	7,	8,	1,	11,	2,	13,	14,	1,	4,	7,	13,	14,	11,	5,	6,	3,	10,	15,	12,	0,	15,	10,	3,	6,	9,	12,	0,	9,	5

        // something found by BOOL
	    //15,	3,	1,	13,	8,	4,	14,	7,	6,	15,	11,	2,	3,	8,	4,	14,	9,	12,	7,	0,	2,	1,	13,	10,	12,	6,	0,	9,	5,	11,	10,	5,	0,	13,	14,	8,	7,	10,	11,	1,	10,	3,	4,	15,	13,	4,	1,	2,	5,	11,	8,	5,	12,	7,	6,	12,	9,	0,	3,	6,	2,	14,	15,	9
        // 15, 3,  1,  13, 8,  4,  14, 7,  6,  15, 11, 2,  3,  8,  4,  14, 9,  12, 7,  0,  2,  1,  13, 10, 12, 6,  0,  9,  5,  11, 10, 5,  4,  13, 14, 8,  1,  10, 11, 1,  10, 3,  5,  15, 13, 4,  8,  2,  7,  11, 0,  6,  12, 7,  6,  12, 9,  0,  3,  5,  2,  14, 15, 9

/*wtf*/ //15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 5, 15, 13, 4, 1, 2, 4, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
/*s2*/  15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9
        //                                                                                                                                             ^                   ^                      

// s-4 supposedly passes...?
/*s2*/  //15, 3, 1, 13, 8,  4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6,  0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11,  1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9,  0, 3, 5, 2, 14, 15, 9
/*wtf*/ //15, 3, 1, 13, 8, 15, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 15, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 11, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 15, 3, 5, 2, 14, 15, 9
        //               ^^                                                                      ^^                                            ^^                                                           ^^



    };

    bool fail = false;
    int n = 6;
    int m = 4;
    std::ostream& out = std::cerr;
    int threshold = 8;

    #include "check/checks2.cpp"
    if(s <= threshold) {
        printscores(scores, n, m, out);
    } 
    /*
    // check s2
    int num_scores = numscores(n,m);
    int scores[num_scores];
    calcscores(solution, scores, n, m);
    int s = score(scores, n, m);
    if(s <= 8) {
        printscores(scores, n, m, out);
    } else {
        out << "score: " << s << std::endl;
    }
    */


    #include "check/checks3.cpp"
    /*
    // check s3; distinct per-row
    int num_cols = pow(2, n-2);
    for(int i=0; i<4; i++) {
        int start_pos = ((i/2) * 2 * num_cols) + (i%2);
        for(int j=0; j<num_cols; j++) {
            int cur = solution[start_pos + (2*j)];
            for(int k=j+1; k<num_cols; k++) {
                if(cur == solution[start_pos + (2*k)]) {
                    fail = true;
                    out << "Failed s-3 for row " << i << ", index " 
                        << (start_pos + (2*j)) << " and " << (start_pos + (2*k)) 
                        << " value " << cur << std::endl;
                }
            }
        }
    }
    */

    #include "check/checks4.cpp"
    /*
    // check s4; 2 inputs differ in 1 bit --> outputs differ in 2 bits
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(i xor j);
            if(bs.count() == 1) {
                std::bitset<CHAR_BIT * sizeof(unsigned int)> bs2(solution[i] xor solution[j]);
                if(!(bs2.count() >= 2)) {
                    fail = true;
                    out << "Failed s-4 for inputs " 
                        << i << "(" << std::bitset<6>(i) << ") and "
                        << j << "(" << std::bitset<6>(j) << ") xor " 
                        << std::bitset<6>(i xor j) << " count " << bs.count() << " values " 
                        << solution[i] << "(" << std::bitset<4>(solution[i]) << ") and "
                        << solution[j] << "(" << std::bitset<4>(solution[j]) << ") xor "
                        << std::bitset<4>(solution[i] xor solution[j]) << " count " << bs2.count() << std::endl;
                }
            }
        }
    }
    */

    #include "check/checks5.cpp"
    /*
    // check s5; inputs differ in 2 middle bits --> outputs differ in 2 bits
    int s5sum = 3 * pow(2, (n / 2) - 1);    // for s-5, e.g. 001100
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            if((i xor j) == s5sum) {
                std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(solution[i] xor solution[j]);
                if(!(bs.count() >= 2)) {
                    fail = true;
                    out << "Failed s-5 for inputs "
                        << i << "(" << std::bitset<6>(i) << ") and "
                        << j << "(" << std::bitset<6>(j) << ") values " 
                        << solution[i] << "(" << std::bitset<4>(solution[i]) << ") and "
                        << solution[j] << "(" << std::bitset<4>(solution[j]) << ") xor "
                        << std::bitset<4>(solution[i] xor solution[j]) << " count " << bs.count() << std::endl;
                }
            }
        }
    }
    */

    #include "check/checks6.cpp"
    /*
    // check s6; inputs differ in first 2 bits, same in last two --> outputs != 
    int s6sum = 3 * pow(2,n-2);          // for s-6, e.g. 110000
    int s6and = 3 * (pow(2,n-2) + 1);    // for s-6, e.g. 110011
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            if(((i xor j) bitand s6and) == s6sum) {
                if(solution[i] == solution[j]) {
                    fail = true;
                    out << "Failed s-6 for inputs "
                        << i << "(" << std::bitset<6>(i) << ") and "
                        << j << "(" << std::bitset<6>(j) << ") xor " 
                        << std::bitset<6>(i xor j) << " and s6and " << std::bitset<6>((i xor j) bitand s6and) << " values " 
                        << solution[i] << "(" << std::bitset<4>(solution[i]) << ") and "
                        << solution[j] << "(" << std::bitset<4>(solution[j]) << ")" << std::endl;
                }
            }
        }
    }
    */

    #include "check/checks7.cpp"
    /*
    // check s7
    // pow(2,n) * pow(2,m) space for the check array
    // allocate space for row 0, which we'll ignore
    int count[(int)pow(2,n)][(int)pow(2,m)];
    for(int i=0; i<pow(2,n); i++) {
        for(int j=0; j<pow(2,m); j++) {
            count[i][j] = 0;
        }
    }
    // count for the unordered pairs, skipping comparison against self
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            count[i xor j][solution[i] xor solution[j]] += 1;
        }
    }
    // ensure the counts are <= 8
    for(int i=0; i<pow(2,n); i++) {
        for(int j=0; j<pow(2,m); j++) {
            if(count[i][j] > 8) {
                fail = true;
                out << "Failed s-7 for input diff " << std::bitset<6>(i) 
                    << "(" << i << ")" << " and output diff " << std::bitset<4>(j) 
                    << "(" << j << ") count " << count[i][j] << std::endl;
            }
        }
    }
    */

    if(fail) {
        printbox(solution, n, out);
    }
    //printbits(solution, n, m, out);
}
