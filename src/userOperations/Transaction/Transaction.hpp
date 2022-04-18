#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <vector>

#include <userInput/StringInput.hpp>
#include <utils/Datetime.hpp>
#include <utils/Rand.hpp>

class Transaction
{
public:
    Transaction() = default;
    Transaction(const Datetime&,
                uuidType from,
                uuidType to,
                const std::string& description,
                const std::string& type,
                double amount);

    bool set(const Datetime&,
             uuidType from,
             uuidType to,
             const std::string& description,
             const std::string& type,
             double amount);

    bool execute(void);

    Datetime date() const;
    uuidType from() const;
    uuidType to() const;
    std::string description() const;
    std::string type() const;
    double amount() const;

    friend bool operator==(const Transaction&, const Transaction&);
    friend bool operator!=(const Transaction&, const Transaction&);
    friend std::ostream& operator<<(std::ostream&, const Transaction&);

    using TransactionList = std::vector<Transaction>;

private:
    Datetime date_;
    uuidType from_;
    uuidType to_;
    std::string description_;
    std::string type_;
    double amount_;
};

#endif