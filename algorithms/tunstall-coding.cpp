#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <string>
#include <bitset>

using namespace std;

// Function to sort the unordered_map by values in descending order
vector<pair<string, float>> sortHashmap(unordered_map<string, float>& m){
    vector<pair<string, float>> sortedMap(m.begin(), m.end());
    sort(sortedMap.begin(), sortedMap.end(), [](pair<string, float>& a, pair<string, float>& b){
        return a.second > b.second;
    });
    return sortedMap;
}

// Function to convert frequency counts to probabilities
void toProb(vector<pair<string, float>>& s, float totalChars){
    for(auto& x:s){
        x.second = x.second / totalChars;
    }
}

// Converts an integer code to a binary string with fixed length
string intToBinaryString(int code, int n){
    string binaryString = "";
    while(code > 0){
        binaryString = (code % 2 == 0 ? "0" : "1") + binaryString;
        code /= 2;
    }
    while(binaryString.size() < n){
        binaryString = "0" + binaryString;
    }
    return binaryString;
}

// Updates the Tunstall table (codebook) by generating binary codes for each string symbol
void updateTunstallTable(vector<pair<string, float>>& s, unordered_map<string, string>& tunstallCodebook, int n){
    queue<pair<string, float>> q;
    for(const auto& x : s){
        q.push(x);
    }
    int code = 0;
    while(tunstallCodebook.size() < (1 << n)){
        pair<string, float> top = q.front();
        q.pop();
        // Assign binary code to the symbol if not already coded
        if(tunstallCodebook.find(top.first) == tunstallCodebook.end()){
            tunstallCodebook[top.first] = intToBinaryString(code++, n);
        }
        // Create new sequences by combining with other symbols and calculate probabilities
        for(const auto& symbol : s){
            string newSequence = top.first + symbol.first;
            float newProb = top.second * symbol.second;
            q.push({newSequence, newProb});
        }
    }
}

// Writes a binary string to a file in bytes (8 bits at a time)
void writeBinary(ofstream& outfile, const string& binaryString){
    for(size_t i = 0; i < binaryString.size(); i += 8){
        string byteString = binaryString.substr(i, 8);
        if(byteString.size() < 8){
            byteString.insert(0, 8 - byteString.size(), '0');
        }
        bitset<8> byte(byteString);
        outfile.put(static_cast<unsigned char>(byte.to_ulong()));
    }
}

// Encodes the input file using the Tunstall codebook and writes the encoded binary output to a file
void encode(string& inputFile, unordered_map<string, string>& tunstallCodebook, string& encodedFile){
    ifstream infile(inputFile);
    ofstream outfile(encodedFile, ios::binary);
    char ch;
    string encodedOutput;
    // For each character in input, retrieve its code from the Tunstall codebook
    while(infile.get(ch)){
        string symbol(1, ch);
        encodedOutput += tunstallCodebook[symbol];
    }
    // Write the binary encoded output to the file
    writeBinary(outfile, encodedOutput);
    infile.close();
    outfile.close();
    cout << "Encoding completed. Encoded output written to " << encodedFile << endl;
}

// Reads a binary file and returns its content as a string of binary digits
string readBinary(ifstream& infile){
    string binaryString;
    char byte;
    // Convert each byte into a binary string and append
    while(infile.get(byte)){
        binaryString += bitset<8>(static_cast<unsigned char>(byte)).to_string();
    }
    return binaryString;
}

// Decodes the encoded file back into its original form using the Tunstall codebook
void decode(string& encodedFile, unordered_map<string, string>& tunstallCodebook, string& decodedFile){
    ifstream infile(encodedFile, ios::binary);
    ofstream outfile(decodedFile);
    string binaryString = readBinary(infile);
    // Create a reverse codebook to map binary codes back to their corresponding symbols
    unordered_map<string, string> reverseCodebook;
    for(const auto& pair : tunstallCodebook){
        reverseCodebook[pair.second] = pair.first;
    }
    string currentCode;
    // Read through the binary string and decode each binary code into its symbol
    for(char bit : binaryString){
        currentCode += bit;
        if(reverseCodebook.find(currentCode) != reverseCodebook.end()){
            outfile.put(reverseCodebook[currentCode][0]);
            currentCode.clear();
        }
    }
    infile.close();
    outfile.close();
    cout << "Decoding completed. Decoded output written to " << decodedFile << endl;
}

// Main function to create the Tunstall table, encode the input file, and decode it back
void tunstallTable(string& inputFile, string& encodedFile, string& decodedFile){
    ifstream infile(inputFile);
    unordered_map<char, float> s;
    char ch;
    float totalChars = 0;
    // Count the frequency of each character in the input file
    while(infile.get(ch)){
        s[ch]++;
        totalChars++;
    }
    infile.close();
    unordered_map<string, float> probMap;
    // Convert frequency counts to string-based map for processing
    for(const auto& pair : s){
        probMap[string(1, pair.first)] = pair.second;
    }
    // Sort the characters by frequency and convert them to probabilities
    vector<pair<string, float>> sortedS = sortHashmap(probMap);
    toProb(sortedS, totalChars);
    // Calculate the number of bits required for the code length
    int totalUniqueChars = sortedS.size();
    int n = ceil(log2(totalUniqueChars));
    unordered_map<string, string> tunstallCodebook;
    // Generate the Tunstall codebook
    updateTunstallTable(sortedS, tunstallCodebook, n);
    // Write the Tunstall codebook to a file
    ofstream outfile("key.txt");
    for(auto x:tunstallCodebook){
        outfile << x.first << "->" << x.second << endl;
    }
    outfile.close();
    // Encode the input file and then decode it
    encode(inputFile, tunstallCodebook, encodedFile);
    decode(encodedFile, tunstallCodebook, decodedFile);
}

int main(){
    string inputFile = "input.txt";
    string encodedFile = "encoded.txt";
    string decodedFile = "decoded.txt";
    tunstallTable(inputFile, encodedFile, decodedFile);
    return 0;
}
