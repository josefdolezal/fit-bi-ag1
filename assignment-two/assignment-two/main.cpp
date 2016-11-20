//
//  main.cpp
//  assignment-two
//
//  Created by Josef Dolezal on 19/11/16.
//  Copyright © 2016 Josef Dolezal. All rights reserved.
//

#include <iostream>
#include <stdint.h>


using namespace std;

// Animal
struct Animal {
    uint32_t id;
    uint32_t power;

    Animal * left = NULL;
    Animal * right = NULL;
    Animal * child = NULL;
    Animal * mirror = NULL;

    Animal(uint32_t id, uint32_t power): id(id), power(power) {}

    void clear() {
        left = NULL;
        right = NULL;
        child = NULL;
    }

    void clearSiblings() {
        left = NULL;
        right = NULL;
    }

    ~Animal() {
        left = NULL;
        if(right != NULL) delete right;
        right = NULL;
        if(child != NULL) delete child;
        child = NULL;
        mirror = NULL;
    }
};


bool operator < (const Animal & lhs, const Animal & rhs) {
    if(lhs.power == rhs.power) return lhs.id < rhs.id;

    return lhs.power < rhs.power;
}


bool operator > (const Animal & lhs, const Animal & rhs) {
    if(lhs.power == rhs.power) return lhs.id < rhs.id;

    return lhs.power > rhs.power;
}


class Herd {
public:
    static const bool
        MIN = true,
        MAX = false;

private:
    Animal * head = NULL;

    bool strategy = true;

public:
    Herd(bool strategy): strategy(strategy) {}

    void add(Animal * animal) {
        if(head == NULL) {
            head = animal;
            return;
        }

        Herd single(strategy);
        single.add(animal);

        merge(single);
    }

    Animal * removeExtrem() {
        if(head == NULL) return NULL;

        Animal * oldHead = head;

        if(oldHead->child == NULL)
            head = NULL;
        else
            head = mergeSiblings(oldHead->child);

        oldHead->clear();
        return oldHead;
    }

    void remove(Animal * animal) {
        if(animal == NULL) return;

        // Groooot under the root
        if(animal->left != NULL && animal->left->child == animal) {
            Animal * parent = animal->left;

            parent->child = animal->right;
            if(parent->child != NULL) parent->child->left = parent;

            animal->left = NULL;
            animal->right = NULL;

            Herd h(strategy);

            h.add(animal);
            h.removeExtrem();

            animal->clear();
            delete animal;
            merge(h);

            return;
        }

        // Removing root
        if(animal == head) {
            removeExtrem();
            animal->clear();
            delete animal;
            return;
        }

        // Have siblings

        animal->left->right = animal->right;
        if(animal->right != NULL) animal->right->left = animal->left;

        Herd h(strategy);

        h.add(animal);
        h.removeExtrem();

        animal->clear();
        delete animal;

        merge(h);
        return;
    }

    void merge(Herd & other) {
        // Other Herd is empty
        if(other.head == NULL) return;

        // This Herd is empty
        if(head == NULL) {
            head = other.head;
            other.head = NULL;

            return;
        }

        // The other Heard root is extrem
        if(isBeforeHead(other.head)) {
            putFirstBeforeSecond(other.head, head);
        } else {
            // Current head is still extrem
            putFirstBeforeSecond(head, other.head);
        }

        other.head = NULL;
    }

    void makeEmpty() {
        if(head != NULL) delete head;
    }

private:
    bool isBeforeHead(Animal * animal) {
        if(strategy == MAX) {
            return (*animal) > (*head);
        }

        return (*animal) < (*head);
    }

    void putFirstBeforeSecond(Animal * first, Animal * second) {
        // First does not have childs yet
        if(first->child != NULL)
            first->child->left = second;

        second->right = first->child;
        first->child = second;
        second->left = first;

        head = first;
    }

    Animal * mergeSiblings(Animal * animal) {
        if(animal == NULL) return NULL;

        if(animal->right == NULL) {
            animal->clearSiblings();
            //vyčistit levo?
            return animal;
        }

        Herd h1(strategy), h2(strategy), merged(strategy);
        Animal *f, *s, *n;

        f = animal;
        s = f->right;
        n = s->right;

        f->right = NULL;
        f->left = NULL;
        s->right = NULL;
        s->left = NULL;

        h1.add(f);
        h2.add(s);

        h1.merge(h2);
        merged.add(mergeSiblings(n));

        h1.merge(merged);
        h1.head->clearSiblings();

        return h1.head;
    }
};


// Truck
class Truck {
    Herd weakHerd;
    Herd strongHerd;

public:
    Truck():
        weakHerd(Herd(Herd::MIN)),
        strongHerd(Herd(Herd::MAX))
    { }

    void addAnimal(uint32_t id, uint32_t power) {
        Animal *min = new Animal(id, power);
        Animal *max = new Animal(id, power);

        min->mirror = max;
        max->mirror = min;

        weakHerd.add(min);
        strongHerd.add(max);
    }

    void feedStrongest() {
        Animal * deleted = strongHerd.removeExtrem();

        if(deleted == NULL) {
            cout << "empty" << endl;
            return;
        }

        weakHerd.remove(deleted->mirror);

        cout << deleted->id << endl;

        delete deleted;
    }

    void feedWeakest() {
        Animal * deleted = weakHerd.removeExtrem();

        if(deleted == NULL) {
            cout << "empty" << endl;
            return;
        }

        strongHerd.remove(deleted->mirror);

        cout << deleted->id << endl;

        delete deleted;
    }

    void takeAnimalsFromTruck(Truck & other) {
        weakHerd.merge(other.weakHerd);
        strongHerd.merge(other.strongHerd);
    }

    ~Truck() {
        weakHerd.makeEmpty();
        strongHerd.makeEmpty();
    }
};


// Zoo
class Zoo {
    static const int
        ADD_ANIMAL = 0,
        FEED_STRONGEST = 1,
        FEED_WEAKEST = 2,
        MERGE_TRUCKS = 3,
        CLOSE_ZOO = 4;

    Truck trucks[10001];

public:
    void startFeeding() {
        int command = -2;

        while(cin >> command && command != Zoo::CLOSE_ZOO) {
            processCommand(command);
        }
    }

private:
    void processCommand(int command) {
        switch (command) {
            case Zoo::ADD_ANIMAL:
                readAddAnimal();
                break;
            case Zoo::FEED_STRONGEST:
                readCommandFeedStrongest();
                break;
            case Zoo::FEED_WEAKEST:
                readCommandFeedWeakest();
                break;
            case Zoo::MERGE_TRUCKS:
                readCommandMergeTruckAnimals();
                break;
            default:
                break;
        }
    }

    void readAddAnimal() {
        int truckId;
        uint32_t animalId, power;

        cin >> truckId >> animalId >> power;
        trucks[truckId].addAnimal(animalId, power);
    }

    void readCommandFeedStrongest() {
        int truckId;

        cin >> truckId;
        trucks[truckId].feedStrongest();
    }

    void readCommandFeedWeakest() {
        int truckId;

        cin >> truckId;
        trucks[truckId].feedWeakest();
    }

    void readCommandMergeTruckAnimals() {
        int sourceTruck, destinationTruck;

        cin >> destinationTruck >> sourceTruck;

        if(destinationTruck != sourceTruck)
            trucks[destinationTruck].takeAnimalsFromTruck(trucks[sourceTruck]);
    }
};


int main() {
    Zoo zoo;
    zoo.startFeeding();
}
