#include <map>
#include <string>

// Node from Trie
struct Node{
    char ch;
    int freq;
    Node* left;
    Node* right;
    Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

// Auxiliar struct to priority queue
struct CompareNodes {
    bool operator()(Node* node1, Node* node2) {
        // The node with the lower frequency has higher priority
        return node1->freq > node2->freq;
    }
};

// Generate the codes and map it with the characters in the text
void generateCodes(Node* root, std::string currentCode, std::map<char, std::string>& codeMap);
