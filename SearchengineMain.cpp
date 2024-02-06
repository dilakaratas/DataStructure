#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "bst.h"
#include "hashtable.h"
#include <chrono>


using namespace std;

#pragma comment(linker, "/STACK:6000000") // for the space


struct DocumentItem {
    string documentName;
    int count;
    DocumentItem* next;
    DocumentItem(const string& name, int c) : documentName(name), count(c), next(nullptr) {}
};

class WordItem {

public:
    WordItem() {}

    string word;
    DocumentItem* documents;
    WordItem(const string& w) : word(w) {
        documents = nullptr;
    }
    void Update(WordItem* item) {
        if (documents == nullptr) {
            documents = new DocumentItem(item->documents->documentName, 1);
            return;
        }
        DocumentItem* iterator = documents;
        while (true) {
            if (iterator->documentName == item->documents->documentName) {
                iterator->count++;
                return;
            }
            if (iterator->next == nullptr) break; // check if there is no next document
            iterator = iterator->next; // next iterator
        }
        iterator->next = new DocumentItem(item->documents->documentName, 1);
    }
};

bool operator<(const DocumentItem& left, const DocumentItem& right) {
    return left.documentName < right.documentName;
}

bool operator<(const WordItem& left, const WordItem& right) {
    return left.word < right.word;
}

bool operator>(const DocumentItem& left, const DocumentItem& right) {
    return left.documentName > right.documentName;
}

bool operator>(const WordItem& left, const WordItem& right) {
    return left.word > right.word;
}




template <class T>
long long timing(T& tree, vector<string> words) {
    int i = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int k = 0; k < i; k++) {
        for (string word : words) {
            WordItem item;
            item.word = word;
            tree.search(word);
        }
    }
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() -
             start);
    //cout << "\nTime: " << BSTTime.count() / i << "\n";
    return BSTTime.count();
}

int main() {
    BinarySearchTree<string, WordItem*> bst;
    HashTable<string, WordItem*> hashTable;

    cout << "Enter number of input files: ";
    int inputFileCount;
    cin >> inputFileCount;
    vector<string> inputFileNames;
    for (int i = 1; i <= inputFileCount; i++) {
        string fileName;
        cout << "Enter " << i << ". file name: ";
        cin >> fileName;
        inputFileNames.push_back(fileName);
    }
    // read and process each document
    for (int i = 0; i < inputFileCount; i++) {
        string fileName = inputFileNames[i];

        ifstream file;
        file.open(fileName, ios::in);
        if (!file.is_open())continue;
        WordItem* currentWordItem = nullptr;

        while (!file.eof()) {
            string line;
            getline(file, line);

            string word="";
            // seperating

            // extracting words
            for (int i = 0; i < line.size(); i++) {
                char ch = line[i];
                if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z') { // if alpha character
                    word += ch;
                }
                else if(word != "") {
                    currentWordItem = new WordItem(word);
                    currentWordItem->documents = new DocumentItem(fileName, 1);
                    bst.insert(word, currentWordItem);
                    hashTable.insert(word, currentWordItem);
                    word = ""; // reset
                }
            }
            if (word != "") {
                currentWordItem = new WordItem(word);
                currentWordItem->documents = new DocumentItem(fileName, 1);
                // insert remaining after remaining controller
                bst.insert(word, currentWordItem);
                hashTable.insert(word, currentWordItem);
                word = "";
            }
        }

        file.close();
    }

    cin.ignore();


    cout << "Enter queried words in one line: ";
    string query;
    getline(cin, query);



    vector<WordItem*> results;
    istringstream iss(query);
    string queryWord;
    vector<string> words;

    while (iss >> queryWord) {

        string processedQueryWord;
        for (char ch : queryWord) {
            if (isalpha(ch)) {
                processedQueryWord += tolower(ch);
                }
            }
            words.push_back(processedQueryWord);
            WordItem* queryResult = bst.search(processedQueryWord);
            if (queryResult != nullptr) {
                results.push_back(queryResult);
            }
            else {
                cout << "No document contains the given query" << endl;
            }

        }
        for (int i = 0; i < inputFileCount; i++) {
            string line = "";
            for (int j = 0; j < results.size(); j++) {

                WordItem* queryResult = results[j];
                DocumentItem* iterator = queryResult->documents;

                // where the found it found loop
                while (iterator != nullptr) {
                    if (iterator->documentName == inputFileNames[i]) {
                        line += ", " + queryResult->word + " found " + to_string(iterator->count) + " times";
                    }
                    iterator = iterator->next; // next doucment
                }


            }
            if (line != "") {
                cout << "in Document " << inputFileNames[i] << line << endl;
            }
        }
        cout << endl;

        long long bstTime = timing(bst, words);
        long long hashTime = timing(hashTable, words);

        cout << "Time: " << bstTime << endl << endl;

        cout << "Time: " << hashTime << endl;
        cout << "Speed Up: " << (double)bstTime / hashTime << endl;

    return 0;
}
