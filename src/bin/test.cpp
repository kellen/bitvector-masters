#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/minimodel.hh>
#include <gecode/driver.hh>
#include <fstream>
#include <string>
#include <cmath>
//#include "sbox.h"
#include "sboxbase.h"
#include "test.h"

using namespace std;
using namespace Gecode;
using namespace Gecode::Search;
using namespace Driver; 

int main(int argc, char* argv[]) {
    //extern int* solution;
    int n = 6;
    int m = 4;
    Support::Timer t;

    ofstream outfile;
    std::stringstream sstm;
    sstm << "output/n" << n << "-m" << m << "-test.txt";
    outfile.open(sstm.str().c_str());
    std::ostream& out = outfile;

    int NUM_TESTS = 21;
    TestSBox::TestBase* boxes[NUM_TESTS];
    boxes[0] = new TestSBox::TestS2();
    boxes[1] = new TestSBox::TestS3();
    boxes[2] = new TestSBox::TestS4();
    boxes[3] = new TestSBox::TestS5();
    boxes[4] = new TestSBox::TestS6();
    boxes[5] = new TestSBox::TestS7();

    boxes[6] = new TestSBox::BitS2();
    boxes[7] = new TestSBox::BitS3();
    boxes[8] = new TestSBox::BitS4();
    boxes[9] = new TestSBox::BitS5();
    boxes[10] = new TestSBox::BitS6();
    boxes[11] = new TestSBox::BitS7();
    boxes[12] = new TestSBox::BitS2Alt();
    boxes[13] = new TestSBox::BitS7Global();

    boxes[14] = new TestSBox::BoolS2();
    boxes[15] = new TestSBox::BoolS2();
    boxes[16] = new TestSBox::BoolS3();
    boxes[17] = new TestSBox::BoolS4();
    boxes[18] = new TestSBox::BoolS5();
    boxes[19] = new TestSBox::BoolS6();
    boxes[20] = new TestSBox::BoolS7();

    int fail = 0;
    for(int i=0; i<NUM_TESTS; i++) {
        out << "Running test " << i << "...";
        for(int j=0; j<4; j++) {
            out << "---------------------------------------";
        } 
        out << std::endl;
        TestSBox::TestBase* s = boxes[i];
        CombinedStop::installCtrlHandler(true);
        out << "Testing a " << n << "x" << m << " sbox." << std::endl;
        s->sol(out);

        t.start();
        DFS<TestSBox::TestBase> e(s);
        Search::Statistics stat = e.statistics();
        if(TestSBox::TestBase* ex = e.next()) {
            ex->print(out);
            ex->verify(out);
            delete ex;
        } else {
            out << "Found no solution." << std::endl;
            fail = fail + 1;
        }
        out << "runtime: " << showpoint << fixed << setprecision(3) << (t.stop()/1000) << std::endl;
        out << "failure: " << stat.fail << std::endl;
    }

    int NUM_FULL = 9;
    TestSBox::FullTestBase* fulltests[NUM_FULL];
    fulltests[0] = new TestSBox::ExorTest();
    fulltests[1] = new TestSBox::EqualTest();
    fulltests[2] = new TestSBox::WeightTest();
    fulltests[3] = new TestSBox::MembershipTest();
    fulltests[4] = new TestSBox::ChannelTest();
    fulltests[5] = new TestSBox::ParityTest();
    fulltests[6] = new TestSBox::ConjunctionTest();
    fulltests[7] = new TestSBox::DisequalTest();
    fulltests[8] = new TestSBox::BoolChannelTest();

    for(int i=0; i<4; i++) {
        out << "=======================================";
    }
    out << std::endl;
    int fullfail = 0;
    for(int i=0; i<NUM_FULL; i++) {
        out << "Running full test " << (i + 1) << "...";
        for(int j=0; j<4; j++) {
            out << "---------------------------------------";
        } 
        out << std::endl;
        out << "Testing a " << n << "x" << m << " sbox." << std::endl;

        TestSBox::FullTestBase* s = fulltests[i];
        CombinedStop::installCtrlHandler(true);
        t.start();
        DFS<TestSBox::FullTestBase> e(s);
        delete s;

        int failures = 0;
        bool found = false;
        Search::Statistics stat = e.statistics();
        while(TestSBox::FullTestBase* ex = e.next()) {
            found = true;
            if(!ex->after(out)) {
                fullfail = fullfail + 1; 
            }
            failures = failures + stat.fail;
            delete ex;
        }
std:cerr << "\tpropagations: " << stat.propagate << std::endl
                  << "\tnodes:        " << stat.node << std::endl
                  << "\tfailures:     " << stat.fail << std::endl
                  ;
        if(!found) {
            out << "No result found." << std::endl;
            fullfail = fullfail + 1;
        }
        out << "aggregate search failures: " << failures << std::endl;
        out << "current full failures: " << fullfail << std::endl;
    }

    outfile.close();

    if(fail > 0) {
        std::cerr << "Tests failed: " << fail << std::endl;
    } else {
        std::cerr << "Tests passed." << std::endl;
    }
    if(fullfail > 0) {
        std::cerr << "Full tests failed: " << fullfail << std::endl;
    } else {
        std::cerr << "Full tests passed." << std::endl;
    }
    return 0;
}
