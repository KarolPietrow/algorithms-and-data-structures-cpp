#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct PointState {
    int point;
    unsigned char state;
    PointState(int p = -1, unsigned char s = -1) : point(p), state(s) {}
};

unsigned int n; // Liczba punktów
unsigned int m; // Liczba połączeń między punktami
int s; // Numer punktu startowego
std::vector<std::vector<int>> graph;
std::vector<int> result;
std::vector<char> visited;
std::vector<unsigned char> visitedBfs;
std::queue<PointState> q; // punkt, status (0 == stabilny; 1 == niestabilny)

struct Parent {
    int parentPoint;
    bool state;
    Parent(int p = -1, bool s = false) : parentPoint(p), state(s) {}
};

bool detect_cycle(const int u) {
    // 0 == nieodwiedzony; 1 == aktualnie odwiedzany; 2 == już odwiedzony
    visited[u] = 1;
    for (int v : graph[u]) {
        if (visited[v] == 0) {
            if (detect_cycle(v)) {
                return true;
            }
        } else if (visited[v] == 1) {
            return true;
        }
    }
    visited[u] = 2;
    return false;
}

bool bfs() {
    if (graph[s].empty()) {
        return true;
    }

    visitedBfs.resize(n, 0);
    std::vector<PointState> parent(n * 2, PointState(-1, -1)); // Jednowymiarowy wektor

    q.emplace(s, 0); // Startujemy od punktu początkowego jako stabilny
    visitedBfs[s] = 2;

    while (!q.empty()) {
        auto [u, state] = q.front();
        q.pop();

        for (int v : graph[u]) {
            int next_state = 1 - state; // Zmiana stanu na przeciwny

            if (!(next_state == 0 ? visitedBfs[v] == 2 || visitedBfs[v] == 3
                                  : visitedBfs[v] == 1 || visitedBfs[v] == 3)) {
                if (next_state == 0) {
                    visitedBfs[v] += 2;
                } else {
                    visitedBfs[v] += 1;
                }

                parent[v * 2 + next_state] = PointState(u, state); // Zapis rodzica
                q.emplace(v, next_state);

                if (graph[v].empty() && next_state == 0) {
                    while (v != -1) {
                        result.emplace_back(v);
                        auto [prev, prev_state] = parent[v * 2 + next_state];
                        v = prev;
                        next_state = prev_state;
                    }
                    std::reverse(result.begin(), result.end());
                    return true;
                }
                                  }
        }
    }
    return false;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    unsigned short t; // Liczba testów
    std::cin >> t;

    for (unsigned short i = 0; i < t; i++) {
        std::cin >> n >> m >> s;

        graph.resize(n);

        for (int j = 0; j < m; j++) {
            int x; // Punkt startowy
            int y; // Punkt końcowy
            std::cin >> x >> y;
            graph[x].push_back(y);
        }

        for (int j=0; j<n; j++) {
            std::sort(graph[j].begin(), graph[j].end());
        }

        if (bfs()) {
            std::cout << "TAK\n";
            if (result.empty()) {
                std::cout << s;
            } else {
                for (int v : result) {
                    std::cout << v << " ";
                }
            }
            std::cout << "\n";
        } else {
            visited.resize(n, 0);
            if (detect_cycle(s)) {
                std::cout << "PETLA\n";
            } else {
                std::cout << "NIE\n";
            }
        }
        graph.clear();
        result.clear();
        visited.clear();
        visitedBfs.clear();
        while (!q.empty()) {
            q.pop();
        }
    }
    return 0;
}