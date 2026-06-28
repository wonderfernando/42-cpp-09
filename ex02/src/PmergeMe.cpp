#include "PmergeMe.hpp"

static void printSequence(const std::string& prefix, const std::vector<int>& seq) {
    std::cout << prefix;
    for (size_t i = 0; i < seq.size(); ++i) {
        std::cout << seq[i];
        if (i + 1 < seq.size()) std::cout << " ";
    }
    std::cout << std::endl;
}

std::vector<FordNode> PmergeMe::fordJohnsonVector(
    std::vector<FordNode> elems,
    std::vector<size_t>& serialToFlag)
{
    size_t n = elems.size();
    if (n <= 1) return elems;

    size_t m = n / 2;
    bool odd = n % 2;
    FordNode oddNode(0, 0, 0);
    if (odd) oddNode = elems[n - 1];

    for (size_t i = 0; i < m; ++i) {
        if (elems[2*i].value > elems[2*i+1].value)
            std::swap(elems[2*i], elems[2*i+1]);
        elems[2*i].flag = i;
        elems[2*i+1].flag = i;
    }

    std::vector<FordNode> low(m), high(m);
    for (size_t i = 0; i < m; ++i) {
        low[i]  = elems[2*i];
        high[i] = elems[2*i+1];
    }

    std::vector<std::pair<size_t,size_t> > saved(m);
    for (size_t i = 0; i < m; ++i) {
        size_t s = high[i].serial;
        saved[i] = std::make_pair(s, serialToFlag[s]);
        serialToFlag[s] = high[i].flag;
    }

    std::vector<FordNode> sortedHigh = fordJohnsonVector(high, serialToFlag);

    std::vector<FordNode> reorderedLow(m);
    for (size_t i = 0; i < m; ++i)
        reorderedLow[i] = low[serialToFlag[sortedHigh[i].serial]];

    for (size_t i = 0; i < m; ++i)
        serialToFlag[saved[i].first] = saved[i].second;

    std::vector<FordNode> result;
    result.reserve(n);
    result.push_back(reorderedLow[0]);
    for (size_t i = 0; i < m; ++i)
        result.push_back(sortedHigh[i]);

    // inserção dos pends inline
    size_t prevJ = 1, curJ = 3;
    while (m > 1 && prevJ < m) {
        size_t upper = (curJ - 1 >= m) ? m - 1 : curJ - 1;
        for (size_t i = upper; i >= prevJ; --i)
            result.insert(std::lower_bound(result.begin(), result.end(),
                reorderedLow[i], CompareByValue()), reorderedLow[i]);
        size_t nextJ = curJ + 2 * prevJ;
        prevJ = curJ;
        curJ = nextJ;
        if (curJ > m && prevJ >= m) break;
    }

    if (odd)
        result.insert(std::lower_bound(result.begin(), result.end(),
            oddNode, CompareByValue()), oddNode);

    return result;
}

std::deque<FordNode> PmergeMe::fordJohnsonDeque(
    std::deque<FordNode> elems,
    std::vector<size_t>& serialToFlag)
{
    size_t n = elems.size();
    if (n <= 1) return elems;

    size_t m = n / 2;
    bool odd = n % 2;
    FordNode oddNode(0, 0, 0);
    if (odd) oddNode = elems[n - 1];

    for (size_t i = 0; i < m; ++i) {
        if (elems[2*i].value > elems[2*i+1].value)
            std::swap(elems[2*i], elems[2*i+1]);
        elems[2*i].flag = i;
        elems[2*i+1].flag = i;
    }

    std::deque<FordNode> low(m), high(m);
    for (size_t i = 0; i < m; ++i) {
        low[i]  = elems[2*i];
        high[i] = elems[2*i+1];
    }

    std::vector<std::pair<size_t,size_t> > saved(m);
    for (size_t i = 0; i < m; ++i) {
        size_t s = high[i].serial;
        saved[i] = std::make_pair(s, serialToFlag[s]);
        serialToFlag[s] = high[i].flag;
    }

    std::deque<FordNode> sortedHigh = fordJohnsonDeque(high, serialToFlag);

    std::deque<FordNode> reorderedLow(m);
    for (size_t i = 0; i < m; ++i)
        reorderedLow[i] = low[serialToFlag[sortedHigh[i].serial]];

    for (size_t i = 0; i < m; ++i)
        serialToFlag[saved[i].first] = saved[i].second;

    std::deque<FordNode> result;
    result.push_back(reorderedLow[0]);
    for (size_t i = 0; i < m; ++i)
        result.push_back(sortedHigh[i]);

    // inserção dos pends inline
    size_t prevJ = 1, curJ = 3;
    while (m > 1 && prevJ < m) {
        size_t upper = (curJ - 1 >= m) ? m - 1 : curJ - 1;
        for (size_t i = upper; i >= prevJ; --i)
            result.insert(std::lower_bound(result.begin(), result.end(),
                reorderedLow[i], CompareByValue()), reorderedLow[i]);
        size_t nextJ = curJ + 2 * prevJ;
        prevJ = curJ;
        curJ = nextJ;
        if (curJ > m && prevJ >= m) break;
    }

    if (odd)
        result.insert(std::lower_bound(result.begin(), result.end(),
            oddNode, CompareByValue()), oddNode);

    return result;
}

void PmergeMe::sortVector(const std::vector<int>& values) {
    size_t n = values.size();
    if (n == 0) { std::cerr << "Error" << std::endl; return; }

    clock_t start = clock();

    // buildElements inline
    size_t m = n / 2;
    bool odd = n % 2;
    int oddVal = odd ? values[n - 1] : 0;
    std::vector<FordNode> elements(m);
    for (size_t i = 0; i < m; ++i) {
        int a = values[2*i], b = values[2*i+1];
        elements[i] = (a <= b) ? FordNode(b, a, i) : FordNode(a, b, i);
    }

    std::vector<int> result;
    result.reserve(n);

    if (m == 0) {
        result.push_back(oddVal);
    } else {
        std::vector<size_t> serialToFlag(n, static_cast<size_t>(-1));
        std::vector<FordNode> sorted = fordJohnsonVector(elements, serialToFlag);

        result.push_back(sorted[0].pend);
        for (size_t i = 0; i < sorted.size(); ++i)
            result.push_back(sorted[i].value);

        // insertPends inline
        size_t prevJ = 1, curJ = 3;
        while (m > 1 && prevJ < m) {
            size_t upper = (curJ - 1 >= m) ? m - 1 : curJ - 1;
            for (size_t i = upper; i >= prevJ; --i)
                result.insert(std::lower_bound(result.begin(), result.end(),
                    sorted[i].pend), sorted[i].pend);
            size_t nextJ = curJ + 2 * prevJ;
            prevJ = curJ; curJ = nextJ;
            if (curJ > m && prevJ >= m) break;
        }

        if (odd)
            result.insert(std::lower_bound(result.begin(), result.end(), oddVal), oddVal);
    }

    double elapsed = static_cast<double>(clock() - start) / CLOCKS_PER_SEC * 1000000.0;
    printSequence("After: ", result);
    std::cout << "Time to process a range of " << n
              << " elements with std::vector : " << elapsed << " us" << std::endl;
}

void PmergeMe::sortDeque(const std::vector<int>& values) {
    size_t n = values.size();
    if (n == 0) return;

    clock_t start = clock();

    // buildElements inline
    size_t m = n / 2;
    bool odd = n % 2;
    int oddVal = odd ? values[n - 1] : 0;
    std::deque<FordNode> elements(m);
    for (size_t i = 0; i < m; ++i) {
        int a = values[2*i], b = values[2*i+1];
        elements[i] = (a <= b) ? FordNode(b, a, i) : FordNode(a, b, i);
    }

    std::deque<int> result;

    if (m == 0) {
        result.push_back(oddVal);
    } else {
        std::vector<size_t> serialToFlag(n, static_cast<size_t>(-1));
        std::deque<FordNode> sorted = fordJohnsonDeque(elements, serialToFlag);

        result.push_back(sorted[0].pend);
        for (size_t i = 0; i < sorted.size(); ++i)
            result.push_back(sorted[i].value);

        // insertPendsDeque inline
        size_t prevJ = 1, curJ = 3;
        while (m > 1 && prevJ < m) {
            size_t upper = (curJ - 1 >= m) ? m - 1 : curJ - 1;
            for (size_t i = upper; i >= prevJ; --i)
                result.insert(std::lower_bound(result.begin(), result.end(),
                    sorted[i].pend), sorted[i].pend);
            size_t nextJ = curJ + 2 * prevJ;
            prevJ = curJ; curJ = nextJ;
            if (curJ > m && prevJ >= m) break;
        }

        if (odd)
            result.insert(std::lower_bound(result.begin(), result.end(), oddVal), oddVal);
    }

    double elapsed = static_cast<double>(clock() - start) / CLOCKS_PER_SEC * 1000000.0;
    std::cout << "Time to process a range of " << n
              << " elements with std::deque : " << elapsed << " us" << std::endl;
}