    unsigned int s4num_outputs = std::pow(2, n);
    for(unsigned int i=0; i<s4num_outputs; i++) {
        for(unsigned int j=i+1; j<s4num_outputs; j++) {
            // constraint S-4; 
            //std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(i xor j);
            //if(bs.count() == 1) {
            if(__builtin_popcount(i xor j) == 1U) {
                //rel(*this, weight(exor(x[i], x[j])) >= 2);
                // result of the xor; m bits
                BitVar z(*this, m);
                exor(*this, x[i], x[j], z); // xi xor xj = z
                //REL(i,j);
                // result of the hamming weight; max m
                IntVar p(*this, 0, m);
                weight(*this, z, p);
                rel(*this, p >= 2);
            }
        }
    }
