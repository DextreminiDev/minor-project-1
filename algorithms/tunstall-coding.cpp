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

vector<pair<string, float>> sortHashmap(unordered_map<string, float>& m){
    vector<pair<string, float>> sortedMap(m.begin(), m.end());
    sort(sortedMap.begin(), sortedMap.end(), [](pair<string, float>& a, pair<string, float>& b){
        return a.second > b.second;
    });
    return sortedMap;
}

void toProb(vector<pair<string, float>>& s, float totalChars){
    for (auto& x:s){
        x.second = x.second/totalChars;
    }
}

string intToBinaryString(int code, int n){
    string binaryString = "";
    while (code > 0){
        binaryString = (code % 2 == 0 ? "0" : "1") + binaryString;
        code /= 2;
    }
    while (binaryString.size() < n){
        binaryString = "0" + binaryString;
    }
    return binaryString;
}

void updateTunstallTable(vector<pair<string, float>>& s, int K, unordered_map<string, string>& tunstallCodebook, int n){
    queue<pair<string, float>> q;
    for (const auto& x : s){
        q.push(x);
    }
    int code = 0;
    while (tunstallCodebook.size() < (1 << n)){
        pair<string, float> top = q.front();
        q.pop();
        if (tunstallCodebook.find(top.first) == tunstallCodebook.end()){
            tunstallCodebook[top.first] = intToBinaryString(code++, n);
        }
        for (const auto& symbol : s){
            string newSequence = top.first + symbol.first;
            float newProb = top.second * symbol.second;
            q.push({newSequence, newProb});
        }
    }
}

void writeBinary(ofstream& outfile, const string& binaryString){
    for (size_t i = 0; i < binaryString.size(); i += 8){
        string byteString = binaryString.substr(i, 8);
        if (byteString.size() < 8){
            byteString.insert(0, 8 - byteString.size(), '0');
        }
        bitset<8> byte(byteString);
        outfile.put(static_cast<unsigned char>(byte.to_ulong()));
    }
}

void encode(string& inputFile, unordered_map<string, string>& tunstallCodebook, string& encodedFile){
    ifstream infile(inputFile);
    ofstream outfile(encodedFile, ios::binary);
    char ch;
    string encodedOutput;
    while (infile.get(ch)){
        string symbol(1, ch);
        encodedOutput += tunstallCodebook[symbol];
    }
    writeBinary(outfile, encodedOutput);
    infile.close();
    outfile.close();
    cout << "Encoding completed. Encoded output written to " << encodedFile << endl;
}

string readBinary(ifstream& infile){
    string binaryString;
    char byte;
    while (infile.get(byte)){
        binaryString += bitset<8>(static_cast<unsigned char>(byte)).to_string();
    }
    return binaryString;
}

void decode(string& encodedFile, unordered_map<string, string>& tunstallCodebook, string& decodedFile){
    ifstream infile(encodedFile, ios::binary);
    ofstream outfile(decodedFile);
    string binaryString = readBinary(infile);
    unordered_map<string, string> reverseCodebook;
    for (const auto& pair : tunstallCodebook){
        reverseCodebook[pair.second] = pair.first;
    }
    string currentCode;
    for (char bit : binaryString){
        currentCode += bit;
        if (reverseCodebook.find(currentCode) != reverseCodebook.end()){
            outfile.put(reverseCodebook[currentCode][0]);
            currentCode.clear();
        }
    }
    infile.close();
    outfile.close();
    cout << "Decoding completed. Decoded output written to " << decodedFile << endl;
}

void tunstallTable(string& inputFile, string& encodedFile, string& decodedFile){
    ifstream infile(inputFile);
    unordered_map<char, float> s;
    char ch;
    float totalChars = 0;
    while (infile.get(ch)){
        s[ch]++;
        totalChars++;
    }
    infile.close();
    unordered_map<string, float> probMap;
    for (const auto& pair : s){
        probMap[string(1, pair.first)] = pair.second;
    }
    vector<pair<string, float>> sortedS = sortHashmap(probMap);
    toProb(sortedS, totalChars);
    int totalUniqueChars = sortedS.size();
    int n = ceil(log2(totalUniqueChars)) + 1;
    int N = totalUniqueChars;
    int K = (static_cast<int>(pow(2, n)) - N) / (N - 1);
    unordered_map<string, string> tunstallCodebook;
    updateTunstallTable(sortedS, K, tunstallCodebook, n);
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
