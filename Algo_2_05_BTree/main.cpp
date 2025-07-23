#include <iostream>
#include <vector>

struct BTreeNode {
    bool leaf; // Czy węzeł jest liściem (nie ma dzieci)
    int num_keys; // Liczba aktualnych kluczy
    std::vector<std::string> keys; // Przechowuje hasła
    std::vector<std::string> values; // Przechowuje objaśnienia
    std::vector<BTreeNode*> children; // Przechowuje wskaźniki do dzieci

    BTreeNode(bool is_leaf_node) : leaf(is_leaf_node), num_keys(0) {}

    int findKeyIndex(const std::string& key) {
        // Szukanie pierwszego elementu >= od klucza
        auto it = lower_bound(keys.begin(), keys.begin() + num_keys, key);
        return it - keys.begin(); // Zwracamy indeks
    }
};

class BTree {
private:
    BTreeNode* root;
    int t;

    // Wstawianie klucza do niepełnego węzła
    void insertNonFull(BTreeNode* node, const std::string &key, const std::string &value) {
        int idx = node->findKeyIndex(key); // Szukamy miejsca dla klucza
        if (idx < node->num_keys && node->keys[idx] == key) { // Klucz już istnieje
            node->values[idx] = value; // Aktualizujemy wartość
            return;
        }

        if (node->leaf) { // Wezeł jest liściem
            node->keys.insert(node->keys.begin() + idx, key); // Wstawiamy klucz
            node->values.insert(node->values.begin() + idx, value);
            node->num_keys++;
        } else { // Węzeł nie jest liściem
            BTreeNode* child = node->children[idx];
            if (child->num_keys == 2 * t - 1) { // Schodzimy do dziecka
                splitChild(node, idx);
                idx = node->findKeyIndex(key);
                if (idx < node->num_keys && node->keys[idx] == key) {
                    node->values[idx] = value;
                    return;
                }
                child = node->children[idx]; // Aktualizujemy dziecko po podziale
            }
            insertNonFull(child, key, value); // Rekurencyjnie wstawiamy do dziecka
        }
    }

    // Podział pełnego dziecka na dwa węzły
    void splitChild(BTreeNode* parent, int idx) {
        BTreeNode* child = parent->children[idx];
        BTreeNode* new_child = new BTreeNode(child->leaf);
        new_child->num_keys = t - 1;

        for (int i = 0; i < t - 1; ++i) {
            new_child->keys.push_back(child->keys[t + i]);
            new_child->values.push_back(child->values[t + i]);
        }

        if (!child->leaf) {
            for (int i = 0; i < t; ++i) {
                new_child->children.push_back(child->children[t + i]);
            }
        }

        child->num_keys = t - 1;

        std::string median_key = child->keys[t - 1];
        std::string median_value = child->values[t - 1];

        parent->keys.insert(parent->keys.begin() + idx, median_key);
        parent->values.insert(parent->values.begin() + idx, median_value);
        parent->children.insert(parent->children.begin() + idx + 1, new_child);
        parent->num_keys++;

        child->keys.resize(t - 1);
        child->values.resize(t - 1);
        if (!child->leaf) {
            child->children.resize(t);
        }
    }

public:
    BTree(int degree) : t(degree), root(nullptr) {}

    void insert(const std::string &key, const std::string &value) {
        if (root == nullptr) { // Drzewo jest puste
            root = new BTreeNode(true); // Tworzymy nowy korzeń (liść)
            root->keys.push_back(key); // Dodajemy klucz
            root->values.push_back(value); // i objaśnienie
            root->num_keys = 1;
            return;
        }
        if (root->num_keys == 2 * t - 1) { // Korzeń jest pełny
            BTreeNode* new_root = new BTreeNode(false); // Tworzymy nowy korzeń
            new_root->children.push_back(root);
            root = new_root;
            splitChild(root, 0);
        }
        // Wstawianie klucza do niepełnego korzenia
        insertNonFull(root, key, value);
    }

    std::string search(const std::string& key) {
        BTreeNode* current = root;
        while (current != nullptr) {
            int idx = current->findKeyIndex(key);
            if (idx < current->num_keys && current->keys[idx] == key) {
                return current->values[idx];
            }
            if (current->leaf) {
                return "BRAK";
            } else {
                current = current->children[idx];
            }
        }
        return "BRAK";
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n; // Liczba operacji
    std::cin >> n;
    std::cin.ignore();
    BTree tree(10);

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);
        size_t position = line.find(':');

        if (position != std::string::npos) { // Znaleziono dwukropek
            std::string key = line.substr(0, position); // Hasło
            std::string value = line.substr(position + 1); // Objaśnienie

            if (!value.empty() && value[0] == ' ') {
                value.erase(0,1);
            }
            tree.insert(key,value);
        } else {
            std::string result = tree.search(line);
            std::cout << result << "\n";
        }
    }
    return 0;
}