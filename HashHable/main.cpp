#include <iostream>
#include "HashTable.cpp"

using namespace std;


int main(int argc, const char * argv[]){
    HashTable<string> TableA;

    TableA.set("SIMILARKEY",   "This is store 1");
    TableA.set("SIMILARKEY",   "This is store 2");
    TableA.set("DIFFERENTKEY", "This is store 3");

    cout << TableA.get("SIMILARKEY") << endl;
    cout << TableA.get("SIMILARKEY") << endl;
    cout << TableA.get("DIFFERENTKEY") << endl;
}

