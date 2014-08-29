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
        }
    }
