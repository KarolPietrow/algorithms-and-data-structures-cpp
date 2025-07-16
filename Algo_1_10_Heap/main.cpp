#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class Heap {
private:
    std::vector<int> heap;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }

    // Przywraca własność kopca w dół od węzła i
    void heapifyDown(int i) {
        int largest = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < heap.size() && heap[left] > heap[largest]) {
            largest = left;
        }

        if (right < heap.size() && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest != i) {
            std::swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    // Przywraca własność kopca w górę od węzła i
    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)] < heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    void insert(int value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    int extractMax() {
        int maxElement = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return maxElement;
    }

    int getMax() {
        // Zwraca maksimum bez usuwania
        return heap[0];
    }

    bool empty() {
        return heap.empty();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Liczba przedsiębiorców
    std::cin >> n;

    std::vector<int> results(n,0);

    for (int i = 0; i < n; i++) {
        int o; // Liczba operacji dla przedsiębiorcy
        std::cin >> o;

        Heap prices; // Kopiec przechowujący ceny produktów
        std::vector<int> taxes; // Wektor stawek podatkowych

        while (o) {
            char f;
            std::cin >> f;
            switch (f) {
                default:
                    break;

                case 'a':
                    int p; // Cena w zł
                    int t; // Podatek w %
                    std::cin >> p >> t;
                    prices.insert(p);
                    taxes.push_back(t);
                    o--;
                    break;

                case 'p':
                    int x;
                    std::cin >> x;
                    if (!prices.empty()) {
                        int maxPrice = prices.extractMax();
                        int newPrice = maxPrice + x;
                        if (newPrice > 0) {
                            prices.insert(newPrice);
                        } else {
                            prices.insert(maxPrice);
                        }
                    }
                    o--;
                    break;
            }
        }

        // Przydzielanie najmniejszych stawek do największych cen
        int totalTax = 0;
        sort(taxes.begin(), taxes.end());
        for (int j = 0; j < taxes.size(); j++) {
            int price = prices.extractMax();
            totalTax += price * taxes[j];
        }
        results[i] = totalTax;
    }
    for (int i = 0; i < n; i++) {
        std::cout << results[i] << "\n";
    }

    return 0;
}
