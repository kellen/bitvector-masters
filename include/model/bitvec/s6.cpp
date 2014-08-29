    for(unsigned int i=0; i<num_outputs; i++) {
        for(unsigned int j=i+1; j<num_outputs; j++) {
            // constraint S-6
            // If two inputs to an S-box differ in their first two bits and are identical 
            // in their last two bits, the two outputs must not be the same.
            if(((i xor j) bitand s6and) == s6sum) {
                disequal(*this, x[i], x[j]);
            }
        }
    }
