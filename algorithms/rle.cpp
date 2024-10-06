#include <bits/stdc++.h>

using namespace std;

void compress_text_file(const string& input_file, const string& output_file);
void decompress_text_file(const string& input_file, const string& output_file);



int main(void){
    string path = "";
    string input_file = "";

    string input_txt = path + input_file;
    string compressed_txt = path + "compressed_" + input_file;
    string decompressed_txt = path + "decompressed_" + input_file;

    // Compress the text file
    compress_text_file(input_txt, compressed_txt);
    cout << "File compressed successfully." << endl;

    // Decompress the text file
    decompress_text_file(compressed_txt, decompressed_txt);
    cout << "File decompressed successfully." << endl;

    return 0;
}



// Function to compress a text file using RLE
void compress_text_file(const string& input_file, const string& output_file){
    ifstream in(input_file);
    ofstream out(output_file);

    if(!in || !out){
        cerr << "Error opening file!" << endl;
        return;
    }

    char current_char, previous_char;
    int count = 1;

    // Read the first character
    in.get(previous_char);

    // Iterate through the file
    while(in.get(current_char)){
        if(current_char == previous_char){
            count++;
        } 
        else{
            out << previous_char << count;  // Write character and count
            previous_char = current_char;   // Update the current character
            count = 1;
        }
    }

    // Write the last character and count
    out << previous_char << count;

    in.close();
    out.close();
}



// Function to decompress a compressed text file using RLE
void decompress_text_file(const string& input_file, const string& output_file){
    ifstream in(input_file);
    ofstream out(output_file);

    if(!in || !out){
        cerr << "Error opening file!" << endl;
        return;
    }

    char ch;
    int count;

    // Read character and count pair from input
    while(in >> ch >> count){
        out << string(count, ch);  // Write character 'count' times
    }

    in.close();
    out.close();
}
