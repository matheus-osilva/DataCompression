#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include "auxiliar/auxiliar.h"

bool readBitFromFile(std::ifstream& inFile, unsigned char& buffer, unsigned int& bitCount) {
    // If the buffer is empty, read the next byte from the file
    if (bitCount == 0) {
        if (!inFile.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
            // Reached the end of the file
            return false;
        }
        bitCount = 8;
    }

    // Extract the next bit from the buffer
    bool bit = (buffer >> (bitCount - 1)) & 1;
    --bitCount;

    return bit;
}

Node* readTrie(std::ifstream& inFile) {
    bool bitread;
    char ch;
    inFile.read((char*)&bitread, 1);
    if (bitread) {
        inFile.read((char*)&ch, 1);
        Node* leaf = new Node(ch, 0);
        leaf->left = nullptr;
        leaf->right = nullptr;
        return leaf;
    }
    Node* novo = new Node('\0', 0);
    novo->left = readTrie(inFile);
    novo->right = readTrie(inFile);
    return novo;
}

Node* readTrieFromFile(std::string filename) {
    std::ifstream inFile(filename, std::ios::binary | std::ios::in);
    if (!inFile.is_open())
    {
        return nullptr;
    }
    Node* trie = readTrie(inFile);
    inFile.close();
    return trie;
}

void generateText(Node* root, std::string& currentText, std::ifstream& inFile) {
    bool bit;
    Node* actual;
    unsigned char buffer;
    unsigned int bitCount = 0;
    while (inFile.tellg() != std::ifstream::pos_type(-1))
    {
        actual = root;
        while (!(actual->left == nullptr && actual->right == nullptr))
        {
            bit = readBitFromFile(inFile, buffer, bitCount);
            if (bit) {
                actual = actual->right;
            }
            else {
                actual = actual->left;
            }
        }
        currentText += actual->ch;
    }
    currentText = currentText.substr(0, currentText.size() - 1);
}

int main() {
    std::string triebin;
    std::string textbin;
    std::string textout;
    std::cout << "Insira o nome do arquivo da arvore de descompactacao: ";
    std::cin >> triebin;
    std::cout << "Insira o nome do arquivo compactado: ";
    std::cin >> textbin;
    std::cout << "Insira o nome para o novo arquivo de texto: ";
    std::cin >> textout;
    //Recreate the read trie
    Node* root = readTrieFromFile(triebin);
    if (root == nullptr)
    {
        std:: cout << "Nao foi possivel abrir o arquivo da arvore";
        return 1;
    }

    std::string text;
    std::ifstream textInput(textbin, std::ios::binary | std::ios::in);
    if (!textInput.is_open())
    {
        std:: cout << "Nao foi possivel abrir o arquivo de texto compactado";
        return 1;
    }
    generateText(root, text, textInput);
    std::ofstream textOutput(textout, std::ios::binary | std::ios::out);
        if (!textOutput.is_open())
    {
        std:: cout << "Nao foi possivel abrir o arquivo de texto compactado";
        return 1;
    }
    textOutput << text;
}