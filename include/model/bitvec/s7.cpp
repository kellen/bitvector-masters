
    // for all ordered input pairs (i.e. not repeating)
    //  calculate the xor of the outputs for these inputs, put this in b
    //  channel b to an integer representation
    //  put b in a list for the given input xor

    // later...
    // for all the input xors, 
    //  fetch the list
    //  copy the items in the list to the tmp array
    //  constraint count(tmp) <= 8

/*
    //std::vector<BitVar> xorvals[(int)pow(2,n)];
    // obs! array of vectors of IntVars
    std::vector<IntVar> xorintvals[(int)pow(2,n)];
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            // this can probably be determined, but who wants to do that?
            BitVar b(*this, m);
            IntVar c(*this, 0, pow(2,n));
            //xorvals[i xor j].push_back(b);
            exor(*this, x[i], x[j], b);
            channel(*this, b, c);
            xorintvals[i xor j].push_back(c);

            // std::cerr << "xor: " << (i xor j) << " size: " << (xorintvals[i xor j].size()) << std::endl;
        }
    }
    // indexing from 1 since we ignore row 0
    for(unsigned int i=1; i<pow(2,n); i++) {
        // pull the vars from the vector into an array
        IntVarArray tmp(*this, xorintvals[i].size());
        for(unsigned int j=0; j<xorintvals[i].size(); j++) {
            tmp[j] = xorintvals[i][j];
        }
        // counts
        IntVarArray counts(*this, pow(2,m), 0, pow(2,n));
        count(*this, tmp, counts);
        rel(*this, counts, IRT_LQ, 8);
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
                BitVar b(*this, m);
                exor(*this, x[j], x[k], b);
                channel(*this, b, xorintvals[cur]);
                cur++;
            }
        }
        // get the counts for this \delta I
        // ensure no more than 8
        IntVarArray counts(*this, pow(2,m), 0, pow(2,n));
        count(*this, xorintvals, counts);
        rel(*this, counts, IRT_LQ, 8);
    }
