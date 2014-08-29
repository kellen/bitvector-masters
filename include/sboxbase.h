#ifndef SBOXBASE_H
#define SBOXBASE_H

#include <fstream>
// seems arbitrary which gecode header is included...
#include <gecode/driver.hh>

namespace SBoxBase {
    class SBoxBase : public Gecode::Space {
        public:
            SBoxBase(void) : Gecode::Space() {}
            SBoxBase(bool share, SBoxBase& sb) : Gecode::Space(share, sb) {}
            virtual void print(std::ostream& out) const = 0;
    };
}
#endif
