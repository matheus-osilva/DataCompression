#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include "auxiliar/auxiliar.h"

void writeBitToFile(std::ofstream& outFile, bool bit) {
    static unsigned char buffer = 0;
    static unsigned int bitCount = 0;

    // Set the bit in the buffer if it's '1'
    if (bit)
        buffer |= (1 << (7 - bitCount));

    ++bitCount;

    // If the buffer is full, write it to the file
    if (bitCount == 8) {
        outFile.write(reinterpret_cast<const char*>(&buffer), sizeof(buffer));
        buffer = 0;
        bitCount = 0;
    }
}

void writeTrie(Node* root, std::ofstream& outFile) {
    if (root->left == nullptr && root->right == nullptr)
    {
        bool bitt = true;
        outFile.write((char *)&bitt, sizeof(bitt));
        char ch = root->ch;
        outFile.write((char*)&ch, 1);
        return;
    }
    bool bitf = false;
    outFile.write((char *)&bitf, sizeof(bitf));
    writeTrie(root->left, outFile);
    writeTrie(root->right, outFile);
}

void writeTrieToFile(Node* root, std::string filename) {
    std::ofstream outFile(filename, std::ios::binary | std::ios::out);
    writeTrie(root, outFile);
    outFile.close();
    return;
}

int main() {
    std::string file_path;
    std::cout << "Insira o nome do arquivo ou seu endereco: ";
    std::cin >> file_path;

    // Read the text file
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Count the occurrences of each character
    std::map<char, int> charCounts;
    for (char c : text) {
        charCounts[c]++;
    }

    // Sort the characters based on their frequency in ascending order
    std::vector<std::pair<char, int>> sortedChars(charCounts.begin(), charCounts.end());
    std::sort(sortedChars.begin(), sortedChars.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) { return a.second < b.second;});

    // Create a vector with all nodes
    std::vector<Node*> nodesChars;
    for (auto& entry : sortedChars){
        Node* character = new Node(entry.first, entry.second);
        nodesChars.push_back(character);
    }

    // Build the priority queues with leaves
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> leaves;
    for (Node* i : nodesChars) {
        leaves.push(i);
    }

    // Until there is only one element in leaves create the trie
    char mod = '\0';
    while (leaves.size() > 1) {

        Node* one = leaves.top();
        leaves.pop();
        Node* two = leaves.top();
        leaves.pop();
        int size = one->freq + two->freq;
        char nullChar = '\0';
        Node* father = new Node(nullChar, size);
        father->left = one;
        father->right = two;
        father->ch = mod;
        leaves.push(father);
        mod += 1;
    } 
    Node* root = leaves.top(); // Trie root
    leaves.pop();

    // Create the codification vector
    std::map<char, std::string> codeMap;
    generateCodes(root, "", codeMap);
    std::string fullCode;
    for (char c : text) {
        fullCode.append(codeMap[c]);
    }

    // Open or create a file to insert the trie
    std::string filename = "trie.bin";
    std::ofstream outputFile(filename, std::ios::binary | std::ios::out);// create the file if it does not exists
    writeTrieToFile(root, filename);
    std::string textfile = "texto.bin";
    std::ofstream outtext(textfile, std::ios::binary | std::ios::out);
    int size = fullCode.size();
    for (char c : fullCode)
    {
        if (c == '1')
        {
            writeBitToFile(outtext, true);
        }
        else if (c == '0')
        {
            writeBitToFile(outtext, false);
        }
    }
    int restsize = fullCode.size() % 8;
    for (int i = 0; i < 8 - restsize; i++)
    {
        writeBitToFile(outtext, false);
    }
    
    
    outputFile.close();
    outtext.close();
    file.close();
    return 0;
}