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
    const int tableSize = 100;
    vector<Hash<ValueType>> hashTable;
public:
    void init(); // Is called from all constructors
    
    HashTable();
    HashTable(int size);
    
    
    int hasher(string key); // Used hash the key and find location in hashTable
    
    void set(string key, ValueType value); // Setter
    ValueType get(string key);             // Getter
};

enum REPORTING{
    NONE, LOG, POF
};
const int REPORTING_LEVEL = REPORTING::NONE;


/*
 HASH CONFLICT NODE
 */
template<class ValueType>
HashConflictNode<ValueType>::HashConflictNode(){
    throw logic_error("DON'T USE DEFAULT CONSTRUCTOR FOR HASHCONFLICT NODE");
};

template<class ValueType>
HashConflictNode<ValueType>::HashConflictNode(ValueType sentData, string sentAccessKey): accessKey(sentAccessKey), data(sentData){}


/*
 HASH
 */
template<class ValueType>
Hash<ValueType>::Hash(): hasSet(false), conflictList(nullptr){}

template<class ValueType>
Hash<ValueType>::~Hash(){
    if(conflictList != nullptr)
        delete conflictList;
}

template<class ValueType>
void Hash<ValueType>::set(ValueType sentData, string key){
    // First time setting this, no conflicts found
    if(hasSet == false){
        hasSet = true;
        keyUsed = key;
        data = sentData;
    }
    
    // Change the current value
    else if(key == keyUsed){
        data = sentData;
    }
    
    // First conflict found
    else if(hasSet == true && conflictList == nullptr){
        if(REPORTING_LEVEL == REPORTING::LOG) cout << "Conflict found: " << key << "!=" << keyUsed << endl;
        
        conflictList = new list<HashConflictNode<ValueType>>(0);
        conflictList -> push_back(HashConflictNode<ValueType>(sentData, key));
    }
    
    // Second or more conflicts found
    else {
        conflictList -> push_back(HashConflictNode<ValueType>(sentData, key));
    }
};


template<class ValueType>
ValueType Hash<ValueType>::get(string key){
    // Nothing stored given the key
    if(hasSet == false){
        throw logic_error("There is nothing stored in this location");
    }
    
    // Only one thing has been set
    else if(keyUsed == key){
        return data;
    }
    
    else {
        // throw logic_error("Functionality not added to deal with iterating through conflicts");
        if(REPORTING_LEVEL == REPORTING::LOG) cout << "Resolving a conflicted value" << endl;
        
        // Stores a local dereferenced value
        list<HashConflictNode<ValueType>> localConflictList = *conflictList;
        for(typename list<HashConflictNode<ValueType>>::iterator it = localConflictList.begin(); it != localConflictList.end(); ++it){
            if(it->accessKey == key){
                return it->data;
            }
        }
        
        throw logic_error("Could not resolve key conflict");
    }
}

/*
 HASH TABLE
 */

template<class ValueType>
HashTable<ValueType>::HashTable(){
    init();
};

template<class ValueType>
HashTable<ValueType>::HashTable(int size){
    init();
}

template<class ValueType>
void HashTable<ValueType>::init(){
    hashTable = vector<Hash<ValueType>>(tableSize);
}

template<class ValueType>
int HashTable<ValueType>::hasher(string key){
    int index = 0;
    for(int i = 0; i < key.length(); ++i){
        index += (int)key[i];
    }
    return index % tableSize;
}

template<class ValueType>
void HashTable<ValueType>::set(string key, ValueType value){
    hashTable[hasher(key)].set(value, key);
};

template<class ValueType>
ValueType HashTable<ValueType>::get(string key){
    return hashTable[hasher(key)].get(key);
};




#endif
