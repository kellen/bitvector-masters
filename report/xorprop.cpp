ExecStatus 
Exor::propagate(Space& home, 
                const ModEventDelta&) {
    BitType ux = x0.upper();
    BitType uy = x1.upper();
    BitType uz = x2.upper();
    BitType lx = x0.lower();
    BitType ly = x1.lower();
    BitType lz = x2.lower();

    BitType lowx = lx | ((~ uz) & ly) | (lz & (~ uy));
    BitType upx  = ux & (uz | uy) & (~ (ly & lz));
    GECODE_ME_CHECK(x0.bounds(home, upx, lowx));

    BitType lowy = ly | ((~ uz) & lx) | (lz & (~ ux));
    BitType upy  = uy & (uz | ux) & (~ (lx & lz));
    GECODE_ME_CHECK(x1.bounds(home, upy, lowy));

    BitType lowz = lz | ((~ ux) & ly) | (lx & (~ uy));
    BitType upz  = uz & (ux | uy) & (~ (lx & ly));
    GECODE_ME_CHECK(x2.bounds(home, upz, lowz));

    if(x0.assigned() && x1.assigned() && x2.assigned()) {
        return home.ES_SUBSUMED(*this);
    }
    return ES_FIX;
}
