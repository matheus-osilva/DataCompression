#include "auxiliar.h"

void generateCodes(Node* root, std::string currentCode, std::map<char, std::string>& codeMap) {
    if (root == nullptr) {
        return;
    }
    // If the current node is a leaf, store the code in the map
    if (root->left == nullptr && root->right == nullptr) {
        codeMap[root->ch] = currentCode;
        return;
    }

    // Traverse left child with '0'
    if (root->left != nullptr)
    {
        generateCodes(root->left, currentCode + '0', codeMap);
    }
    
    // Traverse right child with '1'
    if (root->right != nullptr)
    {
        generateCodes(root->right, currentCode + '1', codeMap);
    }
    
}
