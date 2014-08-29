    // weights for each element in a set
    // in order to calculate the value stored in wach set
    std::vector<int> bitpos;    // elements (m)
    std::vector<int> bitvals;   // weights (2^m)
    for(int i=0; i<m; i++) {
        bitvals.push_back((int)std::pow(2, i));
        bitpos.push_back(i+1);	// the sets are 1-based
    }
    IntArgs weight(bitvals);
    IntArgs elements(bitpos);

    // constrain intvals to be the integer representaiton of x
    for(int i=0; i<std::pow(2,n); i++) {
        weights(*this, elements, weight, x[i], intvals[i]);
    }
