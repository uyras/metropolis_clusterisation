#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>
#include "MagneticSystem.h"
#include "Graph.h"

using namespace std;

namespace {

string extractState(const string& line) {
    size_t pos = line.find('\t');
    if (pos == string::npos) {
        return line;
    }
    return line.substr(pos + 1);
}

}

int main(int argc, char* argv[]){

    if (argc!=7){
        cout<<"usage: "<<endl;
        cout<<argv[0]<<" <sys.mfsys> <state file.txt> <distance> <size_x> <size_y> <size_z>"<<endl;
        return 0;
    }

    double sx = atof(argv[4]);
    double sy = atof(argv[5]);
    double sz = atof(argv[6]);
    double dist = atof(argv[3]);

    MagneticSystem sys(argv[1], {sx,sy,sz});
    ifstream states(argv[2]);

    cout<<"# min\tmax\t2nd max\t3rd max\tcount\tmean\tmean2\tmean4"<<endl;
    vector<string> inputStates;
    string state_all;
    while (std::getline(states, state_all)) {
        if (state_all.empty() || state_all[0] == '#') continue;
        inputStates.push_back(extractState(state_all));
    }

    vector<triplet> results(inputStates.size());
    Graph baseGraph(sys, dist);
    atomic_size_t nextIndex{0};
    const unsigned int workerCount = std::max(1u, std::thread::hardware_concurrency());
    vector<thread> workers;
    workers.reserve(workerCount);

    for (unsigned int workerId = 0; workerId < workerCount; ++workerId) {
        workers.emplace_back([&]() {
            while (true) {
                const size_t index = nextIndex.fetch_add(1);
                if (index >= inputStates.size()) {
                    break;
                }

                Graph graph = baseGraph;
                graph.stateToEdges(inputStates[index]);
                results[index] = graph.findClusterStats();
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    cout<<std::setprecision(10)<<std::scientific;
    for (const triplet& res : results) {
        cout<<res.min<<"\t"
            <<res.max<<"\t";
        if (res.count>1) cout<<res.max2;
        cout<<"\t";
        if (res.count>2) cout<<res.max3;
        cout<<"\t";
        cout<<res.max3<<"\t"
            <<res.count<<"\t"
            <<res.mean<<"\t"
            <<res.mean2<<"\t"
            <<res.mean4<<endl;
    }

    
    
    // cout<<sys.N()<<"\t"<<state.size()<<endl;
    // cout<<state<<endl;
    // cout<<"=========="<<endl;
    // cout<<"min="<<res.min<<", max="<<res.max<<", mean="<<res.mean<<", count="<<res.count<<endl;
    // G.printConnectedComponents();

    return 0;
}
