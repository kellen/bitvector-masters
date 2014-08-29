    // check s5; inputs differ in 2 middle bits --> outputs differ in 2 bits
    int s5sum = 3 * pow(2, (n / 2) - 1);    // for s-5, e.g. 001100
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            if((i xor j) == s5sum) {
                std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(solution[i] xor solution[j]);
                if(!(bs.count() >= 2)) {
                    fail = true;
                    out << "Failed s-5 for inputs "
                        << i << "(" << std::bitset<6>(i) << ") and "
                        << j << "(" << std::bitset<6>(j) << ") values " 
                        << solution[i] << "(" << std::bitset<4>(solution[i]) << ") and "
                        << solution[j] << "(" << std::bitset<4>(solution[j]) << ") xor "
                        << std::bitset<4>(solution[i] xor solution[j]) << " count " << bs.count() << std::endl;
                }
            }
        }
    }
