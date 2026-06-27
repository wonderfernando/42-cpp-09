#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        this->_database = other._database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

std::string BitcoinExchange::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n\v\f");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n\v\f");
    return str.substr(first, (last - first + 1));
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i]))
            return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (isLeap) {
            if (day > 29)
                return false;
        } else {
            if (day > 28)
                return false;
        }
    }
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const {
    if (valueStr.empty())
        return false;
    char* endptr;
    value = std::strtod(valueStr.c_str(), &endptr);
    if (endptr == valueStr.c_str() || *endptr != '\0')
        return false;
    return true;
}

void BitcoinExchange::processLine(const std::string& line) const {
    if (line.empty())
        return;
    if (trim(line).empty())
        return;

    size_t pipePos = line.find('|');
    if (pipePos == std::string::npos) {
        std::cout << "Error: bad input => " << trim(line) << std::endl;
        return;
    }

    std::string date = trim(line.substr(0, pipePos));
    std::string valueStr = trim(line.substr(pipePos + 1));

    if (!isValidDate(date)) {
        std::cout << "Error: bad input => " << date << std::endl;
        return;
    }

    double value;
    if (!isValidValue(valueStr, value)) {
        std::cout << "Error: bad input => " << trim(valueStr) << std::endl;
        return;
    }

    if (value < 0) {
        std::cout << "Error: not a positive number." << std::endl;
        return;
    }
    if (value > 1000) {
        std::cout << "Error: too large a number." << std::endl;
        return;
    }

    if (_database.empty()) {
        std::cout << "Error: database is empty." << std::endl;
        return;
    }

    // Find closest date <= query_date
    std::map<std::string, double>::const_iterator it = _database.upper_bound(date);
    if (it == _database.begin()) {
        std::cout << "Error: date is older than the earliest database record => " << date << std::endl;
        return;
    }

    // Decrement iterator to get the element <= query_date
    --it;

    double rate = it->second;
    double result = value * rate;

    std::cout << date << " => " << value << " = " << result << std::endl;
}

bool BitcoinExchange::loadDatabase(const std::string& dbPath) {
    std::ifstream file(dbPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database." << std::endl;
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Error: database is empty." << std::endl;
        return false;
    }

    // Process each line of the CSV database
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) {
            std::cerr << "Error: invalid line in database => " << line << std::endl;
            continue;
        }

        std::string date = trim(line.substr(0, commaPos));
        std::string rateStr = trim(line.substr(commaPos + 1));

        if (!isValidDate(date)) {
            std::cerr << "Error: invalid date in database => " << date << std::endl;
            continue;
        }

        char* endptr;
        double rate = std::strtod(rateStr.c_str(), &endptr);
        if (endptr == rateStr.c_str() || *endptr != '\0') {
            std::cerr << "Error: invalid rate in database => " << rateStr << std::endl;
            continue;
        }

        _database[date] = rate;
    }
    file.close();
    return true;
}


void BitcoinExchange::processInput(const std::string& inputPath) const {
    std::ifstream file(inputPath.c_str());
    if (!file.is_open()) {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    if (!std::getline(file, line))
        return; 

    std::string trimmedHeader = trim(line);
    if (trimmedHeader != "date | value")
        processLine(line);

    while (std::getline(file, line)) {
        processLine(line);
    }
    file.close();
}
