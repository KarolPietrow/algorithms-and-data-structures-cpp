#include <iostream>
#include <vector>

bool match(char pattern, char c) {
    if (pattern == 'a') { // Sprawdzenie, czy znak jest małą literą
        return (c >= 'a' && c <= 'z');
    }
    if (pattern == 'A') { // Sprawdzenie, czy znak jest wielką literą
        return (c >= 'A' && c <= 'Z');
    }
    if (pattern == '.') { // Sprawdzenie, czy znak jest znakiem specjalnym
        return (c == '.' || c == '?' || c == '!' || c == ',' || c == ';' || c == ':' || c == '-');
    }
    if (pattern == '0') { // Sprawdzenie, czy znak jest cyfrą
        return isdigit(c);
    }
    if (pattern == ' ') { // Sprawdzamy, czy znak jest spacją
        return c == ' ';
    }
    return false;
}

std::vector<int> computePrefix(const std::string& pattern) {
    int m = pattern.size();
    std::vector<int> pi(m, 0);
    int k = 0;

    for (int i = 1; i < m; i++) {
        while (k > 0 && pattern[i] != pattern[k]) {
            k = pi[k-1];
        }
        if (pattern[i] == pattern[k]) {
            k++;
        }
        pi[i] = k;
    }
    return pi;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    std::string pattern; // Szablon poszukiwanej frazy
    std::getline(std::cin, pattern);
    std::vector<int> pi = computePrefix(pattern);

    int j = 0;
    char c;


    while (std::cin.get(c) && c != '\n') {
        while (j > 0 && !match(pattern[j], c)) {
            j = pi[j-1];
        }
        if (match(pattern[j], c)) {
            j++;
        }
        if (j == pattern.size()) {
            std::cout << "T\n";
            return 0;
        }

    }
    std::cout << "N\n";
    return 0;
}