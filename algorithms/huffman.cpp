// Huffman

#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <chrono> // For measuring time
using namespace std;
using namespace chrono;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;
};

// Custom comparator for priority queue
struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Function to create a new node
Node* getNode(char ch, int freq, Node* left, Node* right) {
    Node* node = new Node();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

// Function to encode characters using the Huffman tree
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode) {
    if (root == nullptr)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// Function to decode the encoded data using the Huffman tree
string decode(Node* root, string encodedStr) {
    string result = "";
    Node* currentNode = root;
    for (char bit : encodedStr) {
        if (bit == '0')
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;

        if (!currentNode->left && !currentNode->right) {
            result += currentNode->ch;
            currentNode = root;
        }
    }
    return result;
}

// Function to build Huffman Tree and compress the file
void buildHuffmanTree(string inputFilePath, string outputFilePath, Node*& root) {
    ifstream inputFile(inputFilePath, ios::in);
    if (!inputFile.is_open()) {
        cout << "Error: Could not open input file!" << endl;
        return;
    }

    unordered_map<char, int> freq;
    char ch;
    while (inputFile.get(ch)) {
        freq[ch]++;
    }
    inputFile.clear();
    inputFile.seekg(0);

    priority_queue<Node*, vector<Node*>, compare> pq;
    for (auto pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    root = pq.top();

    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    ofstream outputFile(outputFilePath, ios::out | ios::binary);
    if (!outputFile.is_open()) {
        cout << "Error: Could not open output file!" << endl;
        return;
    }

    string encodedString = "";
    while (inputFile.get(ch)) {
        encodedString += huffmanCode[ch];
    }

    int extraBits = 8 - (encodedString.size() % 8);
    for (int i = 0; i < extraBits; ++i)
        encodedString += "0";

    outputFile << extraBits << "\n";

    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i, 8));
        outputFile.put(byte.to_ulong());
    }

    inputFile.close();
    outputFile.close();
    cout << "File compressed successfully!" << endl;
}

// Function to decompress the file
void decompressFile(string compressedFilePath, string decompressedFilePath, Node* root) {
    ifstream inputFile(compressedFilePath, ios::in | ios::binary);
    if (!inputFile.is_open()) {
        cout << "Error: Could not open compressed file!" << endl;
        return;
    }

    ofstream outputFile(decompressedFilePath, ios::out);
    if (!outputFile.is_open()) {
        cout << "Error: Could not open decompressed file!" << endl;
        return;
    }

    int extraBits;
    inputFile >> extraBits;
    inputFile.get();  // Skip the newline after reading extraBits

    string encodedString = "";
    char byte;
    while (inputFile.get(byte)) {
        bitset<8> bits((unsigned char)byte);
        encodedString += bits.to_string();
    }

    encodedString = encodedString.substr(0, encodedString.size() - extraBits);

    string decodedString = decode(root, encodedString);

    outputFile << decodedString;

    inputFile.close();
    outputFile.close();
    cout << "File decompressed successfully!" << endl;
}

int main() {
    string inputFilePath = "words_alpha.txt";
    string compressedFilePath = "xyz_huff";
    string decompressedFilePath = "huf.txt";
    Node* root = nullptr;

    auto startEncoding = high_resolution_clock::now();
    buildHuffmanTree(inputFilePath, compressedFilePath, root);
    auto endEncoding = high_resolution_clock::now();
    auto encodingDuration = duration_cast<milliseconds>(endEncoding - startEncoding);
    cout << "Encoding Time: " << encodingDuration.count() << " ms" << endl;

    auto startDecoding = high_resolution_clock::now();
    decompressFile(compressedFilePath, decompressedFilePath, root);
    auto endDecoding = high_resolution_clock::now();
    auto decodingDuration = duration_cast<milliseconds>(endDecoding - startDecoding);
    cout << "Decoding Time: " << decodingDuration.count() << " ms" << endl;

    return 0;
}