    unsigned int num_outputs = std::pow(2, n);

    int num_cols = std::pow(2, n-2);  // number of cols in the S-box; 2 = # bits used for rows

    unsigned int s5sum = 3 * std::pow(2, (n / 2) - 1);    // for s-5, e.g. 001100
    // the paper gives two different values for the equality (s6sum); 
    // both 3*pow(2,n-2) and 3*pow(2,n-1); 3*pow(2,n-2) is the correct version
    unsigned int s6sum = 3 * std::pow(2,n-2);          // for s-6, e.g. 110000
    unsigned int s6and = 3 * (std::pow(2,n-2) + 1);    // for s-6, e.g. 110011
