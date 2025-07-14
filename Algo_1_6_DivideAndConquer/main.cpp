#include <iostream>
int time_jump(unsigned int t);
void next_test();

int size = 1;
int start = 0;
int shortJump = 0;
int longJump = 0;

void divide() {
    // Przedział od size/2 do size
    // Od size do size*2

    int left_pivot = size;
    int right_pivot = size*2;

    size = (left_pivot + right_pivot)/2;
    start = time_jump(0);
    shortJump = time_jump(size);
    longJump = time_jump(size);

    while (shortJump != longJump) {
        if (size%2) {
            // std::cout << size << " ";
            return;
        } else if ((start <= longJump && longJump < shortJump) ||
            (shortJump < start && start <= longJump) ||
            (shortJump < start && longJump < shortJump)) {
            left_pivot = size;
            }
        else {
            right_pivot = size;
        }

        size = (left_pivot + right_pivot)/2;

        start = time_jump(0);
        shortJump = time_jump(size);
        longJump = time_jump(size);
    }
    // std::cout << size << " ";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int t; // Liczba zestawów danych
    std::cin >> t;

    for (int i = 0; i < t; i++) {
        size = 1;
        start = time_jump(0);
        shortJump = time_jump(size);
        longJump = time_jump(size);

        while (shortJump != longJump) {
            if ((start < longJump && longJump < shortJump) ||
                (shortJump < start && start < longJump) ||
                (shortJump < start && longJump < shortJump)) {
                divide();
                break;
                } else {
                    size *= 2;
                    start = time_jump(0);
                    shortJump = time_jump(size);
                    longJump = time_jump(size);
                }
        }
        std::cout << size << " ";
        next_test();
    }
    return 0;
}