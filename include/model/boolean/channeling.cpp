    // channel to ints
    for(int i=0; i<std::pow(2,n); i++) {
        linear(*this, weights, x.slice(i*m, 1, m), IRT_EQ, intvals[i]);
    }
