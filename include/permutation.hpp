#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <numeric>
#include <algorithm>
#include <random>
#include <map>

// Utilities
long int gcd(long int a, long int b);
long int lcm(long int a, long int b);

// Forward declarations
class NonModifiablePermutation;
class SparsePermutation;
class Permutation;
class Cycle;

// I/O operators
std::ostream& operator<<(std::ostream&, const Permutation&);
std::istream& operator>>(std::istream&, Permutation&);
std::ostream& operator<<(std::ostream&, const Cycle&);
bool operator<(const Cycle&, const Cycle&);
std::ostream& operator<<(std::ostream&, const SparsePermutation&);
std::istream& operator>>(std::istream&, SparsePermutation&);

// Composition operators
Permutation operator*(const Permutation&, const Permutation&);
SparsePermutation operator*(const SparsePermutation&, const SparsePermutation&);


// Abstract base class for non-modifiable permutations
class NonModifiablePermutation {
public:
    virtual int size() const = 0;
    virtual int operator[](int) const = 0;
    virtual long int order() const = 0;
    virtual std::list<Cycle> cycles() const = 0;
};


// Sparse permutation class
class SparsePermutation {
private:
    int n;
    std::map<int, int> non_trivial_images;

public:
    SparsePermutation(int n0 = 1) : n(n0) {}
    SparsePermutation(std::vector<int>);

    int operator[](int i) const;

    std::list<int> fixed_points() const;
    bool is_derangement();
    SparsePermutation inverse() const;
    std::list<Cycle> cycles() const;
    long int order() const;

    friend std::ostream& operator<<(std::ostream&, const SparsePermutation&);
    friend std::istream& operator>>(std::istream&, SparsePermutation&);
    friend SparsePermutation operator*(const SparsePermutation&, const SparsePermutation&);
};


// Dense permutation class
class Permutation : public NonModifiablePermutation {
private:
    int n;
    std::vector<int> images;

public:
    Permutation(int n0 = 1);
    Permutation(std::vector<int> v) : n(v.size()), images(v) {}

    template <typename Gen>
    Permutation(int n0, Gen& G);

    std::list<int> fixed_points() const;
    bool is_derangement();
    Permutation inverse() const;
    std::list<Cycle> cycles() const;
    long int order() const;

    int size() const { return n; }
    int operator[](int i) const { return images[i]; }

    friend std::ostream& operator<<(std::ostream&, const Permutation&);
    friend std::istream& operator>>(std::istream&, Permutation&);
    friend Permutation operator*(const Permutation&, const Permutation&);
};


// Cyclic permutation class
class Cycle : public NonModifiablePermutation {
private:
    std::vector<int> elem;
    void add_last_point(int i) { elem.push_back(i); }
    std::vector<int>::const_iterator find(int i) const {
        return std::find(elem.begin(), elem.end(), i);
    }

public:
    long int order() const { return elem.size(); }

    int operator[](int i) const;
    Cycle inverse() const;
    std::list<Cycle> cycles() const;
    int size() const { return *std::max_element(elem.begin(), elem.end()); }

    friend std::ostream& operator<<(std::ostream&, const Cycle&);
    friend class Permutation;
    friend class SparsePermutation;
    friend bool operator<(const Cycle&, const Cycle&);
};
