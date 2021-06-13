#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <list>

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
public:
    string processName;
    short serviceTime, arrivalTime, priority;
    Process(string pN = "", short sT = 0, short aT = 0, short p = 0) : processName(pN), serviceTime(sT), arrivalTime(aT), priority(p){} 
    short GetserviceTime(){ return serviceTime;}
    short GetArrivalTime(){ return arrivalTime;}
    short GetPriority(){ return priority;}
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
    short totalserviceTime;
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
    void RoundRobin() {
        short quantum;
        cout << "\nQuantum: "; cin >> quantum;
        ProcessCreation();
        for (Process& process : processes) {
            totalserviceTime += process.GetserviceTime();
        }

        queue<Process> processQueue;
        short currentServiceTime = 0;
        short currentQuantum = 0;
        Process topProcess;
        while (currentServiceTime < totalserviceTime) {
            for (Process& process : processes) {
                if (process.arrivalTime == currentServiceTime)
                    processQueue.push(process);
            }
            
            if (currentQuantum == 0) {
                if (topProcess.serviceTime != 0) {
                    processQueue.push(topProcess);
                }
                topProcess = processQueue.front();
                processQueue.pop();
            }

            cout << "Procesando " << topProcess.processName << "...\n";
            --topProcess.serviceTime;
            if (topProcess.serviceTime <= 0) {
                cout << "Proceso " << topProcess.processName << " finalizado.\n";
                ++currentServiceTime;
                currentQuantum = 0;
                // currentServiceTime += quantum - abs(topProcess.serviceTime);
                topProcess.serviceTime = 0;
                continue;
            }
            cout << "Tiempo restante en " << topProcess.processName << ": " << topProcess.serviceTime << "\n";
            ++currentServiceTime;
            currentQuantum = (currentQuantum + 1) % quantum;
        }
        cout << "TODOS LOS PROCESOS HAN SIDO TERMINADOS\n";
    }
    void ShortestRemaningTime() {
        ProcessCreation();
        for (Process& process : processes) {
            totalserviceTime += process.GetserviceTime();
        }

        list<Process> processQueue;
        short currentServiceTime = 0;
        while (currentServiceTime < totalserviceTime) {
            for (Process& process : processes) {
                if (process.arrivalTime == currentServiceTime)
                    processQueue.push_back(process);
            }

            if (processQueue.size() == 0) {
                ++currentServiceTime;
                continue;
            }

            if (processQueue.size() > 1) {
                processQueue.sort([](Process& a, Process& b) {
                    return a.serviceTime < b.serviceTime;
                    });
            }
            Process& top = processQueue.front();
            cout << "Procesando " << top.processName << "...\n";
            --top.serviceTime;
            cout << "Tiempo restante en " << top.processName << ": " << top.serviceTime << "\n";
            if (top.serviceTime <= 0) {
                processQueue.pop_front();
                cout << "Proceso " << top.processName << " finalizado.\n";
            }
            ++currentServiceTime;
        }
        cout << "TODOS LOS PROCESOS HAN SIDO TERMINADOS\n";
    }
private:
    void ProcessCreation(){
        short totalProcesess;
        do {
            cout << "\n\nNúmero de procesos a registrar: "; cin >> totalProcesess;
        } while(totalProcesess < 1);

        string pN;
        short n, sT, aT, p;
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
            case 2: pM.RoundRobin();
            break;
            case 3: pM.Prioridad();
            break;
            case 4: pM.ShortestJobFirst();
            break;
            case 5: pM.ShortestRemaningTime();
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
