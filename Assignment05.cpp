//Name:           Rusty Dillard
//Student ID:     200897416
//Email:          dilla017@cougars.csusm.edu

#include <iostream>

using namespace std;

struct Edge{
    int adj_vtx; // index of adjacent vertex.
    Edge *next; // the link to the next node in the list.
    Edge(){
        this->adj_vtx = 0;
        this->next = nullptr;
    }
};

struct Queue{
    Edge *front;
    Edge *back;
    Edge *next;
    Queue(){
        this->front = this->back = this->next = nullptr;
    }
    // Deep copy to prevent deleting from the graph when popping later.
    void push_back(Edge *edge){
        Edge *newEdge = new Edge;
        newEdge->adj_vtx = edge->adj_vtx;
        if(this->isEmpty()){
            this->front = this->back = newEdge;
        }
        else{
            this->next = newEdge;
            this->back = this->next;
        }
        Edge *temp = edge->next;
        Edge *nTemp = newEdge;
        while(temp != nullptr){
            Edge *nEdge = new Edge;
            nEdge->adj_vtx = temp->adj_vtx;
            nTemp->next = nEdge;
            nTemp = nTemp->next;
            temp = temp->next;
        }
    }
    void pop_front(){
        Edge *temp = this->front;
        //  Empty Queue
        if(this->front == nullptr){
            return;
        }
        //  Only one edge in Queue
        else if(this->front == this->back){
            delete temp;
            this->front = this->back = nullptr;
            return;
        }
        //  Two edges in Queue
        else if(this->front->next == this->back){
            this->front = this->back;
            delete temp;
            return;
        }
        //  Any other number in Queue
        else{
            this->front = this->next;
            delete temp;
            return;
        }
    }
    bool isEmpty() const{
        if(this->front == nullptr){
            return true;
        }
        return false;
    }
};

class Graph {
    int numVertices;
    Edge **adj;
    void DFSUtil(int v, bool visited[]);
public:
    explicit Graph(int numVertices);
    void addEdge(int v, int w);
    void BFS(int s);
    void DFS(int v);
    bool isCyclic();
};

int main() {
//    int choice;
//    int begin_vtx;
//    int dest_vtx;
//    int numVertex;
//    cout << "Welcome to the Graph BFS and DFS simulator!\n";
//    cout << "Please choose the number of vertices for your graph: ";
//    cin >> numVertex;
//    Graph g(numVertex);
//    do{
//        cout << "What would you like to do?\n";
//        cout << "1. Add an edge\n";
//        cout << "2. Run a Depth First Search\n";
//        cout << "3. Run a Breadth First Search\n";
//        cout << "4. Determine if graph is cyclic or acyclic\n";
//        cout << "5. Exit\n";
//        cout << "Please enter a number between 1 and 5: ";
//        cin >> choice;
//        switch (choice) {
//            case 0:
//            case 1:
//                cout << "Enter the beginning vertex between 0 and " << numVertex - 1 << ": ";
//                cin >> begin_vtx;
//                cout << "Enter the destination vertex between 0 and " << numVertex - 1 << ": ";
//                cin >> dest_vtx;
//                g.addEdge(begin_vtx, dest_vtx);
//                break;
//            case 2:
//                cout << "Enter the vertex you'd like to begin your DFS at between 0 and " << numVertex - 1 << ": ";
//                cin >> choice;
//                g.DFS(choice);
//                cout << "DFS Traversal: ";
//                cout << endl;
//                break;
//            case 3:
//                cout << "Enter the vertex you'd like to begin your BFS at between 0 and " << numVertex - 1 << ": ";
//                cin >> choice;
//                g.BFS(choice);
//                cout << "BFS Traversal: ";
//                cout << endl;
//                break;
//            case 4:
//                cout << "Determining whether this graph has a cycle.\n" << ". . .\n";
//                if(g.hasCycle()){
//                    cout << "This graph has a cycle.\n";
//                }
//                else{
//                    cout << "This graph does not have a cycle.\n";
//                }
//            case 5:
//                cout << "Now closing the DFS/BFS program. Thank you!\n";
//                break;
//            default:
//                cout << "Invalid choice.";
//        }
//    }while(choice != 5);
    Graph test(5);
    test.addEdge(0, 1);
    test.addEdge(0, 2);
    test.addEdge(0, 4);
    test.addEdge(1, 2);
    test.addEdge(1,4);
    test.addEdge(2, 3);
    test.addEdge(3,4);
    test.addEdge(4,0);
    cout << "DFS Traversal: ";
    test.DFS(0);
    cout << endl;
    cout << "BFS Traversal: ";
    test.BFS(2);
    cout << endl;
    cout << "Is test Cyclic or Acyclic? ";
    if(test.isCyclic()){
        cout << "Cyclic.\n";
    }
    else {
        cout << "Acyclic.\n";
    }
    Graph newTest(3);
    newTest.addEdge(0, 1);
//    newTest.addEdge(1, 0);
    cout << "Is newTest Cyclic or Acyclic? ";
    if(newTest.isCyclic()){
        cout << "Cyclic.";
    }
    else {
        cout << "Acyclic.";
    }
}

