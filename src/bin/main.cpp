#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/minimodel.hh>
#include <gecode/driver.hh>
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <cmath>
#include <locale>
#include "options.h"
#include "sbox.h"
#include "sboxbase.h"
#include "setsbox.h"

using namespace std;
using namespace Gecode;
using namespace Driver; 

enum ModelType {
    MODEL_SET, MODEL_BITVEC
};

int main(int argc, char* argv[]) {
    SBoxOptions::SBoxOptions opt;
    // no limit when commented out
    // 24 hours
    //opt.time(24 * 60 * 60 * 1000); // in milliseconds

    opt.model(MODEL_SET, "set", "Use set-based model.");
    opt.model(MODEL_BITVEC, "bitvec", "Use bitvector-based model.");

    opt.s2(SBoxOptions::S2_HC, "hc", "When using the bitvector model, use the global constraint H_C ('Complete heuristic')");
    opt.s2(SBoxOptions::S2_HC_INT, "hcint", "When using the bitvector model, use the global constraint H_C ('Complete heuristic') on integer variables");
    opt.s2(SBoxOptions::S2_DECOMP, "decomp", "When using the bitvector model, use a decomposed variant of the H_C ('Complete heuristic')");

    opt.s7(SBoxOptions::S7_DECOMP, "decomp", "When using the bitvector model, use a decomposed variant of the S-7 constraint");
    opt.s7(SBoxOptions::S7_GLOBAL, "global", "When using the bitvector model, use the global S-7 constraint");

    opt.branching(SBoxOptions::BRANCH_VAR_NONE, "none", "Choose first unassigned variable.");
    opt.branching(SBoxOptions::BRANCH_VAR_RND, "rnd", "Choose random variable.");
    opt.branching(SBoxOptions::BRANCH_VAR_DEGREE_MAX, "degree", "Choose variable involved in most constraints.");
    opt.branching(SBoxOptions::BRANCH_VAR_ACTIVITY_MAX, "activity", "Choose variable with most activity.");

    opt.parse(argc,argv);
    if(opt.n() % 2 != 0) {
        std::cerr << "Number of input bits must be even for constraint S-5" << std::endl;
        return 1;
    }
    if(opt.n() < 4) {
        std::cerr << "Number of input bits (n) must be >= 4 for constraint S-6" << std::endl;
        return 1;
    }

    SBoxBase::SBoxBase* s;
    if(opt.model() == MODEL_SET) {
        s = new SetSBox::SetSBox(opt);
    } else {
        s = new SBox::SBox(opt);
    }

    Search::Options so;
    so.threads = opt.threads();	// default 0
    so.c_d     = opt.c_d();		// default 8
    so.a_d     = opt.a_d();		// default 2
    so.clone   = false; // "Whether engines create a clone when being initialized." 
    so.stop    = CombinedStop::create(opt.node(), opt.fail(), opt.time(), opt.interrupt());
    so.cutoff  = createCutoff(opt);
    if (opt.interrupt()) {
        CombinedStop::installCtrlHandler(true);
    }

    ofstream outfile;
    std::stringstream sstm;

    std::time_t atime = std::time(NULL);
    char mbstr[100];
    size_t dateworked = std::strftime(mbstr, sizeof(mbstr), "%F_%H.%M.%S", std::localtime(&atime));

    sstm << "output/n" << opt.n() 
        << "-m" << opt.m() 
        << "-t" << opt.t() 
        << "-" << (opt.model() == MODEL_SET ? "set" : "bitvec")
        << "-";
    if(opt.model() == MODEL_BITVEC) {
        switch(opt.s2()) {
            case SBoxOptions::S2_DECOMP:
                sstm << "decomp";
                break;
            case SBoxOptions::S2_HC:
                sstm << "hc";
                break;
            case SBoxOptions::S2_HC_INT:
                sstm << "hcint";
                break;
        }
        switch(opt.s7()) {
            case SBoxOptions::S7_DECOMP:
                sstm << "s7decomp";
                break;
            case SBoxOptions::S7_GLOBAL:
                sstm << "s7global";
                break;
        }
        sstm << "-";
    }
    switch(opt.branching()) {
        case SBoxOptions::BRANCH_VAR_NONE:
            sstm << "varnone";
            break;
        case SBoxOptions::BRANCH_VAR_RND:
            sstm << "varrnd";
            break;
        case SBoxOptions::BRANCH_VAR_DEGREE_MAX:
            sstm << "vardegree";
            break;
        case SBoxOptions::BRANCH_VAR_ACTIVITY_MAX:
            sstm << "varactivity";
            break;
        default:
            sstm << "varnoneFB";
    }
    sstm << "-" << (dateworked ? mbstr : "nodate")
        << ".txt";

    outfile.open(sstm.str().c_str());
    std::ostream& out = outfile;

    out << "Testing a " 
        << opt.n() << "x" << opt.m() 
        << " sbox, threshold " << opt.t() 
        << ", " << (opt.model() == MODEL_SET ? "set" : "bitvec") << " implementation." 
        << std::endl;
    out << "Using ";
    switch(opt.s2()) {
        case SBoxOptions::S2_DECOMP:
            out << "decomposed";
            break;
        case SBoxOptions::S2_HC:
            out << "H_C";
            break;
        case SBoxOptions::S2_HC_INT:
            out << "integer H_C";
            break;
    }
    out << " S-2 variant." << std::endl;

    switch(opt.s7()) {
        case SBoxOptions::S7_DECOMP:
            out << "decomposed";
            break;
        case SBoxOptions::S7_GLOBAL:
            out << "global";
            break;
    }
    out << " S-7 variant." << std::endl;

    out << "Branching on ";
    switch(opt.branching()) {
        case SBoxOptions::BRANCH_VAR_NONE:
            out << "first unassigned variable (BIT_VAR_NONE)";
            break;
        case SBoxOptions::BRANCH_VAR_RND:
            out << "random variable (BIT_VAR_RND)";
            break;
        case SBoxOptions::BRANCH_VAR_DEGREE_MAX:
            out << "variable involved in most constraints (BIT_VAR_DEGREE_MAX)";
            break;
        case SBoxOptions::BRANCH_VAR_ACTIVITY_MAX:
            out << "variable with highest activity (BIT_VAR_ACTIVITY_MAX)";
            break;
        default:
            out << "first unassigned (BIT_VAR_NONE, as fallback)";
    }
    out << "." << std::endl;
    dateworked = std::strftime(mbstr, sizeof(mbstr), "%F:%T", std::localtime(&atime));
    out << "Started at " << (dateworked ? mbstr : "date-creation-failed(!)") << std::endl;

    bool found = false;
    int num_solutions = 0;
    Support::Timer t, total_t;
    total_t.start();

    t.start();
    DFS<SBoxBase::SBoxBase> e(s, so);
    out << "propagators: " << s->propagators() << std::endl;
    out << "branchers: " << s->branchers() << std::endl;
    Search::Statistics stat;
    while(SBoxBase::SBoxBase* ex = e.next()) {
        stat = e.statistics();
        num_solutions += 1 ;
        out << "solution " << num_solutions << "-----------------------------------" << std::endl;
        ex->print(out);
        delete ex;
        found = true;
        out << "runtime: " << showpoint << fixed << setprecision(3) << (t.stop()/1000) << std::endl;
        out << "failure: " << stat.fail << std::endl;
                    out << std::endl
                        << "\tsolutions:    " << num_solutions << std::endl
                        << "\tpropagations: " << stat.propagate << std::endl
                        << "\tnodes:        " << stat.node << std::endl
                        << "\tfailures:     " << stat.fail << std::endl
                        << "\trestarts:     " << stat.restart << std::endl
                        << "\tno-goods:     " << stat.nogood << std::endl
                        << "\tpeak depth:   " << stat.depth << std::endl
                        << std::endl;
        t = Support::Timer();
        t.start();
    }
    stat = e.statistics();
    if(!found) {
        out << "Found no solution." << std::endl;
    }
                    out << std::endl
                        << "\tsolutions:    " << num_solutions << std::endl
                        << "\tpropagations: " << stat.propagate << std::endl
                        << "\tnodes:        " << stat.node << std::endl
                        << "\tfailures:     " << stat.fail << std::endl
                        << "\trestarts:     " << stat.restart << std::endl
                        << "\tno-goods:     " << stat.nogood << std::endl
                        << "\tpeak depth:   " << stat.depth << std::endl
                        << std::endl;
    out << "---------------------------------------------------------------" << std::endl;
    out << "final:" << std::endl;
    out << "total solutions: " << num_solutions << std::endl;
    out << "toal runtime: " << showpoint << fixed << setprecision(3) << (total_t.stop()/1000) << std::endl;
    out << "failures since last solution: " << stat.fail << std::endl;

    dateworked = std::strftime(mbstr, sizeof(mbstr), "%F:%T", std::localtime(&atime));
    out << "Stopped at " << (dateworked ? mbstr : "date-creation-failed(!)") << std::endl;

    outfile.close();
    return 0;
}
