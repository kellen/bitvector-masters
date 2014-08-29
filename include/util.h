#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <cmath>

bool parity(unsigned int v);
void printscores(int* scores, int n, int m, std::ostream& out);
void printbox(int* intvals, int n, std::ostream& out);
void printbox(unsigned int* solution, int n, std::ostream& out);
void printbits(unsigned int* intvals, int n, int m, std::ostream& out);
unsigned int numscores(int n, int m);
void printarray(unsigned int* intvals, int n, std::ostream& out);
void printarray(int* intvals, int n, std::ostream& out);
void calcscores(unsigned int* solution, int* scores, int n, int m);
void calcscores(int* solution, int* scores, int n, int m);
int score(int* scores, int n, int m);
void permute(unsigned int* arr, int n);
int randish_range(int l, int u);
bool check(unsigned int* solution, int n, int m, int threshold, std::ostream& out);
void showscorecalc(unsigned int* solution, int n, unsigned int a, unsigned int b, std::ostream& out);
void comparerow(unsigned int* orig, unsigned int* flip, int n, unsigned int offset, unsigned int a, unsigned int b, std::ostream& out);
#endif
