#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

// Sprawdza, czy liczba ma parzystą liczbę unikalnych dzielników pierwszych
bool hasEvenPrime(int n) {
    int count = 0;

    // Dzielnik 2
    if (n % 2 == 0) {
        count++;
        while (n % 2 == 0) {
            n /= 2;
        }
    }
    // Dzielniki nieparzyste od 3 w górę
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            count++;
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    // Jeśli pozostało coś >1 → dzielnik pierwszy
    if (n > 1) {
        count++;
    }
    // Zwracamy true gdy liczba unikalnych dzielników jest parzysta
    return count % 2 == 0;
}

int select_median(std::vector<int>& arr, int k) {
    // Małe tablice (<=5 elementów): sortujemy std::sort
    if (arr.size() <= 5) {
        std::sort(arr.begin(), arr.end());
        return arr[k];
    }
    // Dzielenie na grupy po 5, szukanie median
    std::vector<int> medians;
    for (size_t  i =0; i < arr.size(); i+= 5) {
        size_t end = std::min(i+5, arr.size());
        // Kopiujemy grupę
        std::vector<int> group(arr.begin() + i, arr.begin() + end);
        std::sort(group.begin(), group.end());
        // Dodajemy środkowy element jako medianę
        medians.push_back(group[group.size() / 2]);
    }

    // Pivot - mediana median
    int pivot = select_median(medians, medians.size() / 2);

    // Podział względem pivot
    std::vector<int> lows;
    std::vector<int> highs;
    std::vector<int> pivots;
    for (int num : arr) {
        if (num < pivot) {
            lows.push_back(num);
        } else if (num > pivot) {
            highs.push_back(num);
        } else {
            pivots.push_back(num);
        }
    }

    // Rekurencyjny wybór opowiedniego podzbioru
    if (k < static_cast<int>(lows.size())) {
        return select_median(lows, k);
    }
    if (k < static_cast<int>(lows.size() + pivots.size())) {
        return pivot; // k-ty element to pivot
    }
    // W przeciwnym razie szukamy w highs przesuniętego indeksu
    return select_median(highs, k - lows.size() - pivots.size());
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int t; // Liczba zestawów danych
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        int n; // Liczba wartości liczbowych
        std::cin >> n;
        std::vector<int> accepted_numbers; // Liczby uważane za stabilne (akceptowane)
        for (int j = 0; j < n; j++) {
            int number;
            std::cin >> number;
            if (hasEvenPrime(number)) {
                accepted_numbers.push_back(number);
            }
        }
        int k; // Pozycja szukanej wartości
        std::cin >> k;
        // Sprawdzamy czy jest wystarczająco stabillnych liczb
        if (accepted_numbers.size()-1 < k) {
            std::cout << "BRAK DANYCH\n";
        } else {
            // Wywołujemy algorytm selekcji, by znaleźć k-ty najmniejszy
            int element = select_median(accepted_numbers, k);
            std::cout << element << "\n";
        }
    }
    return 0;
}