// CPP program to implement hashing with chaining
#include<iostream>
#include "hash.hpp"


using namespace std;

HashNode* HashTable::createNode(string key, HashNode* next)
{
    vector<int> *commits = new vector<int>();
    HashNode* nw = new HashNode{key, *commits, next};

    return nw;
}

HashTable::HashTable(int bsize)
{
    tableSize = bsize;
    table = new HashNode*[bsize];
    for (int i = 0; i < bsize; i++){
        table[i] = nullptr;
    }
    

}

//function to calculate hash function
unsigned int HashTable::hashFunction(string s)
{
    // takes in a single string needed to be hashed

    int sum = 0, index = 0;

    for (string::size_type i = 0; i < s.length(); i++){
        sum += s[i];
    }

    index = sum % tableSize;

    return index;
}

// TODO Complete this function
//function to search
HashNode* HashTable::searchItem(string key)
{
    int index = hashFunction(key);

    if (table[index] == nullptr){
        return nullptr;
    } else {

        // loop through chain and check if any buckets equal key

        HashNode *tempNode = table[index];

        while (tempNode != nullptr){
            if (tempNode->key == key){
                return tempNode;
            }

            tempNode = tempNode->next;
        }

        return nullptr;
    }
    
}

bool HashTable::insertItem(string key, int cNum)
{
    // check if key is already in hashtable
    // if it is, go to it and add cNUm to vector
    // if not, create a new HashNode
    int index = hashFunction(key);
    HashNode *node = searchItem(key);

    if (node != nullptr){
        node->commitNums.push_back(cNum);
    } else {
        HashNode *newNode = createNode(key, table[index]);
        newNode->commitNums.push_back(cNum);

        table[index] = newNode;

        // go to index 
    }

}


// function to display hash table //
/* assume the table size is 5. For each bucket it will show the 
** the string key and the commit number (separated by comma) within parenthesis
** e.g. the key is science and commit numbers are 1 and 4. The key science
** is hashed to position 0. So the print format will be-

0|| science(1,4,)
1|| 
2|| 
3|| 
4|| difficult(3,)-->fun(2,)-->computer(0,)

*/
void HashTable::printTable()
{
    for (int i = 0; i < tableSize; i++){
        cout << to_string(i) + "|| ";
        string str = "";

        //loop through every index in hashtable
        HashNode *tempNode = table[i];

        while (tempNode != nullptr){
            // for every index 
                // loop through all buckets (tempNode) and print there commit Nums
            str += tempNode->key + "(";

            for (int i : tempNode->commitNums){
                str += to_string(i) + ",";
            }

            str += ")-->";

            tempNode = tempNode->next;
        }
        
        str = str.substr(0, str.length() - 3);
        cout << str << endl;
    }
    
}

 
