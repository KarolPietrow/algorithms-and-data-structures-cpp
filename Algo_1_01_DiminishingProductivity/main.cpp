#include <algorithm>
#include <iostream>

long calculateCollectedMaterial(unsigned int n, long d_i, long* C, long* K, long* maxDay) {
    long totalCollected=0;
    for (int i=0; i<n; i++) { // i - CHŁOP
        // Ile dni dany chłop faktycznie pracuje: nie więcej niż d_i,
        // ani więcej niż (maxDay[i] + 1) dni przed wyczerpaniem wydajności.
        long actualDays = std::min(d_i, maxDay[i] + 1);
        if (actualDays > 0) {
            // Suma wydajności od dnia 1 do actualDays:
            // (actualDays) * (2*C[i] - (actualDays - 1)*K[i]) / 2
            totalCollected += (actualDays * (2 * C[i] - (actualDays - 1) * K[i])) / 2;
        }
    }
    return totalCollected;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    unsigned int t; // Liczba danych testowych
    std::cin >> t;

    for (int i=0; i<t; i++) {
        unsigned int n; // Liczba chłopów do pomocy
        unsigned long d; // Liczba zapytań o dni
        unsigned long m; // Liczba wymaganych ton blendy

        std::cin >> n >> d >> m;

        long* C = new long[n]; // Wydajność
        long* K = new long[n]; // Redukcja wydajności
        long* maxDay = new long[n]; // Maksimum dni pracy przed wyczerpaniem

        for (int j=0; j<n; j++) {
            std::cin >> C[j] >> K[j];
            // Chłop przestaje pracować, gdy C[i] - x*K[i] <= 0
            // czyli maxDay = floor(C[i] / K[i])
            maxDay[j] = C[j] / K[j];
        }

        unsigned int collectedRequiredAmount=0;

        // Dla każdej d_i obliczamy łączną produkcję i sprawdzamy, czy ≥ m
        for (int j=0; j<d; j++) {
            long d_i;
            std::cin >> d_i;
            long collectedMaterial = calculateCollectedMaterial(n, d_i, C, K, maxDay);
            std::cout << collectedMaterial << " ";
            if (collectedMaterial >= m) {
                collectedRequiredAmount++;
            }
        }

        std::cout << "\n" << collectedRequiredAmount << "\n";


        delete[] C;
        delete[] K;
        delete[] maxDay;
    }
    return 0;
}