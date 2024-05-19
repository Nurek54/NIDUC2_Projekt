#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "Utils/DataGenerator.h"

using namespace std;

// Funkcja do znajdowania lub tworzenia grupy dla głosu
int znajdzLubUtworzGrupe(vector<vector<double>>& grupy, double glos, double prog) {
    for (size_t i = 0; i < grupy.size(); ++i) {
        if (!grupy[i].empty() && fabs(grupy[i][0] - glos) <= prog) {
            return i;
        }
    }
    vector<double> nowaGrupa;
    nowaGrupa.push_back(glos);
    grupy.push_back(nowaGrupa);
    return grupy.size() - 1;
}

// Nieprecyzyjne głosowanie większościowe i pluralistyczne
double votingAlgorithm(const vector<double>& glosy, double prog, bool czyWiekszosc) {
    vector<vector<double>> grupy;
    for (double glos : glosy) {
        int indeksGrupy = znajdzLubUtworzGrupe(grupy, glos, prog);
        grupy[indeksGrupy].push_back(glos);
    }

    int indeksGrupyKandydata = -1;
    size_t maksRozmiarGrupy = 0;
    for (size_t i = 0; i < grupy.size(); ++i) {
        if (grupy[i].size() > maksRozmiarGrupy) {
            maksRozmiarGrupy = grupy[i].size();
            indeksGrupyKandydata = i;
        }
    }

    if (czyWiekszosc && maksRozmiarGrupy <= glosy.size() / 2) {
        return -1; // Brak kandydata większościowego
    }
    return grupy[indeksGrupyKandydata][0]; // Zwróć jednego kandydata losowo z największej grupy
}

// Weighted Average Voting
double weightedAverageVoting(const vector<double>& glosy, const vector<double>& wagi) {
    double suma = 0.0;
    double sumaWag = 0.0;
    for (size_t i = 0; i < glosy.size(); ++i) {
        suma += glosy[i] * wagi[i];
        sumaWag += wagi[i];
    }
    return suma / sumaWag;
}

// Median Voting
double medianVoting(const vector<double>& glosy) {
    vector<double> posortowaneGlosy = glosy;
    sort(posortowaneGlosy.begin(), posortowaneGlosy.end());
    size_t n = posortowaneGlosy.size();
    if (n % 2 == 0) {
        return (posortowaneGlosy[n / 2 - 1] + posortowaneGlosy[n / 2]) / 2.0;
    } else {
        return posortowaneGlosy[n / 2];
    }
}

// Linear Voting
double linearVoting(const vector<double>& glosy) {
    double suma = 0.0;
    for (double glos : glosy) {
        suma += glos;
    }
    return suma / glosy.size();
}

// First-Order Voting
double firstOrderVoting(const vector<double>& glosy) {
    return glosy.front();
}

void generateCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (const auto& row : data) {
        for (const auto& cell : row) {
            outputFile << cell << ", ";
        }
        outputFile << "\n";
    }

    outputFile.close();
}

int main() {
    vector<double> glosy = {10.0, 20.0, 15.0, 15.5, 14.0};
    double freq = 2, dur = 1, sr = 20, ror = 10;
    vector<double> data1 = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, ror);
    vector<double> data2 = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, ror);
    vector<double> data3 = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, ror);
    for (const auto& element : data1) {
        std::cout << element << ' ';
    }
    cout << endl;
    for (const auto& element : data2) {
        std::cout << element << ' ';
    }
    cout << endl;
    for (const auto& element : data3) {
        std::cout << element << ' ';
    }
    cout << endl;

    double prog = 0.5;  // Próg dla głosowania z niedokładnymi danymi

    cout << "Nieprecyzyjne glosowanie wiekszosciowe: ";
    std::vector<std::vector<std::string>> toWrite;
    std::vector<std::string> col = {"lp","selected"};
    toWrite.push_back(col);
    for(int i = 0; i < data1.size(); i++){
        vector<double> temp = {data1[i], data2[i], data3[i]};
        double res = votingAlgorithm(temp, prog, true);
        cout << res << " ";
        std::vector<std::string> dat = {std::to_string(i),std::to_string(res)};
        toWrite.push_back(dat);
    }
    generateCSV("wiekszosciowe.csv",toWrite);
    cout << endl;
    cout << "Nieprecyzyjne glosowanie pluralistyczne: " << votingAlgorithm(glosy, prog, false) << endl;

    vector<double> wagi = {0.1, 0.5, 0.2, 0.1, 0.1};
    cout << "Weighted Average Voting: " << weightedAverageVoting(glosy, wagi) << endl;
    cout << "Median Voting: " << medianVoting(glosy) << endl;
    cout << "Linear Voting: " << linearVoting(glosy) << endl;
    cout << "First-Order Voting: " << firstOrderVoting(glosy) << endl;

    return 0;
}
