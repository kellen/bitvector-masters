#include "options.h"
/** 
 * Options class for s-box generation
 */

void
SBoxOptions::SBoxOptions::help(void) {
	Options::help();
	std::cerr << "\t(unsigned int) default: " << n() << std::endl
		<< "\t\tnumber of input bits" << std::endl
		<< "\t(unsigned int) default: " << m() << std::endl
		<< "\t\tnumber of output bits" << std::endl
		<< "\t(unsigned int) default: " << t() << std::endl
		<< "\t\tthreshold for nonlinearity constraint (S2)" << std::endl;
}

SBoxOptions::SBoxOptions::SBoxOptions(void) : 
    Options("SBox"), _n(6), _m(4), _t(8), 
    _s2("-s2","S-2 criteria variants"),
    _s7("-s7","S-7 criteria variants") 
{
    add(_s2);
    add(_s7);
}

void
SBoxOptions::SBoxOptions::parse(int& argc, char* argv[]) {
	Options::parse(argc,argv);
	if (argc < 3) {
		return;
	}
	n(static_cast<unsigned int>(atoi(argv[1])));
	m(static_cast<unsigned int>(atoi(argv[2])));
	t(static_cast<unsigned int>(atoi(argv[3])));
}

void SBoxOptions::SBoxOptions::n(unsigned int n) { _n = n; }
void SBoxOptions::SBoxOptions::m(unsigned int m) { _m = m; }
void SBoxOptions::SBoxOptions::t(unsigned int t) { _t = t; }
unsigned int SBoxOptions::SBoxOptions::n(void) const { return _n; }
unsigned int SBoxOptions::SBoxOptions::m(void) const { return _m; }
unsigned int SBoxOptions::SBoxOptions::t(void) const { return _t; }

void SBoxOptions::SBoxOptions::s2(int v) { _s2.value(v); }
void SBoxOptions::SBoxOptions::s2(int v, const char* o, const char* h) { _s2.add(v,o,h); }
int SBoxOptions::SBoxOptions::s2(void) const { return _s2.value(); }

void SBoxOptions::SBoxOptions::s7(int v) { _s7.value(v); }
void SBoxOptions::SBoxOptions::s7(int v, const char* o, const char* h) { _s7.add(v,o,h); }
int SBoxOptions::SBoxOptions::s7(void) const { return _s7.value(); }
