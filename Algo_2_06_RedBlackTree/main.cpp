#include <iostream>
#include <cstring>
#include <cstdlib>

enum Color { RED, BLACK };

struct Node {
    std::string name;
    bool color;
    int size;
    Node *left, *right, *parent;
};

Node *NIL;

void update_size(Node *x) {
    if(x == NIL) return;
    x->size = x->left->size + x->right->size + 1;
}

void leftRotate(Node *&root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NIL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    update_size(x);
    update_size(y);
}

void rightRotate(Node *&root, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if(x->right != NIL)
        x->right->parent = y;
    x->parent = y->parent;
    if(y->parent == NIL)
        root = x;
    else if(y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;
    y->parent = x;
    update_size(y);
    update_size(x);
}

void RBInsertFixup(Node *&root, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if(y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if(z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if(y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if(z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RBInsert(Node *&root, Node *z, int pos) {
    Node *y = NIL;
    Node *x = root;
    int r = pos;      // pozycja względna w aktualnym poddrzewie
    int lastDir = -1; // 0: ostatni krok w lewo, 1: ostatni krok w prawo
    while(x != NIL) {
        y = x;
        x->size++; // aktualizacja rozmiaru węzła
        int leftSize = x->left->size;
        if(r <= leftSize) {
            lastDir = 0; // idziemy w lewo
            x = x->left;
        } else {
            r -= (leftSize + 1);
            lastDir = 1; // idziemy w prawo
            x = x->right;
        }
    }
    z->parent = y;
    if(y == NIL) {
        root = z;
    } else {
        if(lastDir == 0)
            y->left = z;
        else
            y->right = z;
    }
    z->left = NIL;
    z->right = NIL;
    z->color = RED;
    z->size = 1;
    RBInsertFixup(root, z);
}

void RBTransplant(Node *&root, Node *u, Node *v) {
    if(u->parent == NIL)
        root = v;
    else if(u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

Node* TreeMinimum(Node *x) {
    while(x->left != NIL)
        x = x->left;
    return x;
}

void RBDeleteFixup(Node *&root, Node *x) {
    while(x != root && x->color == BLACK) {
        if(x == x->parent->left) {
            Node *w = x->parent->right;
            if(w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if(w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = root;
            }
        } else {
            Node *w = x->parent->left;
            if(w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if(w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void RBDelete(Node *&root, Node *z) {
    Node *y = z;
    bool y_original_color = y->color;
    Node *x;
    if(z->left == NIL) {
        x = z->right;
        RBTransplant(root, z, z->right);
    } else if(z->right == NIL) {
        x = z->left;
        RBTransplant(root, z, z->left);
    } else {
        y = TreeMinimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if(y->parent == z)
            x->parent = y;
        else {
            RBTransplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RBTransplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
        y->size = y->left->size + y->right->size + 1;
    }
    Node *temp = x->parent;
    while(temp != NIL) {
        temp->size = temp->left->size + temp->right->size + 1;
        temp = temp->parent;
    }
    if(y_original_color == BLACK)
        RBDeleteFixup(root, x);
}

Node* find_by_rank(Node *root, int rank) {
    if(root == NIL || rank < 0 || rank >= root->size)
        return NIL;
    Node *x = root;
    while(x != NIL) {
        int leftSize = x->left->size;
        if(rank < leftSize)
            x = x->left;
        else if(rank == leftSize)
            return x;
        else {
            rank -= (leftSize + 1);
            x = x->right;
        }
    }
    return NIL;
}

int get_rank(Node *x) {
    int rank = x->left->size;
    while(x != NIL && x->parent != NIL) {
        if(x == x->parent->right)
            rank += x->parent->left->size + 1;
        x = x->parent;
    }
    return rank;
}

void inorder(Node *root) {
    if(root == NIL) return;
    inorder(root->left);
    std::cout << root->name << "\n";
    inorder(root->right);
}

const int HASH_SIZE = 10000;

struct HashEntry {
    std::string key;
    Node* node;
    HashEntry *next;
};

HashEntry* hashTable[HASH_SIZE];

unsigned int hash_func(const std::string& s) {
    unsigned int hash = 0;
    for(char c : s) {
        hash = hash * 31 + (unsigned int)c;
    }
    return hash % HASH_SIZE;
}

void hash_insert(const std::string& key, Node* node) {
    unsigned int h = hash_func(key);
    HashEntry* entry = new HashEntry;
    entry->key = key;
    entry->node = node;
    entry->next = hashTable[h];
    hashTable[h] = entry;
}

Node* hash_find(const std::string& key) {
    unsigned int h = hash_func(key);
    HashEntry* entry = hashTable[h];
    while(entry) {
        if(entry->key == key) {
            return entry->node;
        }
        entry = entry->next;
    }
    return nullptr;
}

void hash_remove(const std::string& key) {
    unsigned int h = hash_func(key);
    HashEntry* entry = hashTable[h], *prev = nullptr;
    while(entry) {
        if(entry->key == key) {
            if(prev) {
                prev->next = entry->next;
            } else {
                hashTable[h] = entry->next;
            }
            delete entry;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}


Node* create_node(const std::string& s) {
    Node* newNode = new Node;
    newNode->name = s;
    newNode->color = RED;
    newNode->size = 1;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->parent = NIL;
    return newNode;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    NIL = new Node;
    NIL->color = BLACK;
    NIL->size = 0;
    NIL->left = NIL->right = NIL->parent = NIL;

    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable[i] = nullptr;
    }

    Node *root = NIL;

    int n; // Liczba osób na liście
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        Node *newNode = create_node(s);
        int pos = (root == NIL ? 0 : root->size);
        RBInsert(root, newNode, pos);
        hash_insert(s, newNode);
    }

    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        char cmd;
        std::cin >> cmd;
        if(cmd == 'a') { // Dodanie osoby na koniec listy
            std::string s; // Pseudonim osoby
            std::cin >> s;
            Node *newNode = create_node(s);
            int pos = (root == NIL ? 0 : root->size);
            RBInsert(root, newNode, pos);
            hash_insert(s, newNode);
        } else if(cmd == 'm') { // Przesunięcie osoby o x pozycji (x dodatnie – w górę, ujemne – w dół)
            std::string s; // Pseudonim osoby
            int x; // Liczba miejsc do przesunięcia (dodatnia - w górę listy, czyli na początek)
            std::cin >> s >> x;
            Node *node = hash_find(s);
            if(!node) {
                continue;
            }
            int current_rank = get_rank(node);
            int new_rank = current_rank - x;
            if(new_rank < 0) {
                new_rank = 0;
            }
            if(new_rank >= root->size) {
                new_rank = root->size - 1;
            }
            RBDelete(root, node);
            node->left = NIL; node->right = NIL; node->parent = NIL;
            node->color = RED;
            node->size = 1;
            RBInsert(root, node, new_rank);
        } else if(cmd == 'r') {
            std::string s1, s2;
            std::cin >> s1 >> s2;
            Node *node1 = hash_find(s1);
            Node *node2 = hash_find(s2);
            if(!node1 || !node2) {
                continue;
            }
            std::swap(node1->name, node2->name);
            hash_remove(s1);
            hash_remove(s2);
            hash_insert(node1->name, node1);
            hash_insert(node2->name, node2);
        }
    }

    inorder(root);

    return 0;
}