#include <iostream>
#include <climits>

int currentTime=0; // Czas globalny
unsigned int l=0; // Ilość kas w sklepie
int s=0; // Liczba sekund potrzebnych na zeskanowanie produktu
int z=0; // Liczba sekund potrzebnych na dokonanie płatności przez klienta

struct Client {
public:
    int products=0;
    int clientWaitTime=0;

    Client() = default;

    explicit Client(int products) {
        this->products = products;
        clientWaitTime=0;
    }
};

struct Node {
    Client client;
    Node* next;
};

// Wskaźnikowa kolejka FIFO do przechowywania klientów
class Queue {
private:
    Node* front;  // wskaźnik na pierwszy element kolejki
    Node* rear;   // wskaźnik na ostatni element kolejki
    int size=0;

public:
    // Konstruktor inicjalizujący pustą kolejkę
    Queue() {
        front = nullptr;
        rear = nullptr;
    }

    // Sprawdzanie czy kolejka jest pusta
    bool isEmpty() {
        return front == nullptr;
    }

    // Dodawanie elementu do kolejki
    void enqueue(Client value) {
        // Tworzenie nowego węzła
        size++;
        Node* temp = new Node();
        temp->client = value;
        temp->next = nullptr;

        if (isEmpty()) {
            // Jeśli kolejka jest pusta, ustaw `front` i `rear` na nowy element
            front = rear = temp;
        } else {
            // Jeśli nie, dodajemy element na końcu kolejki
            rear->next = temp;
            rear = temp;
        }
    }

    // Usuwanie elementu z kolejki
    void dequeue() {
        size--;
        if (isEmpty()) {
            return;
        }
        // Przechowujemy wskaźnik na pierwszy element, by móc go usunąć
        Node* temp = front;
        front = front->next;

        // Jeśli po usunięciu `front` jest nullptr, ustawiamy także `rear` na nullptr
        if (front == nullptr) {
            rear = nullptr;
        }

        // Usunięcie pierwszego elementu
        delete temp;
    }

    // Zwracanie pierwszego elementu kolejki (bez usuwania)
    Client& peek() {
        return front->client;
    }

    int getSize() {
        return size;
    }

    // Destruktor, który czyści pamięć przy usuwaniu kolejki
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};


// Pojedyncza kasa w sklepie
struct CashRegister {
    bool isOpen=false;
    int waitTime=0; // Czas symulacji
    Queue queue;

    static void addClientToQueue(Client client, CashRegister* registers) {
        int minValue = INT_MAX;
        int minIndex = INT_MAX;

        for (int i=0; i<l; i++) {
            if (registers[i].isOpen) {
                if (registers[i].waitTime < minValue) {
                    minValue = registers[i].waitTime;
                    minIndex = i;
                }
            }
        }
        // Wyliczamy czas obsługi i przypisujemy klienta
        client.clientWaitTime = client.products * s + z;
        registers[minIndex].waitTime += client.products * s + z;
        registers[minIndex].queue.enqueue(client);
    }

    // Symuluje upływ t sekund – aktualizuje stan wszystkich otwartych kas.
    static void updateWaitTime(CashRegister* registers, int t) {
        for (int i=0; i<l; i++) {
            if (registers[i].isOpen && registers[i].waitTime > 0) {
                int temp_t = t;
                // Jeśli czas do zasymulowania jest większy lub równy czasu oczekiwania pierwszego klienta, obsłuż go i spróbuj ponownie
                while (!registers[i].queue.isEmpty() && registers[i].queue.peek().clientWaitTime <= temp_t) {
                    registers[i].waitTime -= registers[i].queue.peek().clientWaitTime;
                    temp_t -= registers[i].queue.peek().clientWaitTime;
                    registers[i].queue.dequeue();
                }
                // Jeśli pierwszy klient nie zdążył w całości, skróć jego oczekiwanie
                if (!registers[i].queue.isEmpty()) {
                    registers[i].queue.peek().clientWaitTime -= temp_t;
                    registers[i].waitTime -= temp_t;
                }
            }
        }
    }

    void open() {
        isOpen = true;
    }

    void close(CashRegister* registers) {
        isOpen = false;
        if(!queue.isEmpty()) {
            queue.dequeue();
            while(!queue.isEmpty()) {
                addClientToQueue(queue.peek(), registers);
                queue.dequeue();
            }
        }
        waitTime = 0;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int t; // Czas pracy sklepu podlegający symulacji (w sekundach)

    std::cin >> t >> l >> s >> z;

    auto* registers = new CashRegister[l];
    for (int i=0; i<l; i++) {
        registers[i] = CashRegister();
    }

    while(currentTime<t) {
        // // DEBUG
        // for (int i=0; i<l; i++) {
        //     if (registers[i].isOpen) {
        //         std::cout << "K" << i << ": " << registers[i].queue.getSize() << "o " << registers[i].waitTime << "s";
        //     } else {
        //         std::cout << "K" << i << ": z";
        //     }
        //     if (i<l-1) {
        //         std::cout << ", ";
        //     }
        // }

        char f;
        unsigned int a;

        std::cin >> f;
        switch(f) {
            default: {
                break;
            }
            case 'k': {
                int p = 0; // Czas od rozpoczęcia symulacji lub ostatniego klienta
                int r = 0; // Liczba produktów w koszyku klienta
                std::cin >> p >> r;
                // Jeśli klient pojawia się przed końcem symulacji
                if (currentTime + p <= t) {
                    currentTime += p;
                    CashRegister::updateWaitTime(registers, p);
                    CashRegister::addClientToQueue(Client(r), registers);
                    break;
                } else {
                    CashRegister::updateWaitTime(registers, (t-currentTime));
                    currentTime=t;
                    break;
                }
            }

            case 'o': {
                std::cin >> a;
                if (!registers[a].isOpen) {
                    registers[a].open();
                }
                break;
            }

            case 'z': {
                std::cin >> a;
                if (registers[a].isOpen) {
                    registers[a].close(registers);
                }
                break;
            }
        }
    }

   for (int i=0; i<l; i++) {
        if (registers[i].isOpen) {
            std::cout << "K" << i << ": " << registers[i].queue.getSize() << "o " << registers[i].waitTime << "s";
        } else {
        std::cout << "K" << i << ": z";
        }
        if (i<l-1) {
            std::cout << ", ";
        }
   }

    delete[] registers;
    return 0;
}