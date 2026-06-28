#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <utility>

struct FordNode {
    int value; // maior valor do par
    int pend; //  menor do par
    size_t flag; // indice do par no nivel atual
    size_t serial; // indice original do elemento no vetor original

    FordNode(int v, int p, size_t s)
        : value(v), pend(p), flag(0), serial(s) {}
    FordNode() : value(0), pend(0), flag(0), serial(0) {}
};

struct CompareByValue {
    bool operator()(const FordNode& a, const FordNode& b) const {
        return a.value < b.value;
    }
};

class PmergeMe {
public:
    static void sortVector(const std::vector<int>& values);
    static void sortDeque(const std::vector<int>& values);

private:
    PmergeMe();
    PmergeMe(const PmergeMe&);
    PmergeMe& operator=(const PmergeMe&);
    ~PmergeMe();

    static std::vector<FordNode> fordJohnsonVector(
        std::vector<FordNode> elems,
        std::vector<size_t>& serialToFlag);
    static std::deque<FordNode> fordJohnsonDeque(
        std::deque<FordNode> elems,
        std::vector<size_t>& serialToFlag);
};

#endif
