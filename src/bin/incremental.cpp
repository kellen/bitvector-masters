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
    sstm << "-inc-benchmark.txt";
    outfile.open(sstm.str().c_str());
    std::ostream& out = outfile;

    const unsigned int NUM_RUNS = 25;  // how many runs per model/skip combination
    const unsigned int MAX_TO = 3;     // how many timeouts before we abort
    const unsigned int TIMEOUT = 30;   // timeout in seconds

    out << "NUM_RUNS: " << NUM_RUNS << std::endl
        << "MAX_TIMEOUTS: " << MAX_TO << std::endl
        << "TIMEOUT TIME: " << TIMEOUT << " seconds" << std::endl;
    // make distinct permutations, save them for re-use
    out << "PERMUTATIONS:" << std::endl;
    unsigned int skip[NUM_RUNS * 64];
    for(unsigned int i=0; i<NUM_RUNS; i++) {
        for(unsigned int j=0; j<64; j++) {
            skip[(64*i) + j] = j;
        }
        permute(&(skip[64*i]), 64);
        out << "\t{";
        for(unsigned int j=0; j<64; j++) {
            out << "\t" << skip[(64*i) + j] << ",";
        }
        out << "\t};" << std::endl;
    }
    out << std::endl;
    // how many items to skip for each step
    std::vector<int> num_skip;
    for(int i=8; i<=64; i = i + 4) {
    //for(int i=41; i<=64; i = i + 1) {
        num_skip.push_back(i);
    }

    std::map<string, std::map<int, std::map<string, tuple<double, int, double> > > > results;
    std::map<string, std::vector< tuple<int, int, int, double, double, int, double> > > tabresults;
    for(int benchnum=0; ; benchnum++) {
        bool done = false;
        bool toomany = false;
        BenchmarkBase* s;
        std::stringstream benchname;
        std::stringstream varselname;
        std::stringstream shortname;
        for(unsigned int skipidx=0; skipidx<num_skip.size(); skipidx++) {
            double totaltime = 0;
            int totalnodes = 0;
            int timeoutcnt = 0;

            for(unsigned int runnum=0; !toomany && (runnum < NUM_RUNS); runnum++) {
                done = false;
#include "marks.inc"
                if(done) break; // no more tests to run

                benchname.str("");
                benchname.clear();
                varselname.str("");
                varselname.clear();
                shortname.str("");
                shortname.clear();
                s->name(benchname);
                s->varsel(varselname);
                s->shortname(shortname);

                out << "Running benchmark "; 
                s->name(out);
                out << ", selection: ";
                s->varsel(out);
                out << ", values to find: " << num_skip[skipidx] << ", run number " << runnum << "... ";
                for(int j=0; j<4; j++) {
                    out << "---------------------------------------";
                } 
                out << std::endl;
                out << "Testing a " << n << "x" << m << " sbox." << std::endl;
                out.flush();

                Search::Options so;
                so.stop = CombinedStop::create(0, 0, TIMEOUT * 1000, true);
                CombinedStop::installCtrlHandler(true);

                unsigned int n_p = s->propagators();
                unsigned int n_b = s->branchers();
                DFS<BenchmarkBase> e(s, so);

                double curtime = 0;
                int num_solutions = 0;
                bool found = false;          // found A solution
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
                    ex = NULL;
                    found = true;
                    num_solutions += 1;

                    if(found_solution) break;

                    t = Support::Timer();
                    t.start();
                    ex = e.next();
                }
                curtime += t.stop();
                Search::Statistics stat = e.statistics();

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
                        // timeouts don't count towards the totals, so do nothing with `stat`
                        if(timeoutcnt >= MAX_TO) {
                            out << "Timeouts: " << timeoutcnt << std::endl;
                            out << "Too many timeouts, aborting this model." << std::endl;
                            toomany = true;
                        }
                    }
                } else {
                    if(!e.stopped() && !found_solution) {
                        std::cerr << "DIDN'T FIND THE SOLUTION AND DIDN'T STOP; FUCKUP" << std::endl;
                        out << "DIDN'T FIND THE SOLUTION, FUCKUP OOOPS" << std::endl;
                    }
                    if(!found) {
                        out << "Found no solution." << std::endl;
                        std::cout << benchname.str();
                        std::cout << " found no solution on run " << runnum << std::endl;
                    }
                    out << "Initial" << std::endl
                        << "\tpropagators: " << n_p << std::endl
                        << "\tbranchers:   " << n_b << std::endl
                        << std::endl
                        << "Summary" << std::endl
                        << "\truntime:      " << curtime;
                    out << std::endl
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

                    out << "runtime: " << showpoint << fixed << setprecision(3) << (curtime/1000) << "s" << std::endl;
                    out << "failure: " << stat.fail << std::endl;
                    out << "nodes: " << stat.node << std::endl;
                    out << "number solutions: " << num_solutions << std::endl;
                    out << "total time so far: " << showpoint << fixed << setprecision(3) << (totaltime/1000) << std::endl;
                    out << "average time so far: " << showpoint << fixed << setprecision(3) << ((totaltime/(runnum+1))/1000) << std::endl;
                }
                if(!toomany && (runnum + 1 < NUM_RUNS)) {
                    delete s;
                    s = NULL;
                }
                out.flush();
                if(done || toomany) break; // if we are aborting
            }
            if(done) break; // no more tests to run

            if(toomany) {
                results[benchname.str()]
                    [num_skip[skipidx]]
                    [varselname.str()] = std::make_tuple(-1.0,-1,-1.0);
            } else {
                results[benchname.str()]
                    [num_skip[skipidx]]
                    [varselname.str()] = std::make_tuple(((totaltime/(NUM_RUNS-timeoutcnt))/1000), timeoutcnt, totalnodes/(NUM_RUNS-timeoutcnt));
                // n\tnumruns\tnumtimeouts\ttotaltime\taveragetime\ttotalnodes,averagenodes
                tabresults[shortname.str()].push_back(
                        std::make_tuple(num_skip[skipidx], 
                                        NUM_RUNS, 
                                        timeoutcnt, 
                                        totaltime, 
                                        ((totaltime/(NUM_RUNS-timeoutcnt))/1000), 
                                        totalnodes,
                                        totalnodes/(NUM_RUNS-timeoutcnt)
                            )
                        );
            }

            std::stringstream tot_str;
            tot_str << benchname.str() << " " << varselname.str();
            tot_str << " VALUES TO FIND: " << num_skip[skipidx] 
                    << " TIMEOUTS: " << timeoutcnt 
                    << " TOTAL TIME: " << showpoint << fixed << setprecision(3) << (totaltime/1000)
                    << ", AVERAGE TIME OVER " << (toomany ? 0 : (NUM_RUNS-timeoutcnt))
                    << " OF " << NUM_RUNS << " ATTEMPTED RUNS: " 
                    << showpoint << fixed << setprecision(3) << ((totaltime/(NUM_RUNS-timeoutcnt))/1000) << " SECONDS, "
                    << "AVERAGE NODES: " << (totalnodes/(NUM_RUNS-timeoutcnt))
                    << std::endl;
            out << tot_str.str();
            std::cout << tot_str.str();
            out.flush();
            if(s != NULL) {
                delete s;
                s = NULL;
            }
        }
        if(s != NULL) {
            delete s;
        }
        if(done) break;
    }

    out << " BEGIN LATEX " << std::endl;
    out << "\\begin{tabular}{r";
    for(int colidx=0; colidx<4; colidx++) {
       out << "S[round-mode=places,round-precision=2,table-format=2.2,detect-all]"
           << "l"
           << "S[round-mode=places,round-precision=0,table-format=5,detect-all]"
           ;
    }
    out << "}" << std::endl;
    out << "\\toprule" << std::endl;

    typedef std::map<string, std::map<int, std::map<string, tuple<double, int, double> > > >::iterator results_it_type;
    typedef std::map<int, std::map<string, tuple<double, int, double> > >::iterator in_it_type;
    typedef std::map<string, tuple<double, int, double> >::iterator in_in_it_type;
    for(results_it_type iterator = results.begin(); iterator != results.end(); iterator++) {
        // iterator->first = key
        //out << "	&	\\multicolumn{8}{c}{" << iterator->first << "} \\\\ \\cmidrule{2-9}" << std::endl;
        out << "	&	\\multicolumn{12}{c}{" << iterator->first << "} \\\\ \\cmidrule{2-13}" << std::endl;
        std::stringstream headers;
        std::stringstream contents;
        bool headersdone = false;
        // iterator->second = value
        for(in_it_type skips = iterator->second.begin(); skips != iterator->second.end(); skips++) {
            contents << skips->first;
            for(in_in_it_type varsels = skips->second.begin(); varsels != skips->second.end(); varsels++) {
                if(!headersdone) {
                    //headers << "	&	\\multicolumn{2}{c}{" << varsels->first << "}";
                    headers << "	&	\\multicolumn{3}{c}{" << varsels->first << "}";
                }
                if(get<1>(varsels->second) < MAX_TO) {
                    contents << "	&	" << showpoint << fixed << setprecision(3) << get<0>(varsels->second) << "	&	";
                    // show timeout or not
                    if(get<1>(varsels->second) > 0) {
                        contents << "(" << get<1>(varsels->second) << ")";
                    } 
                    contents << "\t&\t" << showpoint << fixed << setprecision(3) << get<2>(varsels->second);
                } else {
                    contents << "\t&\t\t&\t\t&\t";
                }
            }
            contents << "	\\\\" << std::endl;
            headersdone = true;
        }
        headers << " \\\\" << std::endl;

        for(int col=2; col<13; col=col+3) {
            headers << "\\cmidrule(l){" << col << "-" << (col+2) << "}";
        }
        headers << std::endl;

        headers << "n";
        for(int col=0; col<4; col++) {
            headers << "\t&\t\\multicolumn{1}{c}{time}\t&\t\\multicolumn{1}{c}{t/o}\t&\t\\multicolumn{1}{c}{nodes}";
        }
        headers << "\\\\";

        out << headers.str() << std::endl;
        out << contents.str();
        out << "\\\\" << std::endl;
    }
    out << "\\bottomrule" << std::endl;
    out << "\\end{tabular}" << std::endl;

    out.flush();


    out << std::endl << std::endl << " BEGIN TABLES " << std::endl << std::endl;
    typedef std::map< string, std::vector< tuple<int, int, int, double, double, int, double> > > ::iterator tabiter;
    typedef std::vector< tuple<int, int, int, double, double, int, double> > vector_type;
    //typedef tuple<int, int, int, double, double, int, double>::iterator tabvalsiter;
    
    for(tabiter iterator = tabresults.begin(); iterator != tabresults.end(); iterator++) {
        out << "\\begin{filecontents}{" << iterator->first << ".tsv}" << std::endl;
        out << "\tn\tnumruns\tnumtimeouts\ttotaltime\taveragetime\ttotalnodes\taveragenodes" << std::endl;

        std::vector< tuple<int, int, int, double, double, int, double> > theval = iterator->second;
        for(unsigned int idx=0; idx<theval.size(); idx++) {
            tuple<int, int, int, double, double, int, double> row = theval[idx];
            out << "\t" << get<0>(row)
                << "\t" << get<1>(row)
                << "\t" << get<2>(row)
                << "\t" << get<3>(row)
                << "\t" << get<4>(row)
                << "\t" << get<5>(row)
                << "\t" << get<6>(row)
                << std::endl;
        }
        out << "\\end{filecontents}" << std::endl << std::endl;
    }

    outfile.close();
    return 0;
}
