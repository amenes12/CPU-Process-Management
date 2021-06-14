#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <list>

using namespace std;

template <typename T>
struct Node {
    Node* left;
    Node* right;
    T value;
    Node(T v) : value(v), left(nullptr), right(nullptr) {}
};

template <class T>
class Tree {
    Node<T>* root;
    function<bool(T&, T&)> fn;

public:
    Tree(function<bool(T&, T&)> f) : root(nullptr), fn(f) {}

    void print() {
        print(root);
    }
    void insert(T v) {
        insert(v, root);
    }

private:
    void print(Node<T>* node) {
        if (node == nullptr) return;
        print(node->left);
        cout << node->value;
        print(node->right);
    }

    void insert(T v, Node<T>*& node) {
        if (node == nullptr) {
            node = new Node<T>(v);
        } else if (fn(v, node->value)) {
            insert(v, node->left);
        } else {
            insert(v, node->right);
        }
    }
};

struct Process {
    string processName;
    short serviceTime, arrivalTime, priority;
    Process(string pN = "", short sT = 0, short aT = 0, short p = 0)
        : processName(pN), serviceTime(sT), arrivalTime(aT), priority(p) {}

    friend ostream& operator<<(ostream& out, Process& p) {
        out << "\nProceso: " << p.processName
            << "\nTiempo de servicio: " << p.serviceTime
            << "\nTiempo de llegada: " << p.arrivalTime
            << "\nPrioridad: " << p.priority << "\n";
        return out;
    }
};

class ProcessManager {
    vector<Process> processes;
    short totalserviceTime;

public:
    ProcessManager() {
        totalserviceTime = 0;
    }

    void FirstComeFirstServed() {
        auto FCFS = [](Process& a, Process& b) { return a.arrivalTime < b.arrivalTime; };
        TreeInit(FCFS);
    }

    void Prioridad() {
        auto priority = [](Process& a, Process& b) { return a.priority < b.priority; };
        TreeInit(priority);
    }

    void ShortestJobFirst() {
        auto SJF = [](Process& a, Process& b) { return a.serviceTime < b.serviceTime; };
        TreeInit(SJF);
    }

    void RoundRobin() {
        short quantum;
        cout << "\nQuantum: "; cin >> quantum;
        Init();
        vector<string> order;
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

            order.push_back(topProcess.processName);
            --topProcess.serviceTime;
            if (topProcess.serviceTime <= 0) {
                ++currentServiceTime;
                currentQuantum = 0;
                topProcess.serviceTime = 0;
                continue;
            }
            ++currentServiceTime;
            currentQuantum = (currentQuantum + 1) % quantum;
        }
        
        PrintOrderVector(order);
    }

    void ShortestRemaningTime() {
        Init();
        vector<string> order;
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
            order.push_back(top.processName);
            --top.serviceTime;
            if (top.serviceTime <= 0) {
                processQueue.pop_front();
            }
            ++currentServiceTime;
        }

        PrintOrderVector(order);
    }

private:
    void Init() {
        ProcessCreation();
        for (Process& process : processes) {
            totalserviceTime += process.serviceTime;
        }
    }

    void ProcessCreation() {
        short totalProcesess;
        do {
            cout << "\n\nNúmero de procesos a registrar: "; cin >> totalProcesess;
        } while (totalProcesess < 1);

        string pN;
        short n, sT, aT, p;
        for (short i = 0; i < totalProcesess; ++i) {
            cout << "\nNombre del proceso " << i + 1 << ": "; cin >> pN;
            cout << "Tiempo de servicio: "; cin >> sT;
            cout << "Tiempo de llegada: "; cin >> aT;
            cout << "Prioridad: "; cin >> p;
            cout << "\n-------------------------\n";
            processes.push_back(Process(pN, sT, aT, p));
        }
    }

    void TreeInit(function<bool(Process&, Process&)> fn) {
        Init();
        Tree<Process> manager(fn);
        for (Process& process : processes) {
            manager.insert(process);
        }
        cout << "\nTiempo total de servicio: " << totalserviceTime << "\n";
        cout << "\n\nOrden de ejecucición según el algoritmo de planificación escogido:\n\n";
        manager.print();
    }

    void PrintOrderVector(vector<string>& order) {
        cout << "\nTiempo total de servicio: " << totalserviceTime << "\n";
        cout << "\n\nOrden de ejecucición según el algoritmo de planificación escogido:\n\n";
        cout << "| ";
        for (string& processName : order) {
            cout << processName << " | ";
        }
        cout << "\n";
    }
};

class Controller {
    ProcessManager pM;
    short selection;

public:
    Controller() {
        selection = SelectionMenu();
    }
    void Show() {
        Show(selection);
    }

private:
    short SelectionMenu() {
        short opt;
        cout << "                       Planificación de procesos                        \n";
        cout << "(1) First Come First Served (FCFS) ";
        cout << "\n(2) Round Robin (RR)";
        cout << "\n(3) Prioridad ";
        cout << "\n(4) Shortest Job First (SJF)";
        cout << "\n(5) Shortest Time Reamining (SRT)";
        do {
            cout << "\n\nSeleccione una de las opciones mostradas: "; cin >> opt;
        } while (opt < 1 || opt > 5);
        return opt;
    }

    void Show(short s) {
        switch (s) {
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

int main() {
    Controller c;
    c.Show();
    return 0;
}
