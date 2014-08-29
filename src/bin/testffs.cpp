#include <strings.h>

#include <iostream>
#include <climits>
#include <string>
#include <cmath>
#include <bitset>
#include <iomanip>
#include <sstream>

static unsigned int myffs(unsigned int x) {
    return x ?  1U << (__builtin_ffs(x) - 1) : x;
}

static unsigned int myfls(unsigned int x) {
    int ret = sizeof(unsigned int) * CHAR_BIT - 1;
    //return 1U << (x ? ret - __builtin_clz(x) : ret);
    return x ? (1U << (ret - __builtin_clz(x))) : x;
}

void p(unsigned int i, unsigned int n1) {
    int ret = sizeof(unsigned int) * CHAR_BIT - 1;
    std::cout << std::setw(2) << i << ": " << std::bitset<32>(n1) << std::endl 
        << " f: " << std::bitset<32>(myffs(n1)) << std::endl
        << " l: " << std::bitset<32>(myfls(n1)) << std::endl
        //<< "ff: " << std::bitset<32>( __builtin_ffs(n1)) << std::endl
        //<< "-f: " << std::bitset<32>( __builtin_ffs(n1) - 1) << std::endl
        //<< "  l: " << std::bitset<32>( __builtin_fls(i)) << std::endl
        ;
}

int main(int argc, char* argv[]) {
    std::cout << "uint size: " << (sizeof(unsigned int) * CHAR_BIT) << std::endl;
    unsigned int n1;
    for (unsigned int i = 0, n1 = 1; ; i++, n1 *= 42) {
        p(i, n1);
        if (n1 >= UINT_MAX / 42) break;
    }

    p(0, 0);
    p(0, UINT_MAX);
    unsigned int x = 0;
    std::cout << "x:" << x << " X? " << (x ? __builtin_ffs(x) : x) << std::endl;
    x = 10;
    std::cout << "x:" << x << " X? " << (x ? __builtin_ffs(x) : x) << std::endl;
}
