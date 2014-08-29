s1 = [14, 0,  4,  15, 13, 7,  1,  4,  2,  14, 15, 2,  11, 13, 8,  1,  3,  10, 10, 6,  6,  12, 12, 11, 5,  9, 9,  5,  0,  3,  7,  8,  4,  15, 1,  12, 14, 8,  8,  2,  13, 4,  6,  9,  2,  1,  11, 7,  15, 5,  12, 11, 9,  3,  7,  14, 3,  10, 10, 0,  5,  6,  0,  13]

def makeperms(s):
    r1 = s[0:32:2]
    r2 = s[1:32:2]
    r3 = s[32::2]
    r4 = s[33::2]
    print "// flip x"
    print toarray(r4, r3, r2, r1)
    print "// flip y"
    print toarray(r1[::-1], r2[::-1], r3[::-1], r4[::-1])
    print "// rotate 180"
    print toarray(r4[::-1], r3[::-1], r2[::-1], r1[::-1])
    print "// exchange the 1st and 2nd, 3rd and 4th rows"
    print toarray(r2, r1, r4, r3)
    print "// exchange only the 1st and 2nd"
    print toarray(r2, r1, r3, r4)
    print "// exchange only the 3rd and 4th"
    print toarray(r1, r2, r4, r3)

makeperms(s1)
