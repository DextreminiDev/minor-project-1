// RLE

#include <bits/stdc++.h>
#include <chrono> // For measuring time

using namespace std;
using namespace std::chrono;

void compress_text_file(const string& input_file, const string& output_file);
void decompress_text_file(const string& input_file, const string& output_file);

int main(void) {
    string path = "D:/Codes/minor-project-1/algorithms/";
    string input_file = "words_alpha.txt";

    string input_txt = path + input_file;
    string compressed_txt = path + "compressed_" + input_file;
    string decompressed_txt = path + "decompressed_" + input_file;

    // Measure compression time
    auto start_compress = high_resolution_clock::now();
    compress_text_file(input_txt, compressed_txt);
    auto end_compress = high_resolution_clock::now();
    cout << "File compressed successfully." << endl;

    auto compress_duration = duration_cast<milliseconds>(end_compress - start_compress);
    cout << "Compression time: " << compress_duration.count() << " ms" << endl;

    // Measure decompression time
    auto start_decompress = high_resolution_clock::now();
    decompress_text_file(compressed_txt, decompressed_txt);
    auto end_decompress = high_resolution_clock::now();
    cout << "File decompressed successfully." << endl;

    auto decompress_duration = duration_cast<milliseconds>(end_decompress - start_decompress);
    cout << "Decompression time: " << decompress_duration.count() << " ms" << endl;

    return 0;
}

// Function to compress a text file using RLE
void compress_text_file(const string& input_file, const string& output_file) {
    ifstream in(input_file);
    ofstream out(output_file);

    if (!in || !out) {
        cerr << "Error opening file!" << endl;
        return;
    }

    char current_char, previous_char;
    int count = 1;

    // Read the first character
    in.get(previous_char);

    // Iterate through the file
    while (in.get(current_char)) {
        if (current_char == previous_char) {
            count++;
        } else {
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
void decompress_text_file(const string& input_file, const string& output_file) {
    ifstream in(input_file);
    ofstream out(output_file);

    if (!in || !out) {
        cerr << "Error opening file!" << endl;
        return;
    }

    char ch;
    int count;

    // Read character and count pair from input
    while (in >> ch >> count) {
        out << string(count, ch);  // Write character 'count' times
    }

    in.close();
    out.close();
}