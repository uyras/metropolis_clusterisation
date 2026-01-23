
#include <vector>
#include <list>
#include <queue>
#include <string>
#include "MagneticSystem.h"
#include "misc.h"

using namespace std;

struct I_E{
    size_t i;
    double E;
};

class Graph {
private:
    int V; // Количество вершин
    vector<list<size_t>> adj; // Список смежности
    vector<list<I_E>> neigh; // Список соседей
    const MagneticSystem &sys;
    
public:
    Graph(const MagneticSystem &sys, double maxDistance) : 
        sys(sys) 
    { 
        V = sys.N(); 
        caluclateNeigh(maxDistance); 
    }

    void caluclateNeigh(double maxDistance){
        neigh.resize(V);
        for (size_t i = 0; i < V-1; i++){
            for (size_t j = i+1; j < V; j++){
                if (length(distance_pbc(sys.parts[i].p, sys.parts[j].p, sys.size)) <= maxDistance){
                    double E = hamiltonianDipolarPBC(&(sys.parts[i]), &(sys.parts[j]), sys.size);
                    neigh[i].push_back({j,E});
                    neigh[j].push_back({i,E});
                }
            }
        }
    }

    // разбираем строку состояния и делаем из ней связи графа
    void stateToEdges(string state){
        if (state.size() != V){
            cerr<<"Error! len(state) != num of spins"<<endl;
        }

        adj.clear();
        adj.resize(V);

        for (size_t i = 0; i < V; i++){
            signed char a = (state[i]=='0')?1:-1;
            for (auto neighbor : neigh[i]) {
                signed char b = (state[neighbor.i]=='0')?1:-1;

                if (neighbor.E * a * b < 0){
                    adj[i].push_back(neighbor.i);
                }
            }
        }
    }
    
    
    // Обход в ширину (BFS) для нахождения всех вершин в компоненте связности
    void BFS(int start, vector<bool>& visited, vector<int>& component) {
        queue<int> q;
        q.push(start);
        visited[start] = true;
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            component.push_back(current);
            
            for (int neighbor : adj[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }


    // Подсчет компонент связности и их размеров
    void printConnectedComponents() {
        vector<bool> visited(V, false);
        vector<vector<int>> components;
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                vector<int> component;
                BFS(i, visited, component);
                components.push_back(component);
            }
        }
        
        // Вывод результатов
        cout << "Всего несвязанных сегментов (компонент связности): " << components.size() << endl;
        cout << "==========================================" << endl;
        
        for (int i = 0; i < components.size(); i++) {
            cout << "Сегмент " << i + 1 << ": " << components[i].size() << " узлов" << endl;
            cout << "Узлы: ";
            for (int node : components[i]) {
                cout << node << " ";
            }
            cout << endl;
            cout << "------------------------------------------" << endl;
        }
    }
    
    // Подсчет компонент связности и их размеров
    triplet findClusterStats() {
        vector<bool> visited(V, false);

        triplet ret = {0};
        unsigned long long mean=0;
        unsigned long long mean2=0;
        unsigned long long mean4=0;
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                vector<int> component;
                BFS(i, visited, component);
                ret.count++;
                mean += component.size();
                mean2 += component.size()*component.size();
                mean4 += component.size()*component.size()*component.size()*component.size();
                if (i==0){
                    ret.max = ret.max2 = ret.max3 = ret.min = component.size();
                } else {
                    if (ret.max<component.size()) {
                        ret.max3 = ret.max2;
                        ret.max2 = ret.max;
                        ret.max = component.size();
                    }
                    if (ret.min>component.size()) ret.min = component.size();
                }
            }
        }
        ret.mean = mean / double(ret.count);
        ret.mean2 = mean2 / double(ret.count);
        ret.mean4 = mean4 / double(ret.count);
        return ret;
    }

    void printNeigh(){
        for (size_t i = 0; i < V; i++){
            cout<<i<<"\t[";
            for (auto neighbor : neigh[i]) {
                cout<<neighbor.i<<"_"<<neighbor.E<<", ";
            }
            cout<<"]"<<endl;
        }
    }

    void printAdj(){
        for (size_t i = 0; i < V; i++){
            cout<<i<<"\t[";
            if (adj.size())
                for (auto a : adj[i]) {
                    cout<<a<<", ";
                }
            cout<<"]"<<endl;
        }
    }
};