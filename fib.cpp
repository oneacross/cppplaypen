#include <iostream>

// Creating a fibonacci function in c++14!

auto fib(int n) {

    if (n == 0 || n == 1) {
        return 1;
    }

    return fib(n - 1) + fib(n - 2);
}

int main() {
    for (auto i = 0; i < 10; i += 1) {
        std::cout << "fib(" << i << ") = " << fib(i) << std::endl;
    }
}

