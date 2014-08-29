    // LSB 0 bit numbering
    // i.e. that the value at b[0] = lsb and at b[m] = msb
    // this makes the weighting easy
    IntArgs weights(m);
    for(int i=0; i<m; i++) {
        int val = std::pow(2,i);
        //std::cerr << "val: " << val << std::endl;
        weights[i] = val;
    }
