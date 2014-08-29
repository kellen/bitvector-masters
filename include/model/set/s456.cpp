    int s5sum = 3 * std::pow(2, (n / 2) - 1);    // for s-5, e.g. 001100
    // the paper gives two different values for the equality (s6sum); 
    // both 3*pow(2,n-2) and 3*pow(2,n-1); 3*pow(2,n-2) is the correct version
    int s6sum = 3 * std::pow(2,n-2);          // for s-6, e.g. 110000
    int s6and = 3 * (std::pow(2,n-2) + 1);    // for s-6, e.g. 110011
    for(unsigned int i=0; i<num_outputs; i++) {
        for(unsigned int j=i+1; j<num_outputs; j++) {
            // constraint S-4; 
            // If two inputs to an S-box differ in exactly one bit, 
            // the outputs must differ in at least two bits.
            std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(i xor j);
            if(bs.count() == 1) {
                // xor = (p v q) ^ ~(p ^ q)
                rel(*this, cardinality((x[i] | x[j]) & -(x[i] & x[j])) >= 2);
            }

            // constraint S-5
            // If two inputs to an S-box differ in the two middle bits exactly, 
            // the outputs must differ in at least two bits.
            if((i xor j) == s5sum) {
                rel(*this, cardinality((x[i] | x[j]) & -(x[i] & x[j])) >= 2);
            }

            // constraint S-6
            // If two inputs to an S-box differ in their first two bits and are identical 
            // in their last two bits, the two outputs must not be the same.
            if(((i xor j) bitand s6and) == s6sum) {
                rel(*this, x[i], SRT_NQ, x[j]);
            }
        }
    }

