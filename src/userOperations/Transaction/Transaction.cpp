#include <userOperations/Transaction/Transaction.hpp>

Transaction::Transaction(const Datetime& date,
                         uuidType from,
                         uuidType to,
                         const std::string& description,
                         const std::string& type,
                         double amount)
    : date_(date)
    , from_(from)
    , to_(to)
    , description_(description)
    , type_(type)
    , amount_(amount)
{ }

bool Transaction::set(const Datetime& date,
                      uuidType from,
                      uuidType to,
                      const std::string& description,
                      const std::string& type,
                      double amount)
{
    date_ = date;
    from_ = from;
    to_ = to;
    description_ = description;
    type_ = type;
    amount_ = amount;

    return true;
}

Datetime Transaction::date() const
{
    return date_;
}

uuidType Transaction::from() const
{
    return from_;
}

uuidType Transaction::to() const
{
    return to_;
}

std::string Transaction::description() const
{
    return description_;
}

std::string Transaction::type() const
{
    return type_;
}

double Transaction::amount() const
{
    return amount_;
}

bool operator==(const Transaction& lhs, const Transaction& rhs)
{
    return lhs.amount_ == rhs.amount_ && lhs.date_ == rhs.date_ &&
           lhs.description_ == rhs.description_ && lhs.from_ == rhs.from_ && lhs.to_ == rhs.to_ &&
           lhs.type_ == rhs.type_;
}

bool operator!=(const Transaction& lhs, const Transaction& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& out, const Transaction& tx)
{
    if(!tx.type_.compare("deposit")) {
        out << "Deposit to Account: " << tx.to_ << " Amount: " << tx.amount_
            << " Date: " << tx.date_.dump();
    }
    else if(!tx.type_.compare("transfer")) {
        out << "Transfer from: " << tx.from_ << " to: " << tx.to_ << " Amount: " << tx.amount_
            << " Date: " << tx.date_.dump();
    }
    else if(!tx.type_.compare("withdraw")) {
        out << "Withdraw from: " << tx.from_ << " Amount: " << tx.amount_
            << " Date: " << tx.date_.dump();
    }
    else {
        out << "Invalid Transaction";
    }

    return out;
}