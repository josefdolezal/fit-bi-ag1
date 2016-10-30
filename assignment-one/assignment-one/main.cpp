//
//  main.cpp
//  assignment-one
//
//  Created by Josef Dolezal on 29/10/16.
//  Copyright © 2016 Josef Dolezal. All rights reserved.
//

#include <iostream>
#include <string>

#ifndef __PROGTEST__
#include <assert.h>
#endif /* __PROGTEST__ */

using namespace std;


template <typename T>
struct QueueElement {
    QueueElement<T> * next;
    QueueElement<T> * predecessor;
    T data;

    QueueElement<T>(T data): next(NULL), predecessor(NULL), data(data) {}
};


template <typename T>
struct Queue {
private:
    QueueElement<T> * head;
    QueueElement<T> * tail;

    int count = 0;

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

    ~Queue<T>() {
        clear();
    }

    void enqueue(T data) {
        QueueElement<T> * element = new QueueElement<T>(data);

        if(head == NULL) head = element;
        else {
            tail->next = element;
            element->predecessor = tail;
        }

        tail = element;
        count += 1;
    }

    T dequeue() {
        QueueElement<T> * element = head;
        T data = element->data;

        head = head->next;
        count -= 1;

        if(head != NULL) head->predecessor = NULL;
        else tail = NULL;

        delete element;
        return data;
    }

    T pop() {
        QueueElement<T> * element = tail;
        T data = element->data;

        tail = tail->predecessor;
        count -= 1;

        if(tail != NULL) tail->next = NULL;
        else head = NULL;

        delete element;
        return data;
    }

    int getCount() const {
        return count;
    }

