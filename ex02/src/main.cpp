#include "PmergeMe.hpp"

static bool parseArgs(int argc, char** argv, std::vector<int>& out) {
    if (argc < 2) {
        std::cerr << "Error" << std::endl;
        return false;
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg.empty()) {
            std::cerr << "Error" << std::endl;
            return false;
        }
        for (size_t j = 0; j < arg.size(); ++j) {
            if (!std::isdigit(arg[j])) {
                std::cerr << "Error" << std::endl;
                return false;
            }
        }
        std::stringstream ss(arg);
        long long val;
        ss >> val;
        if (val < 0 || val > 2147483647) {
            std::cerr << "Error" << std::endl;
            return false;
        }
        out.push_back(static_cast<int>(val));
    }
    return true;
}

int main(int argc, char** argv) {
    std::vector<int> values;
    if (!parseArgs(argc, argv, values)) {
        return 1;
    }

    std::cout << "Before:";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << " " << values[i];
    }
    std::cout << std::endl;

    PmergeMe::sortVector(values);
    PmergeMe::sortDeque(values);

    return 0;
}
