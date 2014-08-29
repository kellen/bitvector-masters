    // constraint S-3; distinct per-row
    // 4 = # of rows; i.e. pow(2,2) where 2 = # of bits for each row
    for(int i=0; i<4; i++) {
        int start_pos = ((i/2) * 2 * num_cols) + (i%2);
        distinct(*this, intvals.slice(start_pos, 2, num_cols));
    }
