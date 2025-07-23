#include <iostream>
#include <vector>
#include <tuple>

bool can_capture(int r1, int c1, int f1, int r2, int c2, bool is_black) {
    int dr = r2 - r1; // Różnica wierszy (ruch pionowy)
    int dc = (is_black) ? (c2 - c1) : (c1 - c2); // Różnica kolumn (ruch poziomy)

    if (f1 == 1) { // Typ 1 - bije:
        // - pole oddalone o 3 jednostki w linii prostej przed sobą
        if (dr == 0 && dc == 3) return true;
        // - pole oddalone o 2 jednostki w linii prostej przed sobą i o jedną jednostkę w prawo bądź lewo
        if (abs(dr) == 1 && dc == 2) return true;
    } else if (f1 == 2) { // Typ 2 - bije:
        // - pole oddalone o 1 jednostkę w linii prostej przed sobą
        if (dr == 0 && dc == 1) return true;
        // - pole oddalone o 1 jednostkę w prawo bądź lewo
        if (abs(dr) == 1 && dc == 0) return true;
    } else if (f1 == 3) { // Typ 3 - bije:
        // - pole oddalone o 1 jednostkę w linii prostej przed sobą
        if (dr == 0 && dc == 1) return true;
        // - pole oddalone o 2 jednostki w line prostej przed sobą i o jedną jednostkę w prawo bądź lewo
        if (abs(dr) == 1 && dc == 2) return true;
    }
    return false; // Żaden warunek nie jest spełniony - brak bicia
}

bool dfs(int u, std::vector<std::vector<int>>& adj, std::vector<int>& match, std::vector<bool>& visited) {
    for (int v : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            // Jeśli v nie jest jeszcze skojarzone, lub jeśli można znaleźć inną ścieżkę
            if (match[v] == -1 || dfs(match[v], adj, match, visited)) {
                match[v] = u;
                return true;
            }
        }
    }
    return false; // Nie znaleziono ścieżki
}

int max_bipartite_matching(int num_u, int num_v, std::vector<std::vector<int>>& adj) {
    std::vector<int> match(num_v, -1); // Indeks wierzchołka dopasowany do v
    int result = 0; // Liczba dopasowanych par
    for (int u = 0; u < num_u; u++) {
        std::vector<bool> visited(num_v, false);
        if (dfs(u, adj, match, visited)) {
            result++;
        }
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Wielkość szachownicy
    std::cin >> n;

    std::vector<std::vector<int>> board(n, std::vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> board[i][j];
        }
    }

    std::vector<std::tuple<int, int, int>> black_pawns;
    std::vector<std::tuple<int, int, int>> white_pawns;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] > 0) { // Pole niepuste
                if ((i + j) % 2 == 0) { // Czarne pole
                    black_pawns.emplace_back(i, j, board[i][j]);
                } else { // Białe pole
                    white_pawns.emplace_back(i, j, board[i][j]);
                }
            }
        }
    }

    int num_black = black_pawns.size();
    int num_white = white_pawns.size();
    std::vector<std::vector<int>> adj(num_black); // Graf dwudzielny

    for (int i = 0; i < num_black; i++) {
        for (int j = 0; j < num_white; j++) {
            auto& bp = black_pawns[i];
            auto& wp = white_pawns[j];
            int br = std::get<0>(bp); // Wiersz czarnego pionka
            int bc = std::get<1>(bp); // Kolumna czarnego pionka
            int bf = std::get<2>(bp); // Funkcja czarnego pionka
            int wr = std::get<0>(wp); // Wiersz białego pionka
            int wc = std::get<1>(wp); // Kolumna białego pionka
            int wf = std::get<2>(wp); // Funkcja białego pionka

            bool can_b_capture_w = can_capture(br, bc, bf, wr, wc, true);
            bool can_w_capture_b = can_capture(wr, wc, wf, br, bc, false);

            if (can_b_capture_w || can_w_capture_b) {
                adj[i].push_back(j);
            }
        }
    }

    int captured_white_pawns = max_bipartite_matching(num_black, num_white, adj);
    std::cout << n * n - captured_white_pawns;

    return 0;
}