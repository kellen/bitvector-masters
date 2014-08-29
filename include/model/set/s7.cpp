/*
    // for all possible input differences
    for(unsigned int i=0; i<std::pow(2,n); i++) {
        // find the 64 ordered pairs which == this difference
        SetVarArray xored(*this, std::pow(2,n), IntSet::empty, IntSet(1,m));
        IntVarArray xorint(*this, std::pow(2,n), 0, std::pow(2,m));
        for(unsigned int j=0; j<std::pow(2,n); j++) {
            SetVar p = x[j];
            SetVar q = x[j xor i];
            // xor their values and get the integer representation of this
            rel(*this, xored[j] == ((p | q) & -(p & q)));
            weights(*this, elements, weight, xored[j], xorint[j]);
        }
        // get the counts for each integer value
        IntVarArray counts(*this, std::pow(2,m), 0, std::pow(2,n));
        count(*this, xorint, counts);
        // the restriction <= 8 for each count of the sums
        // OBS due to using ordered pairs, this value is 16
        if(i != 0) {
            rel(*this, counts, IRT_LQ, 16);
        }
    }
*/


    // for all non-zero input differences \delta I
    for(unsigned int i=1; i<pow(2,n); i++) {
        // for each of the 32 input pairs exhibiting \delta I
        IntVarArray xorintvals(*this, pow(2,n), 0, pow(2,m)); 
        int cur = 0;
        for(unsigned int j=0; j<pow(2,n); j++) {
            // lower number first
            unsigned int k = i xor j;
            if(j < k) {
                // then ok
                // \delta O
                SetVar p = x[j];
                SetVar q = x[k];
                SetVar s(*this, IntSet::empty, IntSet(1,m));
                rel(*this, s == ((p | q) & -(p & q)));
                weights(*this, elements, weight, s, xorintvals[cur]);
                cur++;
            }
        }
        // get the counts for this \delta I
        // ensure no more than 8
        IntVarArray counts(*this, pow(2,m), 0, pow(2,n));
        count(*this, xorintvals, counts);
        rel(*this, counts, IRT_LQ, 8);
    }
