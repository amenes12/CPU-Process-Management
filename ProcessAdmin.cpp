#include <iostream>
#include <functional>
#include <string>
#include <vector>

using namespace std;
template <typename T>

struct Node{
    Node* left;
    Node* right;
    T value;
    Node(T v): value(v), left(nullptr), right(nullptr) {}
};
template <class T>
class Tree{
    Node<T>* root;
    function<bool(T, T)> fn;
public:
    Tree(function<bool(T, T)> f): root(nullptr), fn(f){}
    void print(){ 
        print(root); 
    }
    void insert(T v){ 
        insert(v, root); 
    }
private:
    void print(Node<T>* n){
        if (n == nullptr) return;
        print(n->left);
        cout << n->value << " ";
        print(n->right);
    }
    void insert(T v, Node<T>*& n) {
        if (n == nullptr){
            n = new Node<T>(v);
        } else if (fn(v, n->value)){
            insert(v, n->left);
        } else {
            insert(v, n->right);
        }
    }   
};
class Process{
    string processName;
    unsigned short serviceTime, arrivalTime, priority;
public:
    Process(string pN = "", unsigned short sT = 0, unsigned short aT = 0, unsigned short p = 0) : processName(pN), serviceTime(sT), arrivalTime(aT), priority(p){} 
    unsigned short GetserviceTime(){ return serviceTime;}
    unsigned short GetArrivalTime(){ return arrivalTime;}
    unsigned short GetPriority(){ return priority;}
    string GetName(){ return processName;}
    friend ostream& operator << (ostream& cout, Process& p){
        cout << "\nProceso: " << p.processName 
             << "\nTiempo de servicio: " << p.serviceTime 
             << "\nTiempo de llegada: " << p.arrivalTime
             << "\nPrioridad: " << p.priority << "\n";
        return cout;
    }
};
class ProcessManager{
    Tree<Process>* manager;
    vector<Process> processes;
    unsigned short totalserviceTime;
public:
    ProcessManager(){
        totalserviceTime = 0;
    }
    ~ProcessManager(){
        delete manager;
    }
    void FirstComeFirstServed(){
        auto FCFS = [](Process a, Process b)->bool{ return a.GetArrivalTime() < b.GetArrivalTime();};
        ManagerInitialization(FCFS);
    }
    void Prioridad(){
        auto priority = [](Process a, Process b)->bool{ return a.GetPriority() < b.GetPriority();};
        ManagerInitialization(priority);
    }
    void ShortestJobFirst(){
        auto SJF = [](Process a, Process b)->bool{ return a.GetserviceTime() < b.GetserviceTime();};
        ManagerInitialization(SJF);
    }
private:
    void ProcessCreation(){
        short totalProcesess;
        do {
            cout << "\n\nNúmero de procesos a registrar: "; cin >> totalProcesess;
        } while(totalProcesess < 1);

        string pN;
        unsigned short n, sT, aT, p;
        for (short i = 0; i < totalProcesess; ++i){
            cout << "\nNombre del proceso " << i + 1 << ": "; cin >> pN;
            cout << "Tiempo de servicio: "; cin >> sT;
            cout << "Tiempo de llegada: "; cin >> aT;
            cout << "Prioridad: "; cin >> p;
            cout << "\n-------------------------\n";
            processes.push_back(Process(pN, sT, aT, p));
        }
    }
    void ManagerInitialization(function<bool(Process, Process)> f){
        ProcessCreation();
        manager = new Tree<Process>(f);
        for (short i = 0; i < processes.size(); ++i){
            totalserviceTime += processes[i].GetserviceTime();
            AddProcess(processes[i]); 
        }
        cout << "\n\nOrden según el algoritmo de planificación escogido: \n\n";
        manager->print();
    }
    void AddProcess(Process p){
        manager->insert(p);
    }
};
class Controller{
    ProcessManager pM;
    short selection;
public:
    Controller(){
        selection = SelectionMenu();
    }
    void Show(){
        Show(selection);
    }    
private:
    short SelectionMenu(){
        short opt;
        cout << "                       Planificación de procesos                        \n";
        cout << "(1) First Come First Served (FCFS) ";
        cout << "\n(2) Round Robin (RR)";
        cout << "\n(3) Prioridad ";
        cout << "\n(4) Shortest Job First (SJF)";
        cout << "\n(5) Shortest Time Reamining (SRT)";
        do{
            cout << "\n\nSeleccione una de las opciones mostradas: "; cin >> opt;
        } while(opt < 1 || opt > 5);
        return opt;
    }
    void Show(short s){
        switch(s){
            case 1: pM.FirstComeFirstServed();
            break;
            case 3: pM.Prioridad();
            break;
            case 4: pM.ShortestJobFirst();
            break;
            default: cout << "\nNot available\n";
            break;
        }
    }  
};
int main(){
    Controller c;
    c.Show();
    return 0;
}