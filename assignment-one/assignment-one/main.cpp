//
//  main.cpp
//  assignment-one
//
//  Created by Josef Dolezal on 29/10/16.
//  Copyright © 2016 Josef Dolezal. All rights reserved.
//

#include <iostream>
#include <assert.h>


using namespace std;


template <typename T>
struct QueueElement {
    QueueElement<T> * next;
    T data;

    QueueElement<T>(T data): next(NULL), data(data) {}
};

template <typename T>
struct Queue {
private:
    QueueElement<T> * head;
    QueueElement<T> * tail;

public:
    Queue<T>(): head(NULL), tail(NULL) {}

    /* Copy cstor */
    Queue<T>(const Queue<T> & queue): head(NULL), tail(NULL) { deepCopy(queue); }

    /* Operator = */
    Queue<T> & operator = (const Queue<T> & queue) {
        if(this == &queue) { return *this; }

        deepCopy(queue);

        return *this;
    }

    void enqueue(T data) {
        QueueElement<T> * element = new QueueElement<T>(data);

        if(head == NULL) head = element;
        else tail->next = element;

        tail = element;
    }

    T dequeue() {
        QueueElement<T> * element = head;
        T data = element->data;

        head = head->next;

        delete element;
        return data;
    }

    bool empty() {
        return head == NULL;
    }

    void clear() {
        while(!empty()) { dequeue(); }
    }

private:
    void deepCopy(const Queue<T> & queue) {
        clear();

        QueueElement<T> * element = queue.head;

        while(element != NULL) {
            enqueue(element->data);

            element = element->next;
        }
    }
};


struct Edge {
    static const int UNLOCKED = 0;
    static const int A = 1;
    static const int B = 2;
    static const int C = 4;

    const int destination;
    const int lock;

    Edge(
         int destination,
         int lockType = 0
    ): destination(destination), lock(lockType) {}

    static int lockType(int lock) {
        switch(lock) {
        case 'a': return A;
        case 'b': return B;
        case 'c': return C;
        default: return UNLOCKED;
        }
    }
};


struct Node {
    int predecessor = -1;
    int routeLength = 0;
    const int id;

private:
    int keysAvailable = 0;
    Queue<Edge> queue;

public:

    Node(): id(0) {}

    Node(int id): id(id) {}

    Node(const Node & node): id(node.id) { deepCopy(node); }

    Node & operator = (const Node & node) {
        if(this == &node) return *this;

        deepCopy(node);

        return *this;
    }

    void addEdge(int destination, int lockType) {
        Edge edge = Edge(destination, lockType);

        queue.enqueue(edge);
    }

    Edge nextEdge() {
        return queue.dequeue();
    }

    bool haveEdges() {
        return !queue.empty();
    }

    int getAvailableKeys() {
        return keysAvailable;
    }

    void addKey(int key) {
        keysAvailable |= key;
    }

    bool visited() {
        return routeLength > 0;
    }

    bool hasPredecessor() {
        return predecessor != -1;
    }

private:
    void deepCopy(const Node & node) {
        queue.clear();

        predecessor = node.predecessor;
        routeLength = node.routeLength;
        keysAvailable = node.keysAvailable;
        queue = node.queue;
    }
};

struct InputReader {
    static void readTaskDescription(int &nodes, int &startNode, int &endNode) {
        cin >> nodes;
        cin >> startNode;
        cin >> endNode;
    }

    static Node readNode(int id) {
        Node node(id);
        int edges = 0;

        cin >> edges;

        for(int i = 0; i < edges; ++i) {
            int destination = 0;
            int lock = 0;

            cin >> destination;
            cin >> lock;

            int lockType = Edge::lockType(lock);

            node.addEdge(destination, lockType);
        }
        
        return node;
    }
};


struct Graph {
private:
    int startNode;
    int endNode;
    int nodesCount;

    Node * nodes;

public:
    Graph(
          int nodes,
          int startNode,
          int endNode
    ): startNode(startNode), endNode(endNode), nodesCount(nodes) {
        this->nodes = new Node[ 8 * nodes ];

        for(int i = 0; i < nodesCount; ++i) {
            Node node = InputReader::readNode(i);

            for(int j = 0; j < 8; ++j) {
                this->nodes[i + j*nodesCount] = node;
            }
        }
    }