    bool empty() const {
        return count == 0;
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


struct Key {
    int type;
    int location;

    Key(int type, int location): type(type), location(location) {}
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

    bool accessibleFromFloor(int floor) const {
        int masked = lock & floor;

        return masked == lock;
    }
};


struct Node {
    int predecessor = -1;
    int routeLength = -1;
    int position;
    int id;

private:
    int keysAvailable = 0;
    Queue<Edge> queue;

public:

    Node(): position(0), id(0) {}

    Node(int id): position(id), id(id) {}

    Node(const Node & node): position(node.position), id(node.id) { deepCopy(node); }

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

    bool haveEdges() const {
        return !queue.empty();
    }

    int getAvailableKeys() const {
        return keysAvailable;
    }

    void addKey(int key) {
        keysAvailable |= key;
    }

    bool visited() const {
        return routeLength >= 0;
    }

    bool hasPredecessor() const {
        return predecessor != -1;
    }

private:
    void deepCopy(const Node & node) {
        queue.clear();

        id = node.id;
        position = node.position;
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
            unsigned char lock = 'x';

            cin >> destination;
            cin >> lock;

            int lockType = Edge::lockType((int) lock);

            node.addEdge(destination, lockType);
        }
        
        return node;
    }

    static Queue<Key> readKeys() {
        Queue<Key> keys;
        int count = 0;

        cin >> count;

        for(int i = 0; i < count; ++i) {
            int location = 0;
            unsigned char c = 'x';

            cin >> location;
            cin >> c;

            keys.enqueue(Key((int) c, location));
        }

        return keys;
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

        loadNodes();
        loadKeys();
    }

    ~Graph() {
        delete[] nodes;
    }

    Queue<int> findPath() {
        Queue<Node *> queue;
        Node * s = &nodes[startNode];

        s->routeLength = 0;

        // Move to the higher floor if possible
        if(s->getAvailableKeys() > s->position / nodesCount) {
            s = &nodes[s->getAvailableKeys() * nodesCount + s->id];
            s->routeLength = 0;
        }

        queue.enqueue(s);

        while(!queue.empty()) {
            Node * v = queue.dequeue();
            //cout << "Open: " << v->id << " at position " << v->position << " (floor " << v->getAvailableKeys() << ")" << endl;

            while(v->haveEdges()) {
                Node * next = processEdge(v);

                if(next == NULL) continue;

                queue.enqueue(next);
            }
        }

        return backtrack();
    }

private:
    void loadNodes() {
        for(int i = 0; i < nodesCount; ++i) {
            Node node = InputReader::readNode(i);

            for(int j = 0; j < 8; ++j) {
                int position = i + j*nodesCount;

                this->nodes[position] = node;
                this->nodes[position].position = position;
            }
        }
    }

    void loadKeys() {
        Queue<Key> keys = InputReader::readKeys();

        while(!keys.empty()) {
            Key key = keys.dequeue();

            for(int i = 0; i < 8; ++i) {
                int type = Edge::lockType(key.type);
                nodes[i * nodesCount + key.location].addKey(type);
            }
        }
    }

    Node * processEdge(Node * predecessor) const {
        Edge edge = predecessor->nextEdge();
        int floor = predecessor->position / nodesCount;

        Node * w = &nodes[edge.destination + floor * nodesCount];

        if(!edge.accessibleFromFloor(floor) || (w->visited() && (w->routeLength < (predecessor->routeLength+1)))) {
            //cout << "  Ooops, cant open room " << w->id << " at position " << w->position << " (floor " << floor << ")" << endl;
            return NULL;
        }

        setPredecessor(w, predecessor);

        int possibleFloor = floor | w->getAvailableKeys();
        // I can move to the higher floor
        if(possibleFloor != floor) {
            //cout << "  Moving to floor " << possibleFloor << " from floor " << floor << endl;
            Node * next = &nodes[possibleFloor * nodesCount + w->id];

            setPredecessor(next, predecessor);
            return next;
        }

        // No candidate in higher floor
        return w;
    }

    void setPredecessor(Node * node, Node * predecessor) const {
        node->routeLength = predecessor->routeLength + 1;
        node->predecessor = predecessor->position;
    }

    Queue<int> backtrack() {
        int index = -1;
        int bestRoute = -1;

        Queue<int> route;

        for(int i = 0; i < 8; ++i) {
            Node * end = &nodes[i*nodesCount + endNode];

            if(end->hasPredecessor() && (bestRoute == -1 || end->routeLength < bestRoute)) {
                index = end->position;
                bestRoute = end->routeLength;
            }
        }

        if(index < 0) return route;

        Node * path = &nodes[index];
        route.enqueue(path->id);

        while(path->hasPredecessor()) {
            Node * predecessor = &nodes[path->predecessor];

            route.enqueue(predecessor->id);
            path = predecessor;
        }

        return route;
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
    searchPath();
    //runTests();
    return 0;
}

void searchPath() {
    int nodes = 0;
    int startNode = 0;
    int endNode = 0;

    InputReader::readTaskDescription(nodes, startNode, endNode);

    Graph graph = Graph(nodes, startNode, endNode);
    Queue<int> path = graph.findPath();

    if(!path.empty()) {
        cout << path.getCount() - 1 << endl;
        bool first = true;

        while(!path.empty()) {
            cout << (first ? "" : " ") << path.pop();
            first = false;
        }

        cout << endl;
    } else {
        cout << "No solution" << endl;
    }
}


#ifndef __PROGTEST__
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

    assert(queue.getCount() == 6);

    for(int i = 0; i < 6; ++i)
        assert(values[i] == queue.dequeue());

    queue.enqueue(7);
    assert(queue.empty() == false);
    assert(queue.getCount() == 1);
    assert(queue.dequeue() == 7);

    queue.enqueue(8);
    queue.enqueue(9);

    assert(queue.dequeue() == 8);
    assert(queue.dequeue() == 9);

    assert(queue.empty() == true);
    assert(queue.getCount() == 0);

    queue.enqueue(12);
    queue.enqueue(13);

    assert(queue.empty() == false);
    assert(queue.getCount() == 2);
    queue.clear();
    assert(queue.empty() == true);
    assert(queue.getCount() == 0);

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

    /* Pop */

    queue.enqueue(14);
    queue.enqueue(15);
    queue.enqueue(16);

    assert(queue.pop() == 16);
    assert(queue.getCount() == 2);
    queue.enqueue(17);
    assert(queue.getCount() == 3);
    assert(queue.pop() == 17);
    assert(queue.dequeue() == 14);
    assert(queue.pop() == 15);
    assert(queue.getCount() == 0);

    queue.enqueue(18);
    assert(queue.pop() == 18);
    assert(queue.empty() == true);

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

    assert(Edge(21, Edge::A).accessibleFromFloor(0) == false);
    assert(Edge(21, Edge::A).accessibleFromFloor(Edge::A) == true);
    assert(Edge(21, Edge::UNLOCKED).accessibleFromFloor(Edge::A) == true);
    assert(Edge(21, Edge::B).accessibleFromFloor(Edge::C) == false);
    assert(Edge(22, Edge::C).accessibleFromFloor(7) == true);


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

#endif /* __PROGTEST__ */
