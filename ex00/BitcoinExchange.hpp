#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <string>
# include <map>

class BitcoinExchange {
private:
    std::map<std::string, double> _database;

    bool isValidDate(const std::string& date) const;
    bool isValidValue(const std::string& valueStr, double& value) const;
    std::string trim(const std::string& str) const;

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void processLine(const std::string& line) const;
    bool loadDatabase(const std::string& dbPath);
    void processInput(const std::string& inputPath) const;
};

#endif
