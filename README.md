bit-vector variables for Gecode
===============================

Master's thesis + implementation.

Thesis: [abstract](http://uu.diva-portal.org/smash/record.jsf?pid=diva2%3A761927&dswid=-420), [fulltext (PDF)](http://uu.diva-portal.org/smash/get/diva2:761927/FULLTEXT01.pdf)

Build
-----

Acquire gecode source:

```
make gecode
```

Compile gecode with the bit-vector variable definition:

```
cd gecode
./configure --with-vis=../src/bitvec/bitvec.vis
make
```

When building and running, ensure ```LD_LIBRARY_PATH``` is set:

```
cd gecode/
export LD_LIBRARY_PATH=`pwd`
```

Build binaries from the base directory:

```
make build/bin/incremental
```

There is a flag for enabling bit-inversion symmetry breaking: ```-DUSE_BIT_INVERSION=true```

Enabled for shortcut build targets:

```
make benchmark
```