    ~Graph() {
        delete[] nodes;
    }
};

/* Run tests */

void runTests();
void testQueue();
void testNode();
void testEdge();

/* Algorithm */
void searchPath();


int main() {
    // searchPath();
    runTests();
    return 0;
}

void searchPath() {
    int nodes = 0;
    int startNode = 0;
    int endNode = 0;

    InputReader::readTaskDescription(nodes, startNode, endNode);

    Graph graph = Graph(nodes, startNode, endNode);

    int a = 'c';

    cout << a;
}


void runTests() {
    testQueue();
    testEdge();
    testNode();
}


void testQueue() {
    int values[] = {1, 2, 3, 4, 5, 6};
    Queue<int> queue;
    Queue<int> queueCopy;

    for(int i = 0; i < 6; ++i) queue.enqueue(values[i]);

    for(int i = 0; i < 6; ++i)
        assert(values[i] == queue.dequeue());

    queue.enqueue(7);
    assert(queue.empty() == false);
    assert(queue.dequeue() == 7);

    queue.enqueue(8);
    queue.enqueue(9);

    assert(queue.dequeue() == 8);
    assert(queue.dequeue() == 9);

    assert(queue.empty() == true);

    queue.enqueue(12);
    queue.enqueue(13);

    assert(queue.empty() == false);
    queue.clear();
    assert(queue.empty() == true);

    /* Operator = */
    queue.enqueue(10);
    queueCopy = queue;

    assert(queue.dequeue() == 10);
    assert(queue.empty() == true);
    assert(queueCopy.empty() == false);
    assert(queueCopy.dequeue() == 10);
    assert(queueCopy.empty() == true);


    /* Copy constructor */
    queue.enqueue(11);
    Queue<int> queueConstructed = queue;

    assert(queue.dequeue() == 11);
    assert(queueConstructed.dequeue() == 11);

    cout << "Queue: All tests succeeded." << endl;
}

void testEdge() {
    Edge edge(21, Edge::A);

    assert(Edge::lockType('a') == Edge::A);
    assert(Edge::lockType('b') == Edge::B);
    assert(Edge::lockType('c') == Edge::C);
    assert(Edge::lockType('x') == Edge::UNLOCKED);

    assert(edge.lock == Edge::A);
    assert(edge.destination == 21);

    Edge copy = edge;

    assert(copy.lock == Edge::A);
    assert(copy.destination == 21);

    cout << "Edge: All tests succeeded." << endl;
}

void testNode() {
    Node node;

    node.addKey(Edge::A);
    assert(node.getAvailableKeys() == Edge::A);
    node.addKey(Edge::B);
    assert(node.getAvailableKeys() == 3);
    node.addKey(Edge::C);
    assert(node.getAvailableKeys() == 7);
    node.addKey(Edge::UNLOCKED);
    assert(node.getAvailableKeys() == 7);

    assert(node.haveEdges() == false);
    assert(node.hasPredecessor() == false);
    assert(node.visited() == false);

    node.addEdge(1, Edge::A);
    node.addEdge(2, Edge::B);
    node.addEdge(3, Edge::C);
    node.predecessor = 2;
    node.routeLength = 3;
    Node copy = node;

    assert(node.haveEdges() == true);
    assert(node.hasPredecessor() == true);
    assert(node.visited() == true);
    assert(copy.haveEdges() == true);
    assert(copy.hasPredecessor() == true);
    assert(copy.visited() == true);


    int edgesCounted = 0;
    while(node.haveEdges()) {
        Edge edge = node.nextEdge();
        assert(edge.lock < 5);
        edgesCounted++;
    }

    assert(node.haveEdges() == false);
    assert(copy.haveEdges() == true);
    assert(edgesCounted == 3);

    int copyEdgesCounted = 0;
    while(copy.haveEdges()) {
        Edge edge = copy.nextEdge();
        assert(edge.lock < 5);
        copyEdgesCounted++;
    }

    assert(copy.haveEdges() == false);
    assert(copyEdgesCounted == 3);
    
    cout << "Node: All tests succeeded." << endl;
}
