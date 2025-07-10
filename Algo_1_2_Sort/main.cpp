#include <iostream>

void swap(unsigned long &a, unsigned long &b) {
    unsigned int temp=a;
    a=b;
    b=temp;
}

void swap(std::string &a, std::string &b) {
    std::string temp=a;
    a=b;
    b=temp;
}

// void bubbleSort(std::string id[], unsigned long m[], unsigned int n) {
//     for (int i=0; i<n-1; i++) {
//         for (int j=0; j<n-i-1; j++) {
//             if (m[j] > m[j+1]) {
//                 swap(m[j], m[j+1]);
//                 swap(id[j], id[j+1]);
//             }
//         }
//     }
// }

void insertionSort(std::string id[], unsigned long m[], unsigned int n) {
    for (int i=1; i<n; i++) {
        unsigned long key = m[i];
        std::string keyId = id[i];
        int j=i-1;
        // Przesuwamy większe elementy o jedną pozycję w prawo
        while (j>=0 && m[j]>key) {
            m[j+1]=m[j];
            id[j+1]=id[j];
            j-=1;
        }
        // Wstawiamy wybrany element na właściwe miejsce
        m[j+1]=key;
        id[j+1]=keyId;
    }
}

unsigned long calculateEnergy(unsigned int n, unsigned long m[]) {
    unsigned long totalEnergyCost=0;
    unsigned long totalMass=0;
    for (int i=0; i<n; i++) {
        // Dodajemy koszt wstawienia kolejnego artefaktu
        unsigned long newEnergy=totalMass+m[i];
        totalEnergyCost+=newEnergy;
        // Aktualizujemy masę sejfu
        totalMass+=m[i];
    }
    return totalEnergyCost;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    unsigned int t; // Liczba zestawów danych
    std::cin >> t;

    for (int i=0; i<t; i++) {
        unsigned int n; // Liczba artefaktów w zestawie
        std::cin >> n;

        // std::string id[n];
        // unsigned long m[n];

        std::string* id = new std::string[n]; // Identyfikator artefaktu
        unsigned long* m = new unsigned long[n]; // Masa artefaktu w kilogramach

        for (int j=0; j<n; j++) {
            std::cin >> id[j] >> m[j];
        }

        insertionSort(id, m, n);
        unsigned long minEnergy = calculateEnergy(n,m); // Obliczamy minimalny koszt energetyczny


        std::cout << minEnergy << std::endl;
        for (int k=0; k<n; k++) {
            std::cout << id[k] << " ";
        }
        std::cout << "\n";

        delete[] id;
        delete[] m;
    }

    return 0;
}