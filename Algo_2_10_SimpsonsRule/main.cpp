#include <iostream>
#include <vector>
#include <cmath>

double butelka(double cm);

double butelka(double cm) {
     return 10.0 - (cm - 2.0) * (cm - 2.0);
}

double calculateVolume(double j) {
    const int n = 50; // Parzysta liczba przedziałów
    double h = j / n;
    double sum = pow(butelka(0), 2) + pow(butelka(j), 2);
    for (int i = 1; i < n; ++i) {
        double x = i * h;
        double fx = pow(butelka(x), 2);
        sum += (i % 2 == 1) ? 4 * fx : 2 * fx;
    }
    sum *= h / 3.0;
    return M_PI * sum;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int x, y, z; // Rozmiar akwarium w cm
    std::cin >> x >> y >> z;
    double p; // Stopień wypełnienia akwarium wodą
    std::cin >> p;

    int n; // Liczba butelek
    std::cin >> n;
    std::vector<double> levels(n);
    for (int i = 0; i < n; i++) {
        std::cin >> levels[i];
    }
    double V_max = x * y * z;
    double V_initial = p* V_max;
    double V_current = V_initial;

    for (int i = 0; i < n; i++) {
        double V_bottle = calculateVolume(levels[i]);
        V_current += V_bottle;

        if (V_current > V_max) {
            std::cout << (i+1) << "\n";
            return 0;
        }
    }
    std::cout << "NIE\n";
    return 0;
}
