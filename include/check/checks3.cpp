    // check s3; distinct per-row
    int num_cols = pow(2, n-2);
    for(int i=0; i<4; i++) {
        int start_pos = ((i/2) * 2 * num_cols) + (i%2);
        for(int j=0; j<num_cols; j++) {
            int cur = solution[start_pos + (2*j)];
            for(int k=j+1; k<num_cols; k++) {
                if(cur == solution[start_pos + (2*k)]) {
                    fail = true;
                    out << "Failed s-3 for row " << i << ", index " 
                        << (start_pos + (2*j)) << " and " << (start_pos + (2*k)) 
                        << " value " << cur << std::endl;
                }
            }
        }
    }
