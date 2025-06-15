#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstring> // untuk c-string

using namespace std;

struct MataKuliah {
    string nama;
    int sks;
    string jenis; 
    vector<string> dosenPengampu;
    string hari, jam, kelas, ruang;
};

