#include <iostream>
#include <climits>
#include <string>
#include <cmath>
#include <bitset>
#include "util.h"

int main(int argc, char* argv[]) {

    /*
    // array given in the paper
	int solution[64] = {
		0, 3, 3, 0, 5, 6, 6, 5, 9, 10, 10, 9, 15, 12, 12, 15,
		7, 4, 4, 7, 14, 13, 13, 14, 2, 1, 1, 2, 8, 11, 11, 8,
		3, 9, 15, 5, 0, 15, 12, 3, 5, 12, 9, 0, 10, 6, 6, 10,
		4, 7, 8, 11, 11, 8, 7, 4, 14, 2, 2, 14, 1, 13, 13, 1};
        */
    /* possible score 6 solution */
    int solution[64] = {
        15, 15, 13, 3, 7, 12, 11, 5, 14, 9, 0, 14, 9, 10, 6, 0, 4, 1, 3, 6, 8, 7, 5, 8, 1, 2, 10, 13, 2, 4, 12, 11, 15, 9, 10, 15, 2, 7, 7, 8, 9, 12, 12, 5, 5, 0, 0, 6, 3, 14, 4, 3, 13, 2, 14, 4, 6, 1, 1, 10, 8, 11, 11, 13
    };

	int n = 6;
	int m = 4;

    int num_scores = numscores(n,m);
    int scores[num_scores];
    calcscores(solution, scores, n, m);
    printscores(scores, n, m, std::cerr);

    std::cerr << "CHAR_BIT: " << CHAR_BIT << " sizeof(unsigned int): " << sizeof(unsigned int) << " total: " << (CHAR_BIT * sizeof(unsigned int)) << std::endl;
}