Graph::Graph(int numVertices){
    this->numVertices = numVertices;
    this->adj = new Edge *[this->numVertices];
    for(int i = 0; i < this->numVertices; i++){
        this->adj[i] = new Edge;
        this->adj[i]->next = nullptr;
        this->adj[i]->adj_vtx = i;
    }
}

void Graph::addEdge(int v, int w){
    Edge *newEdge = new Edge;
    newEdge->next = nullptr;
    newEdge->adj_vtx = w;
    Edge *temp = this->adj[v];
    if(temp->next == nullptr){
        temp->next = newEdge;
        return;
    }
    while(temp->next != nullptr){
        temp = temp->next;
    }
    temp->next = newEdge;
}

// Depth First Search starting at the int v
void Graph::DFS(int v){
    bool *visited = new bool[this->numVertices];
    for (int i = 0; i < this->numVertices; i++)
        visited[i] = false;
    this->DFSUtil(v, visited);
}

void Graph::DFSUtil(int v, bool visited[]) {
    visited[v] = true;
    cout << v << " ";
    Edge *i = this->adj[v];
    while(i != nullptr){
        if (!visited[i->adj_vtx]) {
            DFSUtil(i->adj_vtx, visited);
        }
        if(i->next == nullptr && !visited[i->adj_vtx]){
            visited[i->adj_vtx] = true;
            cout << i->adj_vtx << " ";
        }
        i = i->next;
    }
}

void Graph::BFS(int s) {
    bool *visited = new bool[this->numVertices];
    for(int i = 0; i < this->numVertices; i++) {
        visited[i] = false;
    }
    Queue queueTest;
    visited[s] = true;
    queueTest.push_back(this->adj[s]);
    while(!queueTest.isEmpty()) {
        Edge *temp = queueTest.front;
        queueTest.pop_front();
        cout << temp->adj_vtx << " ";
        while(temp != nullptr){
            if(!visited[temp->adj_vtx]){
                visited[temp->adj_vtx] = true;
                queueTest.push_back(this->adj[temp->adj_vtx]);
            }
            temp = temp->next;
        }
    }
}

bool Graph::isCyclic() {
    bool *visited = new bool[this->numVertices];
    for (int i = 0; i < this->numVertices; i++) {
        visited[i] = false;
    }
    for(int j = 0; j < this->numVertices; j++){
        Edge *temp = this->adj[j]->next; // starting with because of the way I initialized **adj in the constructor.
        visited[j] = true;
        while(temp != nullptr){
            if(visited[temp->adj_vtx]){
                return true;
            }
            if(!visited[temp->adj_vtx]){
                visited[temp->adj_vtx] = true;
            }
            temp = temp->next;
        }
    }
    return false;
}