#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <climits>

// Reprezentacja punktu na siatce
struct Point {
    int id;
    long long x, y;
};

// Reprezentacja krawędzi w grafie
struct Edge {
    int to;
    long long weight;
};

bool compareX (const Point &a, const Point &b) {
    return a.x < b.x;
}

bool compareY (const Point &a, const Point &b) {
    return a.y < b.y;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Liczba punktów
    std::cin >> n;

    std::vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        long long x, y;
        std::cin >> x >> y;
        points[i] = {i, x, y};
    }

    int s, d;
    std::cin >> s >> d;

    std::vector<std::vector<Edge>> graph(n);

    // Sortowanie po X, dodawanie krawędzi
    std::sort(points.begin(), points.end(), compareX);
    for (int i = 1; i < n; i++) {
        int u = points[i-1].id;
        int v = points[i].id;
        long long cost = std::min(abs(points[i].x - points[i-1].x), abs(points[i].y - points[i-1].y));
        graph[u].push_back({v, cost});
        graph[v].push_back({u, cost});
    }

    // Sortowanie po Y, dodawanie krawędzi
    sort(points.begin(), points.end(), compareY);
    for (int i = 1; i < n; i++) {
        int u = points[i-1].id;
        int v = points[i].id;
        long long cost = std::min(abs(points[i].x - points[i-1].x), abs(points[i].y - points[i-1].y));
        graph[u].push_back({v, cost});
        graph[v].push_back({u, cost});
    }

    std::vector<long long> dist(n, LLONG_MAX); // Minimalny koszt dotarcia do punktu
    dist[s] = 0;

    // Minimalna kolejka priorytetowa min-heap
    std::priority_queue<
        std::pair<long long, int>, // Przechowujemy koszt, indeks
        std::vector<std::pair<long long, int>>, // Vector jako kontener
        std::greater<std::pair<long long, int>> // Kolejka minimalna
    > pq;
    pq.emplace(0, s);

    // Dijkstra - szukamy najkrótszej ścieżki
    while (!pq.empty()) {
        auto [curCost, u] = pq.top(); // Pobieramy najbliższy wierzchołek
        pq.pop();
        if (curCost > dist[u]) { // Istnieje już lepsza trasa
            continue;
        }
        for (auto &edge : graph[u]) { // Sprawdzenie wszystkich sąsiadów
            int v = edge.to;
            long long nd = curCost + edge.weight; // Aktualizacja kosztu dotarcia
            if (nd < dist[v]){ // Nowa trasa jest lepsza
                dist[v] = nd;
                pq.emplace(nd, v);
            }
        }
    }

    std::cout << dist[d] << "\n";
    return 0;
}
