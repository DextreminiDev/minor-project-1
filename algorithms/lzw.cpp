// LZW

#include <bits/stdc++.h>
#include <chrono> // For measuring execution time
using namespace std;
using namespace chrono; // Simplifies usage of chrono features

// Function to encode the input string using LZW algorithm and store the compressed output in a file
vector<int> encoding(string s1, string output_file) 
{ 
    cout << "Encoding\n"; 
    
    // Initialize a table (dictionary) to store string patterns and their corresponding codes
    unordered_map<string, int> table; 
    
    // Fill the table with all possible single-character strings and their ASCII values
    for (int i = 0; i <= 255; i++) { 
        string ch = ""; 
        ch += char(i); 
        table[ch] = i; 
    } 

    // Variables to store the current pattern (p) and the next character (c)
    string p = "", c = ""; 
    p += s1[0];  // Initialize pattern with the first character of the input string
    int code = 256;  // Starting code for new patterns (codes 0-255 are reserved for single characters)
    
    // Vector to store the encoded output codes
    vector<int> output_code; 
    
    // Header for console output showing the encoding process
    cout << "String\tOutput_Code\tAddition\n"; 

    // Open the output file to store compressed data
    ofstream outfile(output_file); 
    if (!outfile) {  // Check if the file was successfully opened
        cerr << "Error creating file!" << endl;
        return {};  // Return empty vector if file creation failed
    }

    // Loop through the input string to perform encoding
    for (int i = 0; i < s1.length(); i++) { 
        if (i != s1.length() - 1) 
            c += s1[i + 1];  // Get the next character (c)
        
        // If the pattern (p + c) exists in the table, extend the pattern (p)
        if (table.find(p + c) != table.end()) { 
            p = p + c; 
        } 
        else { 
            // Output the code for the current pattern (p)
            cout << p << "\t" << table[p] << "\t\t" 
                 << p + c << "\t" << code << endl; 
            
            // Store the code in the output vector and write to file
            output_code.push_back(table[p]); 
            outfile << table[p] << " ";  

            // Add the new pattern (p + c) to the table with the next available code
            table[p + c] = code; 
            code++; 
            
            // Reset the pattern (p) to the last character processed (c)
            p = c; 
        } 
        c = "";  // Reset the next character (c) to empty
    } 
    
    // Output the code for the last pattern (p) and write it to the file
    cout << p << "\t" << table[p] << endl; 
    output_code.push_back(table[p]); 
    outfile << table[p] << endl;  // Write the final code and end the line
    
    // Close the file after writing the data
    outfile.close(); 
    
    // Return the vector of output codes
    return output_code; 
} 

// Function to decode the compressed data using LZW algorithm
void decoding(vector<int> op) 
{ 
    cout << "\nDecoding\n"; 
    
    // Initialize a table (dictionary) to map codes to strings
    unordered_map<int, string> table; 
    
    // Fill the table with single-character strings and their corresponding ASCII values
    for (int i = 0; i <= 255; i++) { 
        string ch = ""; 
        ch += char(i); 
        table[i] = ch; 
    } 
    
    // Initialize variables for decoding
    int old = op[0], n;  // 'old' holds the previous code, 'n' will hold the current code
    string s = table[old];  // 's' is the current string decoded from 'old'
    string c = ""; 
    c += s[0];  // 'c' is the first character of the decoded string
    cout << s;  // Output the first decoded string
    
    int count = 256;  // Starting code for new patterns (codes 0-255 are reserved)
    
    // Loop through the compressed data to decode
    for (int i = 0; i < op.size() - 1; i++) { 
        n = op[i + 1];  // Get the next code
        
        // If the code 'n' is not found in the table, use the previous string (s) and add 'c'
        if (table.find(n) == table.end()) { 
            s = table[old]; 
            s = s + c; 
        } 
        else { 
            // If the code 'n' is found, set 's' to the corresponding string
            s = table[n];
        } 
        
        // Output the decoded string
        cout << s; 
        
        // 'c' is the first character of the current string (s)
        c = ""; 
        c += s[0]; 
        
        // Add the new pattern (old string + first character of current string) to the table
        table[count] = table[old] + c; 
        count++;  // Increment the code counter
        
        // Update 'old' to the current code for the next iteration
        old = n; 
    } 
} 

int main() 
{ 
    // Open the input file to read the string to be compressed
    ifstream infile("xyz_random_words.txt");
    if (!infile) {  // Check if the file was successfully opened
        cerr << "Error opening file!" << endl;
        return 1;  // Exit if file not found
    }

    // Read the input string from the file
    string s;
    getline(infile, s);
    infile.close();  // Close the file after reading

    // Specify the output file for storing compressed data
    string output_file = "compressed_output.txt";

    // Measure encoding time
    auto start_encode = high_resolution_clock::now();
    vector<int> output_code = encoding(s, output_file); 
    auto stop_encode = high_resolution_clock::now();
    auto duration_encode = duration_cast<milliseconds>(stop_encode - start_encode);
    
    // Measure decoding time
    auto start_decode = high_resolution_clock::now();
    decoding(output_code); 
    auto stop_decode = high_resolution_clock::now();
    auto duration_decode = duration_cast<milliseconds>(stop_decode - start_decode);

    // Display encoding and decoding times at the end
    cout << "\nEncoding Time: " << duration_encode.count() << " milliseconds\n";
    cout << "Decoding Time: " << duration_decode.count() << " milliseconds\n";

    return 0; // Indicate successful execution
}