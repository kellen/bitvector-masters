    // symmetry breaking constraints

    // for flipping over x; 0 <= 33
    rel(*this, intvals[0] <= intvals[(int)std::pow(2,n-1)+1]);

    // for flipping over y; 0 <= 30
    rel(*this, intvals[0] <= intvals[(int)std::pow(2,n-1)-2]);

    // for 180-degree rotation; 0 <= 63
    rel(*this, intvals[0] <= intvals[(int)std::pow(2,n)-1]);

    // for exchanging rows; 0 <= 1
    rel(*this, intvals[0] <= intvals[1]);

    // for exchanging columns
    rel(*this, intvals[0] <= intvals[12]);

    // for exchanging block along the diagonal
    rel(*this, intvals[0] <= intvals[48]);

    // bit-inversion symmetry
#ifdef USE_BIT_INVERSION
    if(USE_BIT_INVERSION) {
        rel(*this, intvals[0] < ((int)std::pow(2,m-1)-1));
    }
#endif
