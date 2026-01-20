#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "MagneticSystem.h"
#include "Graph.h"

using namespace std;

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

    cout<<"# min\tmax\tcount\tmean\tmean2\tmean4"<<endl;
    string state_all,state;
    cout<<std::setprecision(10)<<std::scientific;
    while(std::getline(states,state_all)) {
        if (state_all[0]=='#') continue;

        size_t pos = state_all.find('\t',0);
        if (pos == string::npos)
            state = state_all;
        else
            state = state_all.substr(pos+1);

        Graph G(sys,0.8);
        G.stateToEdges(state);
        triplet res = G.findClusterStats();

        cout<<res.min<<"\t"<<res.max<<"\t"<<res.count<<"\t"<<res.mean<<"\t"<<res.mean2<<"\t"<<res.mean4<<endl;
    }

    
    
    // cout<<sys.N()<<"\t"<<state.size()<<endl;
    // cout<<state<<endl;
    // cout<<"=========="<<endl;
    // cout<<"min="<<res.min<<", max="<<res.max<<", mean="<<res.mean<<", count="<<res.count<<endl;
    // G.printConnectedComponents();

    return 0;
}