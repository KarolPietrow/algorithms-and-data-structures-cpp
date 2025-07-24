#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>


double cross(const std::pair<double, double> &A, const std::pair<double, double> &B, const std::pair<double, double> &C) {
    return (B.first - A.first) * (C.second - A.second)
         - (B.second - A.second) * (C.first - A.first);
}

double polygon_area(const std::vector<std::pair<double, double>> &points) {
    double area = 0.0;
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        const auto &[x1, y1] = points[i];
        const auto &[x2, y2] = points[(i + 1) % n];
        area += x1 * y2 - x2 * y1;
    }
    return std::fabs(area) / 2.0;
}

std::vector<std::pair<double, double>> convex_hull(std::vector<std::pair<double, double>> &points) {
    int n = points.size();
    if (n <= 1) return points;
    std::sort(points.begin(), points.end());
    std::vector<std::pair<double, double>> lower, upper;

    for (const auto &pt : points) {
        while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower.back(), pt) <= 0) {
            lower.pop_back();
        }
        lower.push_back(pt);
    }

    for (int i = n - 1; i >= 0; --i) {
        const auto &pt = points[i];
        while (upper.size() >= 2 &&
               cross(upper[upper.size() - 2], upper.back(), pt) <= 0) {
            upper.pop_back();
               }
        upper.push_back(pt);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int N; // Liczba wież
    std::cin >> N;
    std::vector<std::pair<double, double>> points(N);
    for (int i = 0; i < N; i++) {
        std::cin >> points[i].first >> points[i].second;
    }
    std::vector<std::pair<double, double>> hull = convex_hull(points);
    double area = polygon_area(hull);

    std::cout << std::fixed << std::setprecision(3) << area << "\n";
    return 0;
}