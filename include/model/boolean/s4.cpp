    for(unsigned int i=0; i<num_outputs; i++) {
        for(unsigned int j=i+1; j<num_outputs; j++) {
            // constraint S-4; 
            // If two inputs to an S-box differ in exactly one bit, 
            // the outputs must differ in at least two bits.
            std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(i xor j);
            if(bs.count() == 1) {
                BoolVarArray xored(*this, m, 0, 1);
                for(unsigned int bitidx=0; bitidx<m; bitidx++) {
                    rel(*this, x[(i*m)+bitidx], BOT_XOR, x[(j*m)+bitidx], xored[bitidx]);
                }
                rel(*this, sum(xored) >= 2);
            }
        }
    }
