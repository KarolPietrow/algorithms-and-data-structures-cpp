#include <iostream>
#include <climits>

unsigned int n;
int minCost = INT_MAX;
int cost[35][35];
bool rowTaken[35];
int solution[35];
int bestSolution[35];

void findBestPlace(int column, int currentCost) {
    if (currentCost >= minCost) {
        // Jeżeli już przekroczyliśmy dotychczasowy najlepszy koszt, nie ma sensu dalej szukać
        return;
    }

    if (column == n) {
        // Jeżeli przydzieliliśmy już wszystkie kolumny (column == n),
        // sprawdzamy, czy znalezione rozwiązanie jest lepsze od dotychczasowego najlepszego
        if (currentCost < minCost) {
            minCost = currentCost;
            for (int i = 0; i < n; i++) {
                bestSolution[i] = solution[i];
            }
        }
        return;
    }
    // Próbujemy każdą możliwą grządkę (wiersz), która jeszcze nie została wykorzystana
    for (int row = 0; row < n; row++) {
        if (!rowTaken[row]) {
            solution[column] = row;
            rowTaken[row] = true;
            findBestPlace(column+1, currentCost+cost[row][column]);
            rowTaken[row] = false;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> cost[i][j];
        }
        rowTaken[i] = false;
    }


    findBestPlace(0,0);

    for (int i = 0; i < n; i++) {
        std::cout << bestSolution[i] << " ";
    }
    return 0;
}