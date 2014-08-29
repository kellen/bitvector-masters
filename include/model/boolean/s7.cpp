/*
    // obs! array of vectors of intvars
    std::vector<IntVar> xorintvals[(int)pow(2,n)];

    // for all unordered pairs (i, j)
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            // for this input difference \delta I

            // find the output difference \delta O
            BoolVarArray xored(*this, m, 0, 1);
            for(unsigned int bitidx=0; bitidx<m; bitidx++) {
                rel(*this, x[(i*m)+bitidx], BOT_XOR, x[(j*m)+bitidx], xored[bitidx]);
            }

            // save \delta O in the list for \delta I
            IntVar c(*this, 0, pow(2,m));
            linear(*this, weights, xored, IRT_EQ, c);
            xorintvals[i xor j].push_back(c);
        }
    }
    // indexing from 1 since we ignore row 0
    for(unsigned int i=1; i<pow(2,n); i++) {
        // pull the vars from the vector into an array
        IntVarArray tmp(*this, xorintvals[i].size());
        for(unsigned int j=0; j<xorintvals[i].size(); j++) {
            tmp[j] = xorintvals[i][j];
        }
        // calculate the count for each value
        IntVarArray counts(*this, pow(2,m), 0, pow(2,n));
        rel(*this, counts, IRT_LQ, 8);
    }
    */

    
    // for all non-zero differences \delta I
    for(unsigned int i=1; i<pow(2,n); i++) {
        // for each of the 32 input pairs exhibiting \delta I
        IntVarArray xorintvals(*this, pow(2,n), 0, pow(2,m));
        int cur = 0;
        for(unsigned int j=0; j<pow(2,n); j++) {
            unsigned int k = i xor j;
            if(j < k) {
                //  \delta O
                BoolVarArray xored(*this, m, 0, 1);
                for(unsigned int bitidx=0; bitidx<m; bitidx++) {
                    rel(*this, x[(j*m)+bitidx], BOT_XOR, x[(k*m)+bitidx], xored[bitidx]);
                }

                IntVar c(*this, 0, pow(2,m));
                linear(*this, weights, xored, IRT_EQ, xorintvals[cur]);
                cur++;
            }
        }
        // get the counts for this \delta I
        // ensure no more than 8
        IntVarArray counts(*this, pow(2,m), 0, pow(2,n));
        count(*this, xorintvals, counts);
        rel(*this, counts, IRT_LQ, 8);
    }
