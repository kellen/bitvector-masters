IntVarArray N(*this, numscores(n,m), 0, pow(2,n));
for(int a=1; a<pow(2,n); a++) {
    for(unsigned int bidx=0; bidx<m; bidx++) {
        unsigned int b = pow(2,bidx);

        BoolVarArray Nab(*this, num_outputs, 0, 1);
        for(unsigned int i=0; i<num_outputs; i++) {
            bool par = parity(i & a);
            BoolVar alphaParity(*this, par, par);

            // S(x) ^ constb = anded
            BitVar beta(*this, m, b, b);
            BitVar anded(*this, m);
            conjunction(*this, x[i], beta, anded);

            // betaParity = parity(S(x) ^ constb)
            BoolVar betaParity(*this, 0, 1);
            parity(*this, anded, betaParity);

            // Nab[i] = (alphaParity == betaParity)
            rel(*this, alphaParity, BOT_EQV, betaParity, Nab[i]);
        }
        int pos = ((a-1)*m) + bidx;
        // N(a,b) = sum(Nab)
        //sum(*this, Nab, N[pos]);
        rel(*this, N[pos] == sum(Nab));
    }
}

IntVar maxval(*this, 0, pow(2,n));
IntVar minval(*this, 0, pow(2,n));
max(*this, N, maxval);
min(*this, N, minval);

//IntVar num_assigned(*this, 0, pow(2,n));
//BoolVarArray is_assigned(*this, pow(2,n));
//assigned(*this, x, is_assigned);
//sum(*this, is_assigned, num_assigned);

//rel(*this, maxval >= num_assigned - threshold - pow(2,n-1));
//rel(*this, maxval <= pow(2,n-1) + threshold);
//rel(*this, minval >= pow(2,n-1) - threshold - (pow(2,n) - num_assigned));

rel(*this, maxval >= pow(2,n-1) - threshold);
rel(*this, maxval <= pow(2,n-1) + threshold);
rel(*this, minval >= pow(2,n-1) - threshold);
rel(*this, minval <= pow(2,n-1) + threshold);
