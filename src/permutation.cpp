#include "permutation.hpp"

// Utilities

long int gcd(long int a, long int b) {
    long int u1 = a, u2 = b;
    if (u2 == 0) return u1;
    long int r = u1 % u2;
    while (u2 != 0) {
        r = u1 % u2;
        u1 = u2;
        u2 = r;
    }
    return u1;
}

long int lcm(long int a, long int b) {
    return (a * b) / gcd(a, b);
}

// Permutation implementations

Permutation::Permutation(int n0) : n(n0), images(n0) {
    for (int i = 0; i < n; i++) images[i] = i;
}

template <typename Gen>
Permutation::Permutation(int n0, Gen& G) : n(n0), images(n0) {
    for (int i = 0; i < n; i++) images[i] = i;
    for (int i = 0; i < n - 1; i++) {
        std::uniform_int_distribution<int> N(i, n - 1);
        int j = N(G);
        std::swap(images[i], images[j]);
    }
}

// SparsePermutation implementations

SparsePermutation::SparsePermutation(std::vector<int> v) : n(v.size()) {
    for (int i = 0; i < n; i++) {
        if (i != v[i]) non_trivial_images[i] = v[i];
    }
}

int SparsePermutation::operator[](int i) const {
    auto it = non_trivial_images.find(i);
    return (it != non_trivial_images.end()) ? it->second : i;
}

std::list<int> SparsePermutation::fixed_points() const {
    std::list<int> res;
    for (int i = 0; i < n; i++) {
        if (non_trivial_images.find(i) == non_trivial_images.end()) res.push_front(i);
    }
    return res;
}

bool SparsePermutation::is_derangement() {
    return fixed_points().empty();
}

SparsePermutation SparsePermutation::inverse() const {
    SparsePermutation a(n);
    for (int i = 0; i < n; i++) {
        if (non_trivial_images.find(i) != non_trivial_images.end()) {
            a.non_trivial_images[non_trivial_images.at(i)] = i;
        }
    }
    return a;
}

// Dense cycles
Permutation Permutation::inverse() const {
    Permutation a(n);
    for (int i = 0; i < n; i++) a.images[images[i]] = i;
    return a;
}

// Cycles decomposition
std::list<Cycle> Permutation::cycles() const {
    std::list<Cycle> L;
    std::set<int> S;
    for (int i = 0; i < n; i++) S.insert(i);

    while (!S.empty()) {
        int x = *S.begin();
        S.erase(x);
        if (images[x] != x) {
            Cycle c;
            c.add_last_point(x);
            int x1 = images[x];
            while (x1 != x) {
                S.erase(x1);
                c.add_last_point(x1);
                x1 = images[x1];
            }
            L.push_back(c);
        }
    }
    return L;
}

std::list<Cycle> SparsePermutation::cycles() const {
    std::list<Cycle> L;
    std::set<int> S;
    for (int i = 0; i < n; i++) S.insert(i);

    while (!S.empty()) {
        int x = *S.begin();
        S.erase(x);
        if (non_trivial_images.count(x) && non_trivial_images.at(x) != x) {
            Cycle c;
            c.add_last_point(x);
            int x1 = non_trivial_images.at(x);
            while (x1 != x) {
                S.erase(x1);
                c.add_last_point(x1);
                x1 = non_trivial_images.at(x1);
            }
            L.push_back(c);
        }
    }
    return L;
}

// Orders
long int Permutation::order() const {
    return std::accumulate(cycles().begin(), cycles().end(), 1L,
        [](long int x, const Cycle& c) { return lcm(x, c.order()); });
}

long int SparsePermutation::order() const {
    return std::accumulate(cycles().begin(), cycles().end(), 1L,
        [](long int x, const Cycle& c) { return lcm(x, c.order()); });
}

// Cycle methods
int Cycle::operator[](int i) const {
    auto it = find(i);
    if (it != elem.end()) {
        ++it;
        return (it != elem.end()) ? *it : *elem.begin();
    }
    return i;
}

Cycle Cycle::inverse() const {
    Cycle c;
    c.elem = elem;
    std::reverse(c.elem.begin(), c.elem.end());
    return c;
}

std::list<Cycle> Cycle::cycles() const {
    std::list<Cycle> L;
    L.push_back(*this);
    return L;
}

// I/O operators
std::ostream& operator<<(std::ostream& o, const Permutation& P) {
    o << P.n << " : ";
    for (auto im : P.images) o << im << " ";
    o << "\n";
    return o;
}

std::ostream& operator<<(std::ostream& o, const SparsePermutation& P) {
    o << P.n << " : ";
    for (const auto& [k, v] : P.non_trivial_images) {
        o << k << " " << v << "\n";
    }
    o << "\n";
    return o;
}

std::istream& operator>>(std::istream& f, Permutation& P) {
    int n0;
    f >> n0;
    P.n = n0;
    P.images.resize(n0);
    std::string tmp;
    f >> tmp;
    for (int i = 0; i < n0; i++) f >> P.images[i];
    return f;
}

std::istream& operator>>(std::istream& f, SparsePermutation& P) {
    int n0;
    f >> n0;
    P.n = n0;
    std::pair<int, int> elem;
    while (f >> elem.first >> elem.second) {
        P.non_trivial_images[elem.first] = elem.second;
    }
    return f;
}

std::ostream& operator<<(std::ostream& o, const Cycle& C) {
    o << "[ ";
    for (auto a : C.elem) o << a << " ";
    o << "]\n";
    return o;
}

// Composition
Permutation operator*(const Permutation& a, const Permutation& b) {
    int m = a.size(), n = b.size();
    Permutation res(std::max(m, n));
    if (m < n) {
        Permutation c(n);
        for (int i = 0; i < m; i++) c.images[i] = b[i];
        for (int i = 0; i < n; i++) res.images[i] = a[c[i]];
    } else if (m > n) {
        Permutation c(m);
        for (int i = 0; i < n; i++) c.images[i] = a[i];
        for (int i = 0; i < m; i++) res.images[i] = c[b[i]];
    } else {
        for (int i = 0; i < n; i++) res.images[i] = a[b[i]];
    }
    return res;
}

SparsePermutation operator*(const SparsePermutation& a, const SparsePermutation& b) {
    int m = a.n, n = b.n;
    SparsePermutation res(std::max(m, n));
    if (m < n) {
        SparsePermutation c(n);
        for (int i = 0; i < m; i++) c.non_trivial_images[i] = b[i];
        for (int i = 0; i < n; i++) res.non_trivial_images[i] = a[c[i]];
    } else if (m > n) {
        SparsePermutation c(m);
        for (int i = 0; i < n; i++) c.non_trivial_images[i] = a[i];
        for (int i = 0; i < m; i++) res.non_trivial_images[i] = c[b[i]];
    } else {
        for (int i = 0; i < n; i++) res.non_trivial_images[i] = a[b[i]];
    }
    return res;
}

bool operator<(const Cycle& C1, const Cycle& C2) {
    if (C1.order() != C2.order()) return C1.order() < C2.order();
    auto pair = std::mismatch(C1.elem.begin(), C1.elem.end(), C2.elem.begin());
    return *pair.first < *pair.second;
}
