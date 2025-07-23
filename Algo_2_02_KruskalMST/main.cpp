#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

// Struktura krawędzi
struct Edge {
    int u;
    int v;
    double x; // Prawdopodobieństwo przeżycia
};

bool compareEdges(const Edge &a, const Edge &b) {
    return a.x > b.x;; // Wybieramy najbezpieczniejszą ścieżkę
}

struct DSU {
    std::vector<int> parent, rank;

    // Inicjalizacja DSU (Disjoint Set Union)
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Szukamy rekurencyjnie korzenia
        }
        return parent[x];
    }

    // Łączymy zbiory
    bool unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false; // Już są w tym samym zbiorze
        }

        // Łączymy mniejsze drzewo z większym
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Liczba punktów strategicznych
    std::cin >> n;
    int m; // Ilość ścieżek w lesie
    std::cin >> m;

    std::vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        std::cin >> edges[i].u >> edges[i].v >> edges[i].x; // Wczytujemy krawędzie
    }

    // Sortowanie krawędzi malejąco względem prawdopodobieństwa przeżycia
    std::sort(edges.begin(), edges.end(), compareEdges);

    // Algorytm Kruskala
    DSU dsu(n);
    double result = 1.0;
    int edgesUsed = 0;

    // Przechodzimy po posortowanych krawędziach
    for (int i = 0; i < m && edgesUsed < n-1; i++) {
        if (dsu.unionSets(edges[i].u, edges[i].v)) {
            result *= edges[i].x;
            edgesUsed++;
        }
    }

    std::cout << std::fixed << std::setprecision(5) << result << "\n";

    return 0;
}