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
int main() {
    int n;
    cout << "Masukkan jumlah mata kuliah: ";
    cin >> n;
    cin.ignore();

    vector<MataKuliah> daftarMatkul;

    for (int i = 0; i < n; i++) {
        MataKuliah mk;
        cout << "\nMata kuliah ke-" << i + 1 << ":\n";
        cout << "Nama Mata Kuliah: ";
        getline(cin, mk.nama);
        cout << "Jumlah SKS: ";
        cin >> mk.sks;
        cin.ignore();
        cout << "Jenis (Wajib/Pilihan): ";
        getline(cin, mk.jenis);
        cout << "Dosen Pengampu (pisahkan dengan koma): ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string dosen;
        while (getline(ss, dosen, ',')) {
            dosen.erase(0, dosen.find_first_not_of(" "));
            mk.dosenPengampu.push_back(dosen);
        }
        
