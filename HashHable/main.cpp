#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <exception>

using namespace std;


enum REPORTING{
    NONE, LOG, POF
};
const int REPORTING_LEVEL = REPORTING::NONE;

template<class ValueType>
struct HashConflictNode{
    string accessKey;
    ValueType data;
    
    HashConflictNode(){ throw logic_error("DON'T USE DEFAULT CONSTRUCTOR FOR HASHCONFLICT NODE"); };
    HashConflictNode(ValueType sentData, string sentAccessKey): accessKey(sentAccessKey), data(sentData){}
};

/*
    Cannot set the same data with the same key
*/
template<class ValueType>
class Hash{
private:
    ValueType data;
    
    string keyUsed;
    bool hasSet;
    list<HashConflictNode<ValueType>>* conflictList;
public:
    Hash(): hasSet(false), conflictList(nullptr){}
    ~Hash(){
        if(conflictList != nullptr) delete conflictList;
    }
    
    void set(ValueType sentData, string key){
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
    

    
    ValueType get(string key){
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
    };
};



template<class ValueType>
class HashTable{
private:
    const int tableSize = 100;
    vector<Hash<ValueType>> hashTable;
public:
    void init(){
        hashTable = vector<Hash<ValueType>>(tableSize);
    }
    
    HashTable(){ init(); };
    HashTable(int size) : tableSize(size){ init(); }
    
    int hasher(string key){
        int index = 0;
        for(int i = 0; i < key.length(); ++i){
            index += (int)key[i];
        }
        return index % tableSize;
    }
    
    void set(string key, ValueType value){
        hashTable[hasher(key)].set(value, key);
    };
    
    ValueType get(string key){
        return hashTable[hasher(key)].get(key);
    };
};

int main(int argc, const char * argv[]){
    HashTable<string> TableA(1);

    TableA.set("SIMILARKEY",   "This is store 1");
    TableA.set("SIMILARKEY",   "This is store 2");
    TableA.set("DIFFERENTKEY", "This is store 3");
    
    cout << TableA.get("SIMILARKEY") << endl;
    cout << TableA.get("SIMILARKEY") << endl;
    cout << TableA.get("DIFFERENTKEY") << endl;
}

