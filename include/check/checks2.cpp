    // check s2
    int num_scores = numscores(n,m);
    int scores[num_scores];
    calcscores(solution, scores, n, m);
    int s = score(scores, n, m);
    if(s > threshold) {
        fail = true;
        out << "Failed for s-2 for threshold " << threshold << " score: " << s << std::endl;
    }
