    // constraint S-2; nonlinearity constraint
    IntVarArgs intvalsargs(intvals);
    nonlinearity(*this, n, m, threshold, intvalsargs);
