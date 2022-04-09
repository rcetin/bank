#include <array>
#include <cstdio>
#include <iostream>
#include <string>

#include <SQLiteCpp/SQLiteCpp.h>
#include <userOperations/Storage/Storage.hpp>

static constexpr size_t maxSqlCommandLen = 256;

static const char* createCustomerTableCmd = "CREATE TABLE IF NOT EXISTS [Customers]"
                                            "("
                                            "[id] INTEGER PRIMARY KEY NOT NULL,"
                                            "[fullname] TEXT NOT NULL,"
                                            "[email] TEXT NOT NULL UNIQUE,"
                                            "[address] TEXT NOT NULL,"
                                            "[birthday] TEXT"
                                            ")";

static const char* createCredentialsTableCmd =
    "CREATE TABLE IF NOT EXISTS [Credentials]"
    "("
    "[id] INTEGER PRIMARY KEY NOT NULL,"
    "[customerId] INTEGER NOT NULL,"
    "[username] TEXT NOT NULL UNIQUE,"
    "[password] TEXT NOT NULL,"
    "FOREIGN KEY([customerId]) REFERENCES[Customers]([id])"
    "ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

static const char* createAccountTableCmd =
    "CREATE TABLE IF NOT EXISTS [Accounts]"
    "("
    "[id] INTEGER PRIMARY KEY NOT NULL,"
    "[customerId] INTEGER NOT NULL,"
    "[balance] INTEGER NOT NULL,"
    "[openDate] TEXT NOT NULL,"
    "FOREIGN KEY ([customerId]) REFERENCES [Customers] ([id])"
    "ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

// static const char* dropTableCmd = "DROP TABLE [Customer]";

// bool Storage::removeDb(void)
// {
//     return std::remove(dbFile);
// }

namespace Storage::CustomerMngr
{

bool insert(const Customer& customer, uuidType& outid)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
    db.exec(createCustomerTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO [Customers]"
                        "("
                        "[fullname], [email], [address], [birthday]"
                        ")"
                        "VALUES"
                        "("
                        "\"%s\", \"%s\", \"%s\", \"%s\""
                        ")",
                        customer.fullname().c_str(),
                        customer.email().c_str(),
                        customer.address().c_str(),
                        customer.birthday().c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Insert command executed: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return getByEmail(customer.email(), outid);
}

bool getByEmail(const std::string& email, uuidType& outid)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT id FROM [Customers]"
                        "WHERE email = \"%s\"",
                        email.c_str());

    if(ret <= 0) {
        return false;
    }

    SQLite::Statement query(db, commandArray.data());
    std::cout << "Executed command: " << commandArray.data() << "\n";

    if(query.executeStep()) {
        std::cout << "row =" << query.getColumn(0).getName()
                  << " val=" << query.getColumn(0).getInt() << "\n";
        outid = query.getColumn(0).getInt();
        return true;
    }

    return false;
}

bool getByUsername(const std::string& username, uuidType& outid)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT customerId FROM [Credentials]"
                        "WHERE username = \"%s\"",
                        username.c_str());

    if(ret <= 0) {
        return false;
    }

    SQLite::Statement query(db, commandArray.data());
    std::cout << "Executed command: " << commandArray.data() << "\n";

    if(query.executeStep()) {
        std::cout << "row =" << query.getColumn(0).getName()
                  << " val=" << query.getColumn(0).getInt() << "\n";
        outid = query.getColumn(0).getInt();
        return true;
    }

    return false;
}

bool update(uuidType customerId, const Customer& customer)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE [Customers]"
                        "SET [fullname] = \"%s\","
                        "[email] = \"%s\","
                        "[address] = \"%s\","
                        "[birthday] = \"%s\""
                        "WHERE id = %d",
                        customer.fullname().c_str(),
                        customer.email().c_str(),
                        customer.address().c_str(),
                        customer.birthday().c_str(),
                        customerId);

    if(ret <= 0) {
        return false;
    }

    db.exec(commandArray.data());
    std::cout << "Executed command: " << commandArray.data() << "\n";

    return true;
}

} // namespace Storage::CustomerMngr

namespace Storage::CredentialsMngr
{

bool insert(const Credentials& credentials, int32_t customerId)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
    db.exec(createCredentialsTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO [Credentials]"
                        "("
                        "[customerId], [username], [password]"
                        ")"
                        "VALUES"
                        "("
                        "\"%d\", \"%s\", \"%s\""
                        ")",
                        customerId,
                        credentials.username().c_str(),
                        credentials.password().c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Insert command executed: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}
} // namespace Storage::CredentialsMngr

namespace Storage::AccountMngr
{
bool insert(const Account& account, int32_t customerId)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
    db.exec(createAccountTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO [Accounts]"
                        "("
                        "[customerId], [balance], [openDate]"
                        ")"
                        "VALUES"
                        "("
                        "\"%d\", \"%d\", \"%s\""
                        ")",
                        customerId,
                        account.balance(),
                        account.openDate().c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Insert command executed: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}
} // namespace Storage::AccountMngr