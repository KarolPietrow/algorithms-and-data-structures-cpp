#include <algorithm>
#include <iostream>

void swap (float &a, float &b) {
    float temp = a;
    a = b;
    b = temp;
}

struct Bucket {
    float value1;
    float value2;
    short size;

    Bucket() : size(0) {}

    void add(float value) {
        if (size==0) {
            value1 = value;
        } else if (size==1) {
            value2 = value;
        }
        size++;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Liczba słupków
    std::cin >> n;
    auto* points = new float[n]; // Tablica pozycji

    for (int i = 0; i < n; i++) {
        std::cin >> points[i];
    }

    // Wyznaczamy krańce zakresu
    float minVal = points[0];
    float maxVal = points[0];
    for (int i = 1; i < n; i++) {
        if (points[i] < minVal) {
            minVal = points[i];
        }
        if (points[i] > maxVal) {
            maxVal = points[i];
        }
    }

    if (minVal == maxVal) {
        std::cout << "0.0000\n";
        delete[] points;
        return 0;
    }

    // Dzielimy przedział na n kubełków równej szerokości
    float bucketRange = ((maxVal - minVal) / (float)n);
    auto* buckets = new Bucket[n];

    // Rozrzucamy punkty do kubełków
    for (int i = 0; i < n; i++) {
        int bucketIndex = (int)((points[i] - minVal) / bucketRange);
        bucketIndex = std::min(bucketIndex, n - 1);
        if (buckets[bucketIndex].size < 2) {
            buckets[bucketIndex].add(points[i]);
        } else if (bucketIndex > 0) {
            buckets[bucketIndex-1].add(points[i]);
        }
    }
    delete[] points;

    // W każdym kubełku porządkujemy wartości value1 <= value2
    for (int i = 0; i < n; i++) {
        if (buckets[i].size > 1) {
            if (buckets[i].value1 > buckets[i].value2) {
                swap(buckets[i].value1, buckets[i].value2);
            }
        }
    }

    // Przechodzimy po kubełkach, porównując max poprzedniego z min bieżącego
    float maxGap = 0;
    float previousMax=0;
    if (buckets[0].size == 2) {
        previousMax = buckets[0].value2;
    } else {
        previousMax = buckets[0].value1;
    }

    for (int i = 1; i < n; i++) {
        if (buckets[i].size > 0) {
            float currentMin = buckets[i].value1;
            maxGap = std::max(maxGap, currentMin - previousMax);
            if (buckets[i].size == 2) {
                previousMax = buckets[i].value2;
            } else {
                previousMax = buckets[i].value1;
            }
        }
    }
    delete[] buckets;

    // std::cout << std::fixed << std::setprecision(4) << maxGap << "\n";
    printf("%.4f\n", maxGap);

    return 0;
}