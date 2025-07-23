#include <iostream>
#include <vector>
#include <limits>

struct Node {
    long long val; // Przechowywana wartość wpływu politycznego
    Node *left, *right; // Wskaźniki na lewe i prawe poddrzewo
    Node(long long v) : val(v), left(nullptr), right(nullptr) {}
};

Node* insert(Node* root, long long value) {
    if (!root) {
        return new Node(value); // Tworzymy nowy węzeł
    }
    if (value < root->val) { // Dodawana wartość jest mniejsza
        root->left = insert(root->left, value); // Przechodzimy do lewego poddrzewa
    }
    else { // Dodawana wartość jest większa
        root->right = insert(root->right, value); // Przechodzimy do prawego poddrzewa
    }
    return root;
}

Node* prune(Node* root, long long a, long long b) {
    if (!root) {
        return nullptr;
    }
    if (root->val < a) { // Wartość za mała
        Node* temp = prune(root->right, a, b); // Przechodzimy do prawego poddrzewa
        delete root; // Usuwamy obecny węzeł
        return temp; // Zwracamy nowe poddrzewo
    }
    if (root->val > b) { // Wartość za duża
        Node* temp = prune(root->left, a, b); // Przechodzimy do lewego poddrzewa
        delete root; // Usuwamy obecny węzeł
        return temp; // Zwracamy nowe poddrzewo
    }
    // Przycinamy lewe i prawe poddrzewo
    root->left = prune(root->left, a, b);
    root->right = prune(root->right, a, b);
    return root;
}

void inorder(Node* root, std::vector<long long>& vec) {
    if (!root) {
        return;
    }
    inorder(root->left, vec);
    vec.push_back(root->val);
    inorder(root->right, vec);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Liczba tropów
    std::cin >> n;

    Node* root = nullptr; // Tworzenie drzewa BST
    for (int i = 0; i < n; i++){
        long long value;
        std::cin >> value;
        root = insert(root, value);
    }

    long long a, b; // Granice domkniętego przedziału rozważanych wartości
    long long k; // Suma wpływów
    std::cin >> a >> b >> k;

    root = prune(root, a, b); // Obcinanie wartości spoza zakresu [a,b]

    std::vector<long long> vec;
    inorder(root, vec);

    int l = 0, r = vec.size() - 1;
    bool found = false;
    long long bestDiff = std::numeric_limits<long long>::max();
    long long ans1 = 0, ans2 = 0;

    while (l < r) { // l od lewego końca tablicy, r od prawego
        long long sum = vec[l] + vec[r];
        if (sum == k) { // Suma jest równa k
            long long diff = vec[r] - vec[l];
            if (diff < bestDiff) { // Różnica jest mniejsza niż dotychczasowa najlepsza
                bestDiff = diff;
                ans1 = vec[l];
                ans2 = vec[r];
                found = true; // Znaleziono parę
            }
            l++; // Przesuwamy wskaźniki, aby znaleźć potencjalne inne pary
            r--;
        } else if (sum < k) { // Suma jest za mała
            l++; // Przesuwamy l (bierzemy większą wartość)
        } else { // Suma jest za duża
            r--; // Przesuwamy k (bierzemy mniejszą wartość)
        }
    }

    if (found) {
        std::cout << ans1 << " " << ans2 << "\n";
    }
    else {
        std::cout << "NIE ZNALEZIONO\n";
    }

    return 0;
}