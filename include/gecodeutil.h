#ifndef GECODEUTIL_H
#define GECODEUTIL_H
#include <gecode/int.hh>

void copyvars(const Gecode::IntVarArray& x, int* intvals, int size);
bool copyvars(const Gecode::IntVarArray& x, unsigned int* intvals, int size);
#endif
