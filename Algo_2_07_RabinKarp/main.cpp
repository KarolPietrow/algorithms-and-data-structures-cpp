#include <iostream>
#include <vector>
using namespace std;

long long modular_pow(long long base, long long exponent, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        exponent = exponent >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Rabin–Karp: znajduje pierwsze dwa wystąpienia wzorca X w tekście S
vector<int> find_occurrences(const vector<int>& S, const vector<int>& X, int V, int P) {
    vector<int> occ;
    if (P == 0 || P > V) {
        // Jeśli wzorzec jest pusty lub dłuższy niż tekst - brak wystąpień
        return occ;
    }

    // Podwójne hashowanie dla minimalizacji kolizji
    long long base1 = 101, mod1 = 1000000007;
    long long base2 = 257, mod2 = 1000000009;

    // Oblicz hash wzorca X
    long long hash_x1 = 0, hash_x2 = 0;
    for (int i = 0; i < P; ++i) {
        hash_x1 = (hash_x1 * base1 + X[i]) % mod1;
        hash_x2 = (hash_x2 * base2 + X[i]) % mod2;
    }

    // Wartości base^(P-1) do odjęcia z początkowego znaku podczas przesuwania okna
    long long pow_base1 = modular_pow(base1, P - 1, mod1);
    long long pow_base2 = modular_pow(base2, P - 1, mod2);

    // Oblicz hash pierwszego okna S[0..P-1]
    long long current_hash1 = 0, current_hash2 = 0;
    for (int i = 0; i < P; ++i) {
        current_hash1 = (current_hash1 * base1 + S[i]) % mod1;
        current_hash2 = (current_hash2 * base2 + S[i]) % mod2;
    }

    // Sprawdź pierwsze okno
    if (current_hash1 == hash_x1 && current_hash2 == hash_x2) {
        bool match = true;
        for (int j = 0; j < P; ++j) {
            if (S[j] != X[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            occ.push_back(0);
            if (occ.size() == 2) return occ;
        }
    }

    // Przesuwamy okno po S od i=P do V-1
    for (int i = P; i < V; ++i) {
        // Usuń stary pierwszy element i dodaj nowy na końcu okna
        current_hash1 = ((current_hash1 - S[i - P] * pow_base1 % mod1) + mod1) % mod1;
        current_hash1 = (current_hash1 * base1 + S[i]) % mod1;

        current_hash2 = ((current_hash2 - S[i - P] * pow_base2 % mod2) + mod2) % mod2;
        current_hash2 = (current_hash2 * base2 + S[i]) % mod2;

        int pos = i - P + 1; // Indeks początku obecnego okna
        if (current_hash1 == hash_x1 && current_hash2 == hash_x2) {
            // Znów sprawdźzawartość okna
            bool match = true;
            for (int j = 0; j < P; ++j) {
                if (S[pos + j] != X[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                occ.push_back(pos);
                if (occ.size() >= 2) return occ;
            }
        }
    }

    return occ;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int V;
    cin >> V;
    vector<int> S(V);
    for (int i = 0; i < V; ++i) {
        cin >> S[i];
    }

    int P;
    cin >> P;
    vector<int> X(P);
    for (int i = 0; i < P; ++i) {
        cin >> X[i];
    }

    if (P == 0 || P > V) {
        cout << 0 << '\n';
        return 0;
    }

    vector<int> occurrences = find_occurrences(S, X, V, P);
    if (occurrences.size() < 2) {
        cout << 0 << '\n';
        return 0;
    }

    long long a = occurrences[0];
    long long b = occurrences[1];
    while (true) {
        long long c = a + b;
        if (c >= V) {
            cout << (c - V) << '\n';
            return 0;
        }
        a = b;
        b = c;
    }

    return 0;
}