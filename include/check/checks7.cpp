    // check s7
    // pow(2,n) * pow(2,m) space for the check array
    // allocate space for row 0, which we'll ignore
    int count[(int)pow(2,n)][(int)pow(2,m)];
    for(int i=0; i<pow(2,n); i++) {
        for(int j=0; j<pow(2,m); j++) {
            count[i][j] = 0;
        }
    }
    // count for the unordered pairs, skipping comparison against self
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            count[i xor j][solution[i] xor solution[j]] += 1;
        }
    }
    // ensure the counts are <= 8
    for(int i=0; i<pow(2,n); i++) {
        for(int j=0; j<pow(2,m); j++) {
            if(count[i][j] > 8) {
                fail = true;
                out << "Failed s-7 for input diff " << std::bitset<6>(i) 
                    << "(" << i << ")" << " and output diff " << std::bitset<4>(j) 
                    << "(" << j << ") count " << count[i][j] << std::endl;
            }
        }
    }
