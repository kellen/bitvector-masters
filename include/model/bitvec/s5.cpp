    for(unsigned int i=0; i<num_outputs; i++) {
        for(unsigned int j=i+1; j<num_outputs; j++) {
            // constraint S-5
            if((i xor j) == s5sum) {
//                REL(i,j);
                // rel(*this, parity(exor(x[i], x[j])) >= 2);
                // same as above
                BitVar z(*this, m);
                exor(*this, x[i], x[j], z);
                IntVar p(*this, 0, m);
                weight(*this, z, p);
                rel(*this, p >= 2);
            }
        }
    }
