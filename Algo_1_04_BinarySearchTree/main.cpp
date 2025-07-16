#include <iostream>

struct Measurement {
    int frequency;
    int wavelength;
};

// Węzeł drzewa BST przechowujący pomiary dla danego roku.
struct Node {
    int year;
    Measurement* measurements;
    int measurementCount;
    int measurementCapacity;
    Node* left;
    Node* right;

    Node(int y, int capacity = 100)
        : year(y), measurementCount(0), measurementCapacity(capacity), left(nullptr), right(nullptr)
    {
        measurements = new Measurement[capacity];
    }

    ~Node() {
        delete[] measurements;
    }

    void addMeasurement(int frequency, int wavelength) {
        if (measurementCount == measurementCapacity) {
            measurementCapacity *= 2;
            Measurement* newMeasurements = new Measurement[measurementCapacity];
            for (int i = 0; i < measurementCount; i++) {
                newMeasurements[i] = measurements[i];
            }
            delete[] measurements;
            measurements = newMeasurements;
        }
        measurements[measurementCount++] = {frequency, wavelength};
    }


    void sortMeasurements(int left, int right) { // Merge sort
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        sortMeasurements(left, mid);
        sortMeasurements(mid + 1, right);
        merge(left, mid, right);
    }

    void merge(int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        Measurement* leftArray = new Measurement[n1];
        Measurement* rightArray = new Measurement[n2];

        for (int i = 0; i < n1; i++) {
            leftArray[i] = measurements[left + i];
        }
        for (int i = 0; i < n2; i++) {
            rightArray[i] = measurements[mid + 1 + i];
        }

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (leftArray[i].frequency < rightArray[j].frequency ||
               (leftArray[i].frequency == rightArray[j].frequency && leftArray[i].wavelength < rightArray[j].wavelength)) {
                measurements[k++] = leftArray[i++];
               } else {
                   measurements[k++] = rightArray[j++];
               }
        }

        while (i < n1) measurements[k++] = leftArray[i++];
        while (j < n2) measurements[k++] = rightArray[j++];

        delete[] leftArray;
        delete[] rightArray;
    }
};

void insert(Node*& root, int year, int frequency, int wavelength) {
    if (!root) {
        root = new Node(year);
        root->addMeasurement(frequency, wavelength);
    } else if (year < root->year) {
        insert(root->left, year, frequency, wavelength);
    } else if (year > root->year) {
        insert(root->right, year, frequency, wavelength);
    } else { // Jeśli dany rok już istnieje
        root->addMeasurement(frequency, wavelength);
    }
}

void inOrderTraversal(Node* root) {
    if (!root) return;

    inOrderTraversal(root->left);

    root->sortMeasurements(0, root->measurementCount - 1);

    std::cout << root->year << ": ";
    for (int i = 0; i < root->measurementCount; i++) {
        std::cout << "(" << root->measurements[i].frequency << "," << root->measurements[i].wavelength << ") ";
    }
    std::cout << "\n";

    inOrderTraversal(root->right);
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    Node* root = nullptr;

    int n; // Ilość podróży w czasie
    bool isCorrect=false; // Sprawdza, czy silnik jest dotarty
    bool atLeastOneIncorrectResult=false; // Sprawdcza, czy nastąpił chociaż jeden niepoprawny wynik
    std::cin >> n;

    for (int i=0; i<n; i++) {
        int y; // Rok
        int f; // Zasłyszana częstotliwość w Hz
        int l; // Zmierzona długośc fali w cm
        std::cin >> y >> f >> l;

        if(f >= 20 && f <= 20000) { // !!! znak = chyba nie robi różnicy
            int v = l*f; // Prędkość dźwięku
            if (v >= 31300 && v <= 35000) { // !!! KONIECZNY znak =
                insert(root,y,f,l);
                // W dwóch poniższych if'ach znak = w warunku chyba nie robi różnicy
                if (f >= 275 && f <= 325 && !atLeastOneIncorrectResult) { // Jeśli wynik jest w poprawnym zakresie, oraz NIE NASTĄPIŁ ani jeden niepoprawny wynik
                    isCorrect=true; // to ustawiamy isCorrect na true (w aktualnym momencie wykonywania programu zachodzi co najmniej jeden PRAWIDŁOWY wynik)
                    // Dlaczego sprawdzamy !atLeastOneIncorrectResult? Ponieważ jeśli zaszedł już chociaż jeden NIEPOPRAWNY wynik, to silnik jest niedotarty,
                    // i kolejne poprawne wyniki tego nie zmienią
                }
                if (!(f >= 275 && f <= 325)) { // Jeśli wynik NIE jest w poprawnym zakresie (nie ważne czy nastąpił już jakiś niepoprawny wynik)
                    isCorrect=false; // to ustawiamy isCorrect na false (zachodzi co najmniej jeden NIEPRAWDIŁOWY wynik, więc silnik na pewno NIE będzie dotarty)
                    atLeastOneIncorrectResult=true; // Ustawiamy, że zaszedł niepoprawny wynik
                    // Czemu? Silnik już na pewno nie będzie dotarty, więc trzeba zablokować pierwszego if'a,
                    // żeby nie ustawić błędnie silnika na dotarty przy następnych poprawnych wynikach.
                }
            }
        }
    }
    inOrderTraversal(root);
    if (isCorrect) {
        std::cout << "TAK";
    } else {
        std::cout << "NIE";
    }
    deleteTree(root);
    return 0;
}