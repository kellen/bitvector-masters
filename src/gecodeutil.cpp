#include "gecodeutil.h"

/**
 * copy the `size` assigned values of `x` to `intvals`, using `-1` for unassigned values
 */
void copyvars(const Gecode::IntVarArray& x, int* intvals, int size) {
	for(int i=0; i<size; i++) {
		if(x[i].assigned()) {
			intvals[i] = x[i].val();
		} else {
			intvals[i] = -1;
		}
	}
}

/**
 * copy the `size` assigned values of `x` to `intvals`, using `-1` for unassigned values
 */
bool copyvars(const Gecode::IntVarArray& x, unsigned int* intvals, int size) {
	for(int i=0; i<size; i++) {
		if(x[i].assigned()) {
			intvals[i] = x[i].val();
		} else {
            return false;
		}
	}
    return true;
}
