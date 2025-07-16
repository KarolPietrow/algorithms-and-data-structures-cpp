#include <iostream>

long add(long a, long b) {
    return a + b;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    for (int i=0; i<n; i++) {
        long a,b;
        std::cin >> a >> b;
        std::cout << add(a, b) << "\n";
    }

    return 0;
}