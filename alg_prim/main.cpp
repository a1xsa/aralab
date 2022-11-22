#include <iostream>
#include <random>
#include <ctime>
#include <list>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

const int infinity = 101;
class DHeap
{
public:
    int n;
    int d;
    int* name; // название вершин
    int* key; // вес вершины
    int* index; // индексы 
public:
    DHeap(int _n, int _d, int* _name, int* _key, int* _index) :n(_n), d(_d), name(_name), key(_key), index(_index)
    {
        get_turn();
    }
    void get_turn()
    {
        for (int i = n - 1; i >= 0; i--) {
            diving(i);
        }
    }
    void diving(int i) // O (d logd n)
    {
        int key0 = key[i], name0 = name[i];
        int child = minchild(i);
        while (child != -1 && key0 > key[child]) {
            key[i] = key[child];
            name[i] = name[child];
            index[name[i]] = i;
            i = child;
            child = minchild(i);
        }
        key[i] = key0;
        name[i] = name0;
        index[name[i]] = i;
    }
    void emersion(int i) //O (logd n)
    {
        int key0 = key[i], name0 = name[i], father = parent(i);
        while (father != -1 && key0 < key[father]) {
            key[i] = key[father];
            name[i] = name[father];
            index[name[i]] = i;
            i = father;
            father = parent(i);
        }
        key[i] = key0;
        name[i] = name0;
        index[name[i]] = i;
    }

    void get_min(int& name0, int& key0) // O(d log d n)
    {
        name0 = name[0], key0 = key[0];
        name[0] = name[n - 1];
        key[0] = key[n - 1];
        name[n - 1] = name0;
        key[n - 1] = 0;
        n--;
        if (n > 1) {
            diving(0);
        }
        if (n == 1)
        {
            index[name[0]] = 0;
        }
        index[name0] = n;
    }
    int minchild(int i) const // O(d)
    {
        int left_child = leftchild(i);
        if (left_child == -1) {
            return -1; // leaf
        }
        int right_child = rightchild(i);
        int min_child = left_child;
        // if the last element is left_child
        for (int j = left_child + 1; j <= right_child; j++) {
            if (key[j] < key[min_child]) {
                min_child = j;
            }
        }
        return min_child;
    }
    int leftchild(int i) const // O(1)
    {
        int j = i * d + 1;
        if (j >= n) {
            return -1; // leaf
        }
        else {
            return j;
        }
    }
    int rightchild(int i) const // O(1)
    {
        int j1 = i * d + 1;
        int j = (i + 1) * d;
        if (j1 >= n) {
            return -1; // leaf
        }
        return j < (n - 1) ? j : (n - 1); // min does not work
    }
    int parent(int i) const // O(1)
    {
        if (i == 0) {
            return -1; // root
        }
        else {
            return (i - 1) / d;
        }
    }
    bool empty()
    {
        return (n == 0);
    }
};
bool adjacency_matrix(int n, int r, int** table)//r- кол-во ребер, n- кол-во вершин
{
    int kol=0;
    if (r <= (n-1))
        return 0;
    if (r >= n*(n - 1) / 2)
    {
        for (int i=0;i<n;i++)
            for (int j = 0; j < n; j++)
            {
                if (j < i)
                {
                    table[i][j] = rand() % 100;
                    table[j][i] = table[i][j];
                }
            }
        for (int i = 0; i < n; i++)
            table[i][i] = 0;
        return 1;
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            table[i][j] = infinity;
    for (int i = 0; i < n; i++)
        table[i][i] = 0;
    while (kol < r)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                if (j < i)
                {
                    if (rand() % 2 == 0)
                        continue;
                    if (rand() % 2 == 1)
                        if ((table[i][j] == infinity)&&(kol<r))
                        {
                            table[i][j] = rand() % 100;
                            table[j][i] = table[i][j];
                            kol++;
                        }
                }
            }
    }
    return 1;
}
void print_matrix(int** table, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << endl;
        for (int j = 0; j < n; j++)
        {
            if (table[i][j] == infinity)
                table[i][j] = 101;
            cout << table[i][j] << "             ";
        }
    }
}
struct ADJ
{
    int name;
    int weight;
    ADJ(int _name = 0, int _weight = 0) :name(_name), weight(_weight)
    {}
};

class Graph
{
    int size;
    list<ADJ>* graph;
    list<ADJ>::iterator it;
    int i;
public:
    Graph(int** table, int n) : size(n)
    {
        graph = new list<ADJ>[size];
        for (int i = 0; i < size; i++)
        {
            list<ADJ> tmp;
            for (int j = 0; j < size; j++)
            {
                if (table[i][j] != infinity && table[i][j] != 0) {
                    tmp.push_back(ADJ{ j,table[i][j] });
                }
            }
            graph[i] = tmp;
        }
    }
    ~Graph()
    {
        delete[] graph;
    }
    void targeting(int i_)
    {
        i = i_;
        it = graph[i].begin();
    }
    void go_next() // go to next node in list
    {
        ++it;
    }
    ADJ* get_elem()
    {
        if (it != graph[i].end())
        {
            return &(*it);
        }
        else
            return nullptr;
    }
};

