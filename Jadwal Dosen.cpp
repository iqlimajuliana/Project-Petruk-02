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

// Merge Sort untuk prioritas matkul
void merge(vector<MataKuliah>& matkul, int left, int mid, int right) {
    vector<MataKuliah> L(matkul.begin() + left, matkul.begin() + mid + 1);
    vector<MataKuliah> R(matkul.begin() + mid + 1, matkul.begin() + right + 1);
    int i = 0, j = 0, k = left;
    while (i < L.size() && j < R.size()) {
        if (getPrioritas(L[i].jenis) >= getPrioritas(R[j].jenis)) {
            matkul[k++] = L[i++];
        } else {
            matkul[k++] = R[j++];
        }
    }
    while (i < L.size()) matkul[k++] = L[i++];
    while (j < R.size()) matkul[k++] = R[j++];
}

void mergeSort(vector<MataKuliah>& matkul, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(matkul, left, mid);
        mergeSort(matkul, mid + 1, right);
        merge(matkul, left, mid, right);
    }
}

// Kapasitas Matkul (conquer)
vector<MataKuliah> knapsackDC(int i, int kapasitas, const vector<MataKuliah>& matkul, vector<vector<int>>& memo) {
    if (i < 0 || kapasitas <= 0) return {};
    if (memo[i][kapasitas] != -1) return {};

    if (matkul[i].sks > kapasitas) {
        memo[i][kapasitas] = 0;
        return knapsackDC(i - 1, kapasitas, matkul, memo);
    }
    
    vector<MataKuliah> ambil = knapsackDC(i - 1, kapasitas - matkul[i].sks, matkul, memo);
    ambil.push_back(matkul[i]);

    vector<MataKuliah> skip = knapsackDC(i - 1, kapasitas, matkul, memo);

    int totalSKSAmbil = 0, totalSKSSkip = 0;
    for (size_t k = 0; k < ambil.size(); ++k) totalSKSAmbil += ambil[k].sks;
    for (size_t k = 0; k < skip.size(); ++k) totalSKSSkip += skip[k].sks;

    memo[i][kapasitas] = max(totalSKSAmbil, totalSKSSkip);
    return (totalSKSAmbil > totalSKSSkip) ? ambil : skip;
}
/ Fungsi yang menyimpan hasil ke dalam file 
void simpanJadwal(const map<string, vector<MataKuliah>>& jadwalDosen, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "Gagal membuka file output!" << endl;
        return;
    }
out << "======= JADWAL MENGAJAR DOSEN =======\n\n";
    out << left << setw(20) << "Mata Kuliah" 
        << setw(5)  << "SKS" 
        << setw(10) << "Hari" 
        << setw(15) << "Jam" 
        << setw(10) << "Kelas" 
        << setw(15) << "Ruang" << "\n";
    out << string(75, '-') << "\n";

for (const auto& [dosen, jadwal] : jadwalDosen) {
        out << "\nDosen: " << dosen << "\n";
        for (const auto& mk : jadwal) {
            out << " - " << left << setw(18) << mk.nama 
                << setw(5)  << mk.sks 
                << setw(10) << mk.hari 
                << setw(15) << mk.jam 
                << setw(10) << mk.kelas 
                << setw(15) << mk.ruang << "\n";
        }
    }
out.close();
    cout << "\n? Jadwal berhasil disimpan ke '" << filename << "'\n";
}
int main() {
    int n, batasSKS;
    cout << "Masukkan jumlah mata kuliah: "; cin >> n;
    cout << "Masukkan batas maksimal SKS per dosen: "; cin >> batasSKS;
    cin.ignore();

    vector<MataKuliah> daftarMatkul;
    map<string, vector<MataKuliah>> calonMatkulPerDosen;
    map<string, vector<MataKuliah>> jadwalDosen;
    map<string, int> totalSKSDosen;

    for (int i = 0; i < n; i++) {
        MataKuliah mk;
        cout << "\nMata kuliah ke-" << i + 1 << ":\n";
        cout << "Nama Mata Kuliah: "; getline(cin, mk.nama);
        cout << "Jumlah SKS: "; cin >> mk.sks; cin.ignore();
        cout << "Jenis (Wajib/Pilihan): "; getline(cin, mk.jenis);
        cout << "Dosen Pengampu (pisahkan dengan koma): ";
        string line; getline(cin, line);
        stringstream ss(line);
        string dosen;
        while (getline(ss, dosen, ',')) {
            dosen.erase(0, dosen.find_first_not_of(" "));
            mk.dosenPengampu.push_back(dosen);
        }
        cout << "Hari: "; getline(cin, mk.hari);
        cout << "Jam (contoh: 08:00-10:00): "; getline(cin, mk.jam);
        cout << "Kelas: "; getline(cin, mk.kelas);
        cout << "Ruang: "; getline(cin, mk.ruang);
        daftarMatkul.push_back(mk);
    }

    // Urutkan berdasarkan prioritas wajib > pilihan
    mergeSort(daftarMatkul, 0, daftarMatkul.size() - 1);

    // Masukkan ke struktur per dosen
    for (const auto& mk : daftarMatkul) {
        for (const string& dsn : mk.dosenPengampu)
            calonMatkulPerDosen[dsn].push_back(mk);
    }

    // Jalankan knapsack per dosen
    for (auto it = calonMatkulPerDosen.begin(); it != calonMatkulPerDosen.end(); ++it) {
        const string& dosen = it->first;
        const vector<MataKuliah>& matkulList = it->second;
        int m = matkulList.size();
        vector<vector<int>> memo(m, vector<int>(batasSKS + 1, -1));
        vector<MataKuliah> hasil = knapsackDC(m - 1, batasSKS, matkulList, memo);
        jadwalDosen[dosen] = hasil;
        for (const auto& mk : hasil)
            totalSKSDosen[dosen] += mk.sks;
    }

    // Contoh penggunaan stack dan queue
    stack<string> logStack;
    queue<string> logQueue;
    for (const auto& [dosen, listMatkul] : jadwalDosen) {
        logStack.push(dosen);
        logQueue.push(dosen);
    }

    cout << "\nLog Stack (dosen terakhir yang diproses): " << logStack.top() << endl;
    cout << "Log Queue (dosen pertama yang diproses): " << logQueue.front() << endl;

    simpanJadwal(jadwalDosen, "jadwal_dosen.txt");
    return 0;
}

