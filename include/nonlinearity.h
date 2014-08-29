#ifndef NONLIN_H
#define NONLIN_H

#include <gecode/int.hh>

void nonlinearity(
		Gecode::Home home,
		const int n, const int m,
		const int max_score,
		const Gecode::IntVarArgs& x
		);
#endif
