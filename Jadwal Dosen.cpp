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

