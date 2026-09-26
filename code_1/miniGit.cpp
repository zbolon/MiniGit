#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "miniGit.hpp"
#include <vector>

MiniGit::MiniGit() {
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");

    fs::remove_all("working_directory");
    fs::create_directory("working_directory");
    commits = 0;
    currentCommit = 0;
}

MiniGit::~MiniGit() {   
    // Any postprocessing that may be required
    // delete everything
        // don't need to delete the files, technically, since constructor does that
        // just make sure to delete all DLL nodes and all SLL nodes in those DLL nodes

    // also have to delete the hashtable
        // make a destructor for the hashtable so code looks cleaner

    BranchNode *currBranch = commitHead;
    BranchNode *tempBranch;

    while (currBranch != nullptr){
        tempBranch = currBranch;
        // delete the SLL files
        FileNode *currFile = currBranch->fileHead;
        FileNode *tempFile;

        while (currFile != nullptr){
            tempFile = currFile;


            currFile = tempFile->next;
            delete tempFile;
        }

        currBranch = currBranch->next;
        delete tempBranch;
    }
}

void MiniGit::init(int hashtablesize) {
    // created the directory in the constructor
    commitHead = new BranchNode{0, "", nullptr, nullptr, nullptr};

    //probably create a hashtable of hashtablesize
    ht = new HashTable(hashtablesize);
}

void MiniGit::add(string fileName){

    if (currentCommit != getCurrentBranch()->commitID){
        cout << "Must be on the current branch to make changes" << endl;
        return;
    }

    string filepath = "../build/working_directory/";
    
    //1. check if file exists
    while (!fs::exists(filepath + fileName)){
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cout << "File does not exist. Please enter a valid file name" << endl << "#> ";
        cin >> fileName;
    }

    //2. check if already added to commit
    BranchNode *currBranch = getCurrentBranch();
    FileNode *currFile = currBranch->fileHead;
    FileNode *prev = nullptr;

    while (currFile != nullptr){
        if (currFile->name == fileName){
            cout << "File has already been added to commit" << endl;
            return;
        }
        prev = currFile;
        currFile = currFile->next;
    }

    //3. create a new filenode and add to commit
    FileNode *newNode = new FileNode{fileName, 0, nullptr};

    if (prev == nullptr){
        currBranch->fileHead = newNode;
    } else {
        prev->next = newNode;
    }

}

void MiniGit::rm(string fileName) {

    if (currentCommit != getCurrentBranch()->commitID){
        cout << "Must be on the current branch to make changes" << endl;
        return;
    }

    BranchNode *currBranch = getCurrentBranch();
    FileNode *currFile = currBranch->fileHead;
    FileNode *prev = nullptr;

    // 1. Check if you can remove the fileName
    while (currFile != nullptr){
        // delete file
        if (currFile->name == fileName){
            if (prev == nullptr){
                currBranch->fileHead = currFile->next;
            } else {
                prev->next = currFile->next;
            }
            delete currFile;
            return;
        }

        prev = currFile;
        currFile = currFile->next;
    }

    cout << "Cannot remove file because it was not added to the commit" << endl;
}


void MiniGit::printSearchTable()
{
    ht->printTable();
}

void MiniGit::search(string key)
{
    // find the index where the key is
    // find the bucket with the key
    // print all the commitIDs in that bucket

    HashNode *bucket = ht->searchItem(key);

    if (bucket == nullptr){
        cout << "There is no key named " << key << endl;
    }

    while (bucket != nullptr){

        if (bucket->key == key){
            cout << "Commits with name " << key << ": ";

            for (int i : bucket->commitNums){
                cout << i << ", ";
            }
            cout << endl;

            break;
        }

        bucket = bucket->next;
    }
}

string MiniGit::commit(string msg) {

    if (currentCommit != getCurrentBranch()->commitID){
        cout << "Must be on the current branch to make changes" << endl;
        return "";
    }

    BranchNode *currBranch = getCurrentBranch();
    FileNode *currFile = currBranch->fileHead;
    string workingpath = "../build/working_directory/";
    currBranch->commitMessage = msg;

    BranchNode *newBranch = new BranchNode{currBranch->commitID + 1, "", nullptr, currBranch, nullptr};
    FileNode *prev = nullptr;
    currBranch->next = newBranch;

    while (currFile != nullptr){
        string minipath = "../build/.minigit/" + currFile->name + "_" + to_string(currFile->version);

        if (!fs::exists(minipath)){
            ofstream file;
            file.open(minipath);
            file << readFile(workingpath + currFile->name);
            file.close();

        } else {
            string currNotes = readFile(workingpath + currFile->name);
            string miniNotes = readFile(minipath);

            if (currNotes != miniNotes){
                ofstream file;
                currFile->version += 1;
                file.open("../build/.minigit/" + currFile->name + "_" + to_string(currFile->version));
                file << currNotes;
                file.close();
            }

        }

        FileNode *newFile = new FileNode{currFile->name, currFile->version, nullptr};
        if (prev == nullptr){
            newBranch->fileHead = newFile;
            prev = newBranch->fileHead;
        } else {
            prev->next = newFile;
            prev = prev->next;
        }

        
        currFile = currFile->next;
    }

    istringstream iss(msg);
    string portion;
    
    while (getline(iss, portion, ' ')){
        ht->insertItem(portion, currBranch->commitID);
    }

    commits += 1;
    currentCommit += 1;
    

    return msg;
}

void MiniGit::checkout(string commitID) {

    while (true){
        if (commitID.empty()){
            cout << "Please enter a commit ID" << endl << "#> ";
            getline(cin, commitID);
            continue;
        }

        try{
            int commitNum = stoi(commitID);

            if (commitNum < 0 || commitNum > commits){
                throw (commitNum);
            }
            currentCommit = commitNum;
            break;
        } catch (...){
            cout << "Please enter a number from 0 - " << commits << endl << "#> ";
            getline(cin, commitID);
        }
    }


    // overwrite the files in working directory// don't delete
    BranchNode *oldBranch = commitHead;

    for (int i = 0; i < stoi(commitID); i++){
        oldBranch = oldBranch->next;
    }

    FileNode *oldFile = oldBranch->fileHead;

    while (oldFile != nullptr){
        ofstream file;
        string oldNotes = readFile("../build/.minigit/" + oldFile->name + "_" + to_string(oldFile->version));
        string filepath = "../build/working_directory/" + oldFile->name;
        file.open(filepath);
        file << oldNotes;
        file.close();

        oldFile = oldFile->next;
    }

    currentCommit = stoi(commitID);

}



BranchNode* MiniGit::getCommitHead(){
    return commitHead;
}


string MiniGit::readFile(string filename){
    ifstream myFile(filename);
    string line = "";
    string str = "";
    

    while (getline(myFile, line)){
        str += line + "\n";
    }

    str = str.substr(0, str.size() - 1);


    myFile.close();

    return str;
}

// Probably easier to just make a pointer to the current branch, but whatever
BranchNode* MiniGit::getCurrentBranch(){
    BranchNode *tempBranch = commitHead;
    while (tempBranch->next != nullptr){
        tempBranch = tempBranch->next;
    }

    return tempBranch;
}
