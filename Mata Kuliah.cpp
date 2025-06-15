#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct MataKuliah {
    string nama;
    int sks;
    string jenis;
    vector<string> dosenPengampu;
    string hari, jam, kelas, ruang;
};
