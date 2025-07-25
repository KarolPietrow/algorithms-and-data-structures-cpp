#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>

int Loptr(int HH, int mm);

int Loptr(int HH, int mm){
    if(HH >= 8 && HH < 15) return 0;
    else if(HH >= 15 && HH < 22) return 1;
    return 2;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    std::string t1, t2; // Przedziały czasowe
    int target; // Numer wymiaru/epoki
    std::cin >> t1 >> t2 >> target;
    int HH1 = stoi(t1.substr(0,2));
    int mm1 = stoi(t1.substr(3,2));
    int HH2 = stoi(t2.substr(0,2));
    int mm2 = stoi(t2.substr(3,2));

    int start = HH1 * 60 + mm1;
    int end = HH2 * 60 + mm2;

    const int N = 1000000; // Liczba losowań
    std::mt19937_64 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(start, end - 1); // Rozkład jednostajny

    int hits = 0; // Liczba trafień
    for (int i = 0; i < N; ++i) {
        int m = dist(rng);
        int h = m / 60;
        int mm = m % 60;
        if (Loptr(h, mm) == target) {
            hits++;
        }
    }

    double prob = static_cast<double>(hits) / N;
    std::cout << std::fixed << std::setprecision(2) << prob << "\n";
    return 0;
}
