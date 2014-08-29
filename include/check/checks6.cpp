    // check s6; inputs differ in first 2 bits, same in last two --> outputs != 
    int s6sum = 3 * pow(2,n-2);          // for s-6, e.g. 110000
    int s6and = 3 * (pow(2,n-2) + 1);    // for s-6, e.g. 110011
    for(unsigned int i=0; i<pow(2,n); i++) {
        for(unsigned int j=i+1; j<pow(2,n); j++) {
            if(((i xor j) bitand s6and) == s6sum) {
                if(solution[i] == solution[j]) {
                    fail = true;
                    out << "Failed s-6 for inputs "
                        << i << "(" << std::bitset<6>(i) << ") and "
                        << j << "(" << std::bitset<6>(j) << ") xor " 
                        << std::bitset<6>(i xor j) << " and s6and " << std::bitset<6>((i xor j) bitand s6and) << " values " 
                        << solution[i] << "(" << std::bitset<4>(solution[i]) << ") and "
                        << solution[j] << "(" << std::bitset<4>(solution[j]) << ")" << std::endl;
                }
            }
        }
    }
