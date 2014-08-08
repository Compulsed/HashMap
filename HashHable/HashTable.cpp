//
//  HashTable.cpp
//  HashHable
//
//  Created by Dale Salter on 8/08/2014.
//  Copyright (c) 2014 none. All rights reserved.
//

#include "HashTable.h"


    
HashConflictNode::HashConflictNode(){ throw logic_error("DON'T USE DEFAULT CONSTRUCTOR FOR HASHCONFLICT NODE"); };
HashConflictNode::HashConflictNode(ValueType sentData, string sentAccessKey): accessKey(sentAccessKey), data(sentData){}

Hash::Hash(): hasSet(false), conflictList(nullptr){}
Hash::~Hash(){
    if(conflictList != nullptr) delete conflictList;
}
    
void Hash::set(ValueType sentData, string key){
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
    
    
    
ValueType Hash::get(string key){
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



void HashTable::init(){
        hashTable = vector<Hash<ValueType>>(tableSize);
}
    
HashTable::HashTable(){ init(); };
HashTable::HashTable(int size) : tableSize(size){ init(); }
    
int HashTable::hasher(string key){
        int index = 0;
        for(int i = 0; i < key.length(); ++i){
            index += (int)key[i];
        }
        return index % tableSize;
    }
    
void HashTable::set(string key, ValueType value){
        hashTable[hasher(key)].set(value, key);
};
    
ValueType HashTable::get(string key){
        return hashTable[hasher(key)].get(key);
};
