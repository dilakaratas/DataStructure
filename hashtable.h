#ifndef HOMEWORK3_HASHTABLE_H
#define HOMEWORK3_HASHTABLE_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

template<typename Key, typename Value>
struct HashNode {
    Key key;
    Value value;
    HashNode* next;
    HashNode(Key k, Value v) : key(k), value(v), next(nullptr) {}
};

template<typename Key, typename Value>
class HashTable {
private:
    vector<HashNode<Key, Value>*> table;
    int capacity;
    int size;
    double full;
    int wordCount;
    float loadFactorThreshold;

    int hashFunction(const Key& key);
    void rehash(bool increase);

public:
    HashTable(int initCapacity = 10, float loadFactor = 0.7);
    ~HashTable();
    void insert(Key k, Value v);
    void remove(const Key& key);
    Value search(const Key& key);
};


//constructor
template<typename Key, typename Value>
HashTable<Key, Value>::HashTable(int initCapacity, float loadFactor)
    : capacity(initCapacity), size(0), loadFactorThreshold(loadFactor), wordCount(0), full(0.0) {
    table = vector<HashNode<Key, Value>*>(capacity, nullptr);
}


// destructor
template<typename Key, typename Value>
HashTable<Key, Value>::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        HashNode<Key, Value>* head = table[i];
        while (head != nullptr) {
            HashNode<Key, Value>* temp = head;
            head = head->next;
            delete temp;
        }
    }
}

template<typename Key, typename Value>
int HashTable<Key, Value>::hashFunction(const Key& key) {
    int hashVal = 0;

    for (int i = 0; i < key.length(); i++)
    {
        hashVal = 37 * hashVal + key.at(i);
    }
    hashVal = hashVal % capacity;

    if (hashVal < 0)
    {
        hashVal = hashVal + capacity;
    }
    return(hashVal);
}


template<typename Key, typename Value>
void HashTable<Key, Value>::rehash(bool increase) {
    int oldCapacity = capacity;
    capacity = increase ? capacity * 2 : max(10, capacity / 2); // capacity doesn't go below a minimum value
    table.resize(capacity, nullptr);

    for (int i = 0; i < oldCapacity; i++) {
        HashNode<Key, Value>* head = table[i];

        while (head != nullptr) { // collision chain
            HashNode<Key, Value>* nextNode = head->next;
            int newIndex = hashFunction(head->key);

            // inserting node at the begining of chain
            head->next = table[newIndex];
            table[newIndex] = head;
            table[i] = nextNode;

            // move to next node in old slot chain
            head = nextNode;

        }
    }
    double loadFactor = static_cast<double>(size) / capacity; // calculating new load factor
    cout << "rehashed..." << endl;
    cout << "previous table size: " << oldCapacity << ", new table size: " << capacity << ", current unique word count: " << wordCount << ", current load factor: " << loadFactor << endl;

}

template<typename Key, typename Value>
void HashTable<Key, Value>::insert(Key k, Value v) {
    int index = hashFunction(k);
    HashNode<Key, Value>* newNode = new HashNode<Key, Value>(k, v);
    if (table[index] == nullptr) // if slot is not empty
        table[index] = newNode;
    else {
        //collision
        HashNode<Key, Value>* traverse = table[index];
        while (traverse->next != nullptr) {
            if (traverse->key == k) { // if it is already exist
                wordCount--; // decrement counter
                full -= 1;
                table[index]->value->Update(v);
                break;
            }
            // chaining like linked list adding end
            if (traverse->next == nullptr) {
                traverse->next = newNode;
            }
        }
    }

    // update counter
    wordCount++;
    full += 1.0;
    size++;

    if (static_cast<float>(size) / capacity > 0.9) { // load facotr checking
        rehash(true);
    }
}


template<typename Key, typename Value>
void HashTable<Key, Value>::remove(const Key& key) {
    int index = hashFunction(key);
    HashNode<Key, Value>* head = (*table)[index];
    HashNode<Key, Value>* prev = nullptr;

    while (head != nullptr) {
        if (head->key == key) {
            if (prev) {
                prev->next = head->next;
            }
            else {
                (*table)[index] = head->next;
            }
            delete head;
            size--;
            break;
        }
        prev = head;
        head = head->next;
    }

    if (static_cast<float>(size) / capacity < 0.25 && capacity > 10) {
        rehash(false);
    }
}


template<typename Key, typename Value>
Value HashTable<Key, Value>::search(const Key& key) {
    int index = hashFunction(key);
    HashNode<Key, Value>* head = table[index];

    while (head != nullptr) {
        if (head->key == key) {
            return head->value;
        }
        head = head->next;
    }
    return nullptr;
}

#endif //HOMEWORK3_HASHTABLE_H
