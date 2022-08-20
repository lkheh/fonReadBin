/*
    Usage: fonReadbin <asm_bin_file> [count_per_line=16]
    Read binary data from the file and output the hex string for C/C++
    Version: 1.0
    Email: fon909@outlook.com
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

typedef std::vector<unsigned char> BinaryArray;

void usage();
bool read_binary(ifstream&, BinaryArray&);
unsigned output_hex(BinaryArray const &, unsigned const);

int main(int argc, char **argv) {
    if(argc < 2) {
        usage();
        return -1;
    }

    ifstream fin(argv[1], ios_base::binary);
    if(!fin) {
        cerr << "failed to open file \"" << argv[1]
             << "\".\n";
        return -1;  
    }

    BinaryArray array;
    if(!read_binary(fin, array)) {
        cerr << "failed to parsed file \"" << argv[1] 
             << "\".\n";
        return -1;
    }

    unsigned count_per_line = 16;
    if(argc >= 3) count_per_line = atoi(argv[2]);
    cout << "//Reading \"" << argv[1] << "\"\n"
         << "//Size: " << array.size() << " bytes\n"
         << "//Count per line: " << count_per_line 
         << "\n";
    unsigned null_count = output_hex(array, count_per_line);
    cout << "//NULL count: " << null_count << '\n';
}

unsigned output_hex(BinaryArray const &carr, unsigned const cpl) {
    unsigned null = 0;
    cout << "char code[] = \n\"";
    for(size_t i = 1; i <= carr.size(); ++i) {
        cout << "\\x" << hex << setw(2) 
             << setfill('0') << (unsigned)(carr[i-1]);
        if(!(i % cpl)) {
            cout << "\"\n";
            if(i < carr.size()) cout << '\"';
        }
        if(!(carr[i-1])) ++null;        
    }

    if(carr.size() % cpl) cout << '\"';
    cout << ";\n";
    return null;
}

bool read_binary(ifstream& fin, BinaryArray& arr) {
    try {
        unsigned file_length;

        fin.seekg(0, ios::end);
        file_length = fin.tellg();
        fin.seekg(0, ios::beg);

        arr.resize(file_length);
        char *mem_buf = new char [file_length];
        fin.read(mem_buf, file_length); 
        copy(mem_buf, mem_buf+file_length, arr.begin());
        delete [] mem_buf;
    } catch(...) {return false;}
    return true;
}

void usage() {
    cout << "Usage: fonReadbin <asm_bin_file> [count_per_line=16]\n"
         << "Read binary data from the file and output the hex string for C/C++\n"
         << "Version 1.0\n"
         << "Email: fon909@outlook.com\n"
         << "Example: ./fonReadBin shellcode.asm 32";
}