bool Prim_d_heap(Graph& graph, int n, int d, int** et)// O((m + n) log n), m-кол-во ребер, n-число вершин
{
    int reb = 0;
    int* key=new int[n];
    int* name = new int[n];
    int* b = new int[n];
    int* index = new int[n]; // номера вершин в d-куче
    graph.targeting(0);
    for (int i = 0; i < n; i++)
    {
        name[i] = i;
        index[i] = i;
        b[i] = 0;
        key[i] = infinity;
    }
    key[0] = 0;
    ADJ* p = graph.get_elem();
    while (p != nullptr)
    {
        int j = p->name; // get real name from graph
        key[j] = p->weight;
        graph.go_next();
        p = graph.get_elem();
    }
    DHeap heap( n,d,name, key,index );
    int min_name, min_key;
    heap.get_min(min_name, min_key);
    while (heap.empty() == false)
    {
        int min_name, min_key;
        heap.get_min(min_name,min_key);
        if (min_key == infinity)
        {
            cout << "not svyazi"<<endl;
            return 0;
        }
            et[reb][0] = b[min_name];
            et[reb][1] = min_name;
            reb++;
            graph.targeting(min_name);
            ADJ* p = graph.get_elem();
            while (p != NULL) 
            {
                int j = p->name; // get real name from graph
                int jh = heap.index[j]; // get local index from heap by the name
                if ((key[jh] > p->weight)) {
                    key[jh] = p->weight;
                    heap.emersion(jh);
                    b[j] = min_name;
                }
                graph.go_next();
                p = graph.get_elem();
            }
    }
}
int find_min(int* a, int n, int *vt)//O(n)
{
    int index = 0;
    int min=infinity;
    for (int i = 1; i < n; i++)
        if ((min > a[i])&&(vt[i]==0))
        {
            index = i;
            min = a[i];
        }
    return index;
}
bool Prim_mass(Graph& graph, int n, int** et)//O(n^2)
{
    int* a = new int[n];
    int* b = new int[n];
    int* vt = new int[n];
    int reb = 0;
    int x;
    for (int i = 0; i < n; i++)
    {
        a[i] = infinity;
        b[i] = 0;
        vt[i] = 0;
    }
    vt[0] = 1;
    graph.targeting(0);
    ADJ* p = graph.get_elem();
    while (p != NULL)
    {
        int bn = p->name; 
        if ((a[bn] > p->weight) && (vt[bn]==0)) {
            a[bn] = p->weight;
        }
        graph.go_next();
        p = graph.get_elem();
    }
    while (reb < n-1)
    {
        x = find_min(a, n,vt);
        if (a[x] == infinity)
        {
            cout << "not svyazi" << endl;
            return 0;
        }
        et[reb][0] = b[x];
        et[reb][1] = x;
        reb++;
        vt[x] = 1;
        graph.targeting(x);
        ADJ* p = graph.get_elem();
        while (p != NULL)
        {
            int bn = p->name; 
            if ((a[bn] > p->weight) && (vt[bn] == 0)) {
                a[bn] = p->weight;
                b[bn] = x;
            }
            graph.go_next();
            p = graph.get_elem();
        }
    }
}


int main(int argc, char* argv[])
{
    srand(time(0));
    int** table;
    int** et1;
    int** et2;
    int n, r;
    cout << "input n"<<endl;
    cin >> n;
    cout << "input r" << endl;

  
    cin >> r;
    table = new int* [n];
    for (int i = 0; i < n; i++)
        table[i] = new int[n];
    et1 = new int* [n-1];
    for (int i=0;i< n - 1;i++)
        et1[i]=new int[2];
    et2 = new int* [n - 1];
    for (int i = 0; i < n - 1; i++)
        et2[i] = new int[2];
    if (adjacency_matrix(n, r, table) == 0)
        cout << "not svyazi"<<endl;
    Graph graph(table, n);
    auto begin1 = std::chrono::steady_clock::now();
    if (Prim_d_heap(graph, n, 10, et1) != 0)
    {
        auto end1 = std::chrono::steady_clock::now();
        cout << endl;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < 2; j++)
                cout << et1[i][j]<<"|";
            cout << "   ";
        }
        cout << endl;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);
        cout << "The time: " << elapsed_ms.count() << " mics\n";
    }

    auto begin = std::chrono::steady_clock::now();
    if (Prim_mass(graph, n, et2) != 0)
    {
        auto end = std::chrono::steady_clock::now();
            cout << endl;
            for (int i = 0; i < n - 1; i++)
            {
                for (int j = 0; j < 2; j++)
                    cout << et1[i][j] << "|";
                cout << "   ";
            }
            cout << endl;
          auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
          cout << "The time: " << elapsed_ms.count() << " mics\n";
    }
}
