    // check s4; 2 inputs differ in 1 bit --> outputs differ in 2 bits
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            std::bitset<CHAR_BIT * sizeof(unsigned int)> bs(i xor j);
            if(bs.count() == 1) {
                std::bitset<CHAR_BIT * sizeof(unsigned int)> bs2(solution[i] xor solution[j]);
                if(!(bs2.count() >= 2)) {
                    fail = true;
                    out << "Failed s-4 for inputs " 
                        << i << "(" << std::bitset<6>(i) << ") and "
                        << j << "(" << std::bitset<6>(j) << ") xor " 
                        << std::bitset<6>(i xor j) << " count " << bs.count() << " values " 
                        << solution[i] << "(" << std::bitset<4>(solution[i]) << ") and "
                        << solution[j] << "(" << std::bitset<4>(solution[j]) << ") xor "
                        << std::bitset<4>(solution[i] xor solution[j]) << " count " << bs2.count() << std::endl;
                }
            }
        }
    }
