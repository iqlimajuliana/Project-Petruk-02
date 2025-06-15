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

int getPrioritas(const string& jenis) {
    return (jenis == "wajib" || jenis == "Wajib") ? 2 : 1;
}

bool comparePrioritas(const MataKuliah& a, const MataKuliah& b) {
    return getPrioritas(a.jenis) > getPrioritas(b.jenis);
}
