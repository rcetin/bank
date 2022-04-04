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
                const StringInput& description,
                int32_t amount);

    uuidType set(const Datetime&,
                 uuidType from,
                 uuidType to,
                 const StringInput& description,
                 int32_t amount);
    uuidType id(void);
    bool store(void);

    using TransactionList = std::vector<Transaction>;

    enum class txType
    {
        in,
        out,
        all
    };

    enum class dateFilter
    {
        before,
        after,
        exact
    };

    /**
     * @brief finds in/out/all transactions of a single account
     * 
     * @param accountId id of account
     * @param type  type of transactions
     * @param outTxList [out] transaction list that will be returned
     * @return true 
     * @return false 
     */
    static bool find(uuidType accountId, txType type, TransactionList& outTxList);

    /**
     * @brief Find transactions before/after/exact time
     * 
     * @param accountId id of account
     * @param type  type of transactions
     * @param time  time
     * @param filter    time filter
     * @param outTxList [out] transaction list that will be returned
     * @return true 
     * @return false 
     */
    static bool find(uuidType accountId,
                     txType type,
                     Datetime time,
                     dateFilter filter,
                     TransactionList& outTxList);

    /**
     * @brief Find transactions between time frame
     * 
     * @param accountId id of account
     * @param type  type of transactions
     * @param timeFrom From time
     * @param timeTo    To time
     * @param outTxList [out] transaction list that will be returned
     * @return true 
     * @return false 
     */
    static bool find(uuidType accountId,
                     txType type,
                     Datetime timeFrom,
                     Datetime timeTo,
                     TransactionList& outTxList);

    /**
     * @brief Find transactions between two accounts
     * 
     * @param from Sender account
     * @param to Recepient account
     * @param outTxList [out] transaction list that will be returned
     * @return true 
     * @return false 
     */
    static bool find(uuidType from, uuidType to, TransactionList& outTxList);

    /**
     * @brief Find transactions between two accounts with a time filter
     * 
     * @param from Sender account
     * @param to Recepient account
     * @param time 
     * @param filter 
     * @param outTxList [out] transaction list that will be returned
     * @return true 
     * @return false 
     */
    static bool
    find(uuidType from, uuidType to, Datetime time, dateFilter filter, TransactionList& outTxList);

    /**
     * @brief Find transactions between two accounts in a time interval
     * 
     * @param from Sender account
     * @param to Recepient account
     * @param timeFrom 
     * @param timeTo 
     * @param outTxList [out] transaction list that will be returned
     * @return true 
     * @return false 
     */
    static bool find(
        uuidType from, uuidType to, Datetime timeFrom, Datetime timeTo, TransactionList& outTxList);

private:
    uuidType txId;
    Datetime date_;
    uuidType from_;
    uuidType to_;
    StringInput description_;
    int32_t amount_;
};

#endif