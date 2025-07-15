#include <iostream>

int calculate(int** board, int n) {
    // p[0] oraz p[1] przechowują stany DP dla k i k-1, żeby zaoszczędzić pamięć
    int*** p = new int**[2];
    for (int i=0; i<2; i++) {
        p[i] = new int*[n];
        for (int j=0; j<n; j++) {
            p[i][j] = new int[n];
            for (int k=0; k<n; k++) {
                p[i][j][k] = -1;
            }
        }
    }
    // Stan początkowy: obie osoby na (0,0) zbierają board[0][0]
    p[0][0][0] = board[0][0];

    int maxSteps = 2*(n-1)+1;

    // Iterujemy po kolejnych krokach k = liczba ruchów obu podróżników
    for (int k=1; k<maxSteps; k++) {
        // x1, x2 to wiersze pierwszej i drugiej osoby
        for (int x1 = std::max(0, k-(n-1)); x1 <= std::min (n-1, k); x1++) {
            for (int x2 = std::max(0, k-(n-1)); x2 <= std::min (n-1, k); x2++) {
                int y1 = k-x1; // Kolumna osoby 1
                int y2 = k-x2; // Kolumna osoby 2

                int counter = -1;

                // Cztery możliwe przejścia z poprzedniego kroku
                if (x1 > 0) {
                    counter = std::max(counter, p[(k - 1) % 2][x1 - 1][x2]);
                }
                if (x2 > 0) {
                    counter = std::max(counter, p[(k - 1) % 2][x1][x2 - 1]);
                }
                if (x1 > 0 && x2 > 0) {
                    counter = std::max(counter, p[(k - 1) % 2][x1 - 1][x2 - 1]);
                }
                if (y1 > 0 && y2 > 0) {
                    counter = std::max(counter, p[(k - 1) % 2][x1][x2]);
                }

                counter += board[x1][y1];
                if (x1 != x2) {
                    // Jeśli dwie osoby nie są na tym samym polu, zlicz drugi artefakt
                    counter += board[x2][y2];
                }

                // Zapisujemy wynik do aktualnej warstwy DP
                p[k % 2][x1][x2] = counter;
            }
        }
    }
    int result = p[(maxSteps - 1) % 2][n - 1][n - 1];

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < n; j++) {
            delete[] p[i][j];
        }
        delete[] p[i];
    }
    delete[] p;
    return result;
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    int** board = new int*[n];
    for (int i = 0; i < n; i++) {
        board[i] = new int[n];
        for (int j = 0; j < n; j++) {
            std::cin >> board[i][j];
        }
    }

    std::cout << calculate(board, n) << std::endl;

    for (int i=0; i<n; i++) {
        delete[] board[i];
    }
    delete[] board;
    return 0;
}