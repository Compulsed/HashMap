/*
 Features to add.
 - The ability to use anything other than a string key
 
 Bugs.
 -
 
 ----------------
 Implementation
 ----------------
 Hashing function:
 - Sum of the char array mod storage array length
 
 Conflict handling
 - Conflicts are handled by a list which stores a HashConflictNode
 
 Settling & Getting
 - Set & Get on HashTable will either .set("key", value) the value of the
 Assoc array or get.key("key") it respectively
 
 
 ----------------
 How it works
 ----------------
 The hashing function hashes the char array with the mod of the array length
 
 Setting
 - If there isn't a conflict in the array location it'll store it with the key that was used
 - If there was a HashConflict node will be added onto conflictList
 
 Getting
 - If there is only one hash it'll automatically return it
 - If there is multiple it will go through the list until it fines the one that
 correctly matches the key
 
 
 */


#ifndef HashHable_HashTable_h
#define HashHable_HashTable_h

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>


using namespace std;




template<class ValueType>
struct HashConflictNode{
    string accessKey;
    ValueType data;
    
    HashConflictNode();
    HashConflictNode(ValueType sentData, string sentAccessKey);
};



template<class ValueType>
class Hash{
private:
    ValueType data;
    
    string keyUsed;
    bool hasSet;
    list<HashConflictNode<ValueType>>* conflictList;
public:
    Hash();
    ~Hash();
    
    void set(ValueType sentData, string key); // Setter
    ValueType get(string key);                // Getter
};





template<class ValueType>
class HashTable{
private:
    int tableSize;
    vector<Hash<ValueType>> hashTable;
public:
    void init(); // Is called from all constructors
    
    HashTable();
    HashTable(int size);
    
    
    int hasher(string key); // Used hash the key and find location in hashTable
    
    void set(string key, ValueType value); // Setter
    ValueType get(string key);             // Getter
};


#endif
