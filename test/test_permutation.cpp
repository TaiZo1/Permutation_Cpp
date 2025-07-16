#include "permutation.hpp"
#include <iostream>
#include <random>

int main() {
    std::mt19937 G(std::random_device{}());

    std::cout << "=== Example of Dense Permutation ===\n";

    // Create a random permutation of size 5
    Permutation P(5, G);
    std::cout << "Permutation P:\n" << P << std::endl;

    // Inverse
    Permutation P_inv = P.inverse();
    std::cout << "Inverse of P:\n" << P_inv << std::endl;

    // Composition
    Permutation P_comp = P * P_inv;
    std::cout << "P * P_inv (should be identity):\n" << P_comp << std::endl;

    // Cycles
    std::cout << "Cycles of P:\n";
    for (const auto& c : P.cycles()) {
        std::cout << c;
    }

    // Order
    std::cout << "Order of P: " << P.order() << "\n\n";

    std::cout << "=== Example of Sparse Permutation ===\n";

    std::vector<int> vec = {1, 0, 2, 3, 4};
    SparsePermutation S(vec);
    std::cout << "Sparse permutation S:\n" << S << std::endl;

    // Inverse
    SparsePermutation S_inv = S.inverse();
    std::cout << "Inverse of S:\n" << S_inv << std::endl;

    // Composition
    SparsePermutation S_comp = S * S_inv;
    std::cout << "S * S_inv (should be identity):\n" << S_comp << std::endl;

    // Cycles
    std::cout << "Cycles of S:\n";
    for (const auto& c : S.cycles()) {
        std::cout << c;
    }

    // Order
    std::cout << "Order of S: " << S.order() << "\n";

    std::cout << "\n=== End of Tests ===" << std::endl;

    return 0;
}
