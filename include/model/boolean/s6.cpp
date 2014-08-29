    for(unsigned int i=0; i<num_outputs; i++) {
        for(unsigned int j=i+1; j<num_outputs; j++) {
            // constraint S-6
            // If two inputs to an S-box differ in their first two bits and are identical 
            // in their last two bits, the two outputs must not be the same.
            if(((i xor j) bitand s6and) == s6sum) {
                BoolVarArray xored(*this, m, 0, 1);
                for(unsigned int bitidx=0; bitidx<m; bitidx++) {
                    rel(*this, x[(i*m)+bitidx], BOT_XOR, x[(j*m)+bitidx], xored[bitidx]);
                }
                rel(*this, sum(xored) != 0);
            }
        }
    }
