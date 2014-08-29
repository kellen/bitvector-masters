#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/minimodel.hh>
#include <gecode/driver.hh>
#include <fstream>
#include <string>
#include <cmath>
#include <tuple>
#include "benchmark.h"
#include "util.h"
#include <map>

using namespace std;
using namespace Gecode;
using namespace Gecode::Search;
using namespace Driver; 
using namespace Benchmark;

#define DEBUG false
#define LOG(x) if(DEBUG){std::cerr << x;}
int main(int argc, char* argv[]) {
    //extern int* solution;
    int n = 6;
    int m = 4;
    Support::Timer t;

    ofstream outfile;
    std::stringstream sstm;
    std::time_t atime = std::time(NULL);
    char mbstr[100];
    size_t dateworked = std::strftime(mbstr, sizeof(mbstr), "%F_%H.%M.%S", std::localtime(&atime));

    sstm << "output/n" << n << "-m" << m;
    sstm << "-" << (dateworked ? mbstr : "nodate");
    sstm << "-benchmark.txt";
    outfile.open(sstm.str().c_str());
    std::ostream& out = outfile;

    const unsigned int NUM_RUNS = 4;

    LOG("BLAH");
    // make 50 distinct permutations, save them for re-use
    unsigned int skip[NUM_RUNS * 64];
    for(unsigned int i=0; i<NUM_RUNS; i++) {
        LOG("WUT");
        for(unsigned int j=0; j<64; j++) {
            skip[(64*i) + j] = j;
        }
        LOG("FFFF");
        permute(&(skip[64*i]), 64);
        LOG("BBBB");
    }
    //unsigned int skip[NUM_RUNS * 64] = 
    //{ 48, 27, 26, 42, 17, 10, 3, 58, 38, 24, 0, 28, 34, 1, 57, 22, 47, 15, 52, 13, 56, 30, 43, 16, 7, 8, 20, 40, 36, 14, 29, 35, 12, 60, 9, 59, 23, 25, 54, 6, 51, 11, 21, 63, 61, 45, 62, 41, 53, 37, 2, 39, 44, 4, 32, 33, 50, 55, 18, 49, 31, 5, 19, 46 };
    //{ 42, 48, 27, 26, 17, 10, 3, 58, 38, 24, 0, 28, 34, 1, 57, 22, 47, 15, 52, 13, 56, 30, 43, 16, 7, 8, 20, 40, 36, 14, 29, 35, 12, 60, 9, 59, 23, 25, 54, 6, 51, 11, 21, 63, 61, 45, 62, 41, 53, 37, 2, 39, 44, 4, 32, 33, 50, 55, 18, 49, 31, 5, 19, 46 };

    out << "PERMUTATIONS:" << std::endl;
    for(unsigned int i=0; i<NUM_RUNS; i++) {
        out << "\t{";
        for(unsigned int j=0; j<64; j++) {
            out << "\t" << skip[(64*i) + j] << ",";
        }
        out << "\t};" << std::endl;
    }
    out << std::endl;

    // how many items to skip for each step
    std::vector<int> num_skip;
    for(int i=4; i<40; i = i + 4) {
        num_skip.push_back(i);
    }

    std::map<string, std::map<int, std::map<string, tuple<double, int, double> > > > results;
    for(int benchnum=0; ; benchnum++) {
        bool done;
        BenchmarkBase* s;
        for(unsigned int skipidx=0; skipidx<num_skip.size(); skipidx++) {
            double totaltime = 0;
            int totalnodes = 0;
            int timeoutcnt = 0;
            for(unsigned int runnum=0; runnum < NUM_RUNS; runnum++) {
                done = false;
                switch(benchnum) {
                    case 0:
                        s = new  BoolNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 1:
                        s = new BitHCS7NoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 2:
                        s = new BitDecompNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 3:
                        s = new BitHCNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 4:
                        s = new SetNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 5:
                        s = new BitHCIntNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 6:
                        s = new BitHCIntDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 7:
                        s = new BitHCDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 8:
                        s = new BitHCActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 9:
                        s = new BitHCIntActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 10:
                        s = new BitHCS7DegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 11:
                        s = new BitHCS7ActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 12:
                        s = new BitDecompDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 13:
                        s = new SetDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 14:
                        s = new SetActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 15:
                        s = new SetRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 16:
                        s = new BitHCS7RndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 17:
                        s = new BitHCRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 18:
                        s = new BitHCIntRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 19:
                        s = new BitDecompRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 20:
                        s = new BitDecompActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 21:
                        s = new  BoolDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 22:
                        s = new  BoolActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 23:
                        s = new  BoolRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    default:
                        done = true;
                }
                if(done) break; // no more tests to run

                LOG("FOOB");

                out << "Running benchmark "; 
                s->name(out);
                out << ", selection: ";
                s->varsel(out);
                out << ", values to find: " << num_skip[skipidx] << ", run number " << runnum << "... ";
                for(int j=0; j<4; j++) {
                    out << "---------------------------------------";
                } 
                out << std::endl;

                Search::Options so;
                // 2 minute timeout, hopefully
                so.stop = CombinedStop::create(0, 0, 120000, true);
                CombinedStop::installCtrlHandler(true);
                out << "Testing a " << n << "x" << m << " sbox." << std::endl;

                out.flush();

                DFS<BenchmarkBase> e(s, so);
                unsigned int n_p = s->propagators();
                unsigned int n_b = s->branchers();

                double curtime = 0;
                int num_solutions = 0;
                bool found = false;  // found A solution
                bool found_solution = false; // found THE solution
                t.start();
                BenchmarkBase* ex = e.next();
                while(ex != NULL) {
                    curtime = curtime + t.stop();
                    ex->print(out);

                    if(!ex->verify(out)) {
                        std::cerr << "Violates some constraint." << std::endl;
                    }
                    if(ex->matches_solution(out)) {
                        found_solution = true;
                    }

                    delete ex;
                    found = true;
                    num_solutions += 1;

                    t = Support::Timer();
                    t.start();
                    ex = e.next();
                }
                Search::Statistics stat = e.statistics();
if(!found_solution) {
    std::cerr << "DID NOT FOUND THE SOLUTION!" << std::endl;
}

                curtime += t.stop();
                if(e.stopped()) {
                    int r = static_cast<CombinedStop*>(so.stop)->reason(stat,so);
                    if (r & CombinedStop::SR_INT) {
                        out << "User interrupt." << std::endl;
                        done = true;
                        std::cerr << "Aborting, user interrupt." << std::endl;
                    } else if (r & CombinedStop::SR_TIME){
                        out << "Timeout." << std::endl;
                        std::cout << "Timeout." << std::endl;
                        timeoutcnt++;
                    }
                }
                if(!found) {
                    out << "Found no solution." << std::endl;
                    s->name(std::cout);
                    std::cout << " found no solution on run " << runnum << std::endl;
                }
std::cerr << "Initial" << std::endl
                  << "\tpropagators: " << n_p << std::endl
                  << "\tbranchers:   " << n_b << std::endl
                  << std::endl
                  << "Summary" << std::endl
                  << "\truntime:      " << curtime;
std::cerr << std::endl
                  << "\tsolutions:    " << num_solutions << std::endl
                  << "\tpropagations: " << stat.propagate << std::endl
                  << "\tnodes:        " << stat.node << std::endl
                  << "\tfailures:     " << stat.fail << std::endl
                  << "\trestarts:     " << stat.restart << std::endl
                  << "\tno-goods:     " << stat.nogood << std::endl
                  << "\tpeak depth:   " << stat.depth << std::endl
                  << std::endl;

                out.flush();
                totaltime = totaltime + curtime;
                totalnodes = totalnodes + stat.node;
                out << "runtime: " << showpoint << fixed << setprecision(3) << (curtime/1000) << std::endl;
                out << "failure: " << stat.fail << std::endl;
                out << "nodes: " << stat.node << std::endl;
                out << "number solutions: " << num_solutions << std::endl;
                out << "total time so far: " << showpoint << fixed << setprecision(3) << (totaltime/1000) << std::endl;
                out << "average time so far: " << showpoint << fixed << setprecision(3) << ((totaltime/(runnum+1))/1000) << std::endl;
                if(runnum + 1 < NUM_RUNS) {
                    delete s;
                }
                out.flush();
                if(done) break; // if we are aborting
            }
            if(done) break; // no more tests to run

            std::stringstream benchname;
            std::stringstream varselname;
            s->name(benchname);
            s->varsel(varselname);
            results[benchname.str()][num_skip[skipidx]][varselname.str()] = std::make_tuple(((totaltime/NUM_RUNS)/1000), timeoutcnt, totalnodes/NUM_RUNS);

            std::stringstream tot_str;
            s->name(tot_str);
            tot_str << " ";
            s->varsel(tot_str);
            tot_str << " VALUES TO FIND: " << num_skip[skipidx] << " TIMEOUTS: " << timeoutcnt << " TOTAL TIME: " << showpoint << fixed << setprecision(3) << (totaltime/1000)
                << ", AVERAGE TIME OVER " << NUM_RUNS << " RUNS: " << showpoint << fixed << setprecision(3) << ((totaltime/NUM_RUNS)/1000) << std::endl;
            out << tot_str.str();
            std::cout << tot_str.str();
            out.flush();
            delete s;
        }
        if(done) break;
    }
    LOG("UNFF");

    out << " BEGIN LATEX " << std::endl;
    out << "\\begin{tabular}{rS[round-mode=places,round-precision=2,table-format=5.2,detect-all]lS[round-mode=places,round-precision=2,table-format=5.2,detect-all]lS[round-mode=places,round-precision=2,table-format=5.2,detect-all]lS[round-mode=places,round-precision=2,table-format=5.2,detect-all]l}" << std::endl;
    out << "\\toprule" << std::endl;
    typedef std::map<string, std::map<int, std::map<string, tuple<double, int, double> > > >::iterator results_it_type;
    typedef std::map<int, std::map<string, tuple<double, int, double> > >::iterator in_it_type;
    typedef std::map<string, tuple<double, int, double> >::iterator in_in_it_type;
    for(results_it_type iterator = results.begin(); iterator != results.end(); iterator++) {
        // iterator->first = key
        out << "	&	\\multicolumn{8}{c}{" << iterator->first << "} \\\\ \\cmidrule{2-9}" << std::endl;
        std::stringstream headers;
        std::stringstream contents;
        bool headersdone = false;
        headers << "n";
        // iterator->second = value
        for(in_it_type skips = iterator->second.begin(); skips != iterator->second.end(); skips++) {
            contents << skips->first;
            for(in_in_it_type varsels = skips->second.begin(); varsels != skips->second.end(); varsels++) {
                if(!headersdone) {
                    headers << "	&	\\multicolumn{2}{c}{" << varsels->first << "}";
                }
                contents << "	&	" << showpoint << fixed << setprecision(3) << get<0>(varsels->second) << "	&	";
                // show timeout or not
                if(get<1>(varsels->second) > 0) {
                    contents << "(" << get<1>(varsels->second) << ")";
                } 
                contents << "\t&\t" << showpoint << fixed << setprecision(3) << get<2>(varsels->second);
            }
            contents << "	\\\\" << std::endl;
            headersdone = true;
        }
        headers << " \\\\";
        out << headers.str() << std::endl;
        out << "\\cmidrule{2-9}" << std::endl;
        out << contents.str();
        out << "\\\\" << std::endl;
    }
    out << "\\bottomrule" << std::endl;
    out << "\\end{tabular}" << std::endl;

    out.flush();
    outfile.close();
    return 0;
}
