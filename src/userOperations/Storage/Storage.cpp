#include <array>
#include <cstdio>
#include <iostream>
#include <string>

#include <SQLiteCpp/SQLiteCpp.h>
#include <userOperations/Storage/Storage.hpp>

static constexpr size_t maxSqlCommandLen = 256;

static const char* createCustomerTableCmd = "CREATE TABLE IF NOT EXISTS [Customers]"
                                            "("
                                            "[customerId] INTEGER PRIMARY KEY NOT NULL,"
                                            "[fullname] TEXT NOT NULL,"
                                            "[email] TEXT NOT NULL UNIQUE,"
                                            "[address] TEXT NOT NULL,"
                                            "[birthday] TEXT"
                                            ")";

static const char* createCredentialsTableCmd =
    "CREATE TABLE IF NOT EXISTS [Credentials]"
    "("
    "[credentialId] INTEGER PRIMARY KEY NOT NULL,"
    "[customerId] INTEGER NOT NULL,"
    "[username] TEXT NOT NULL UNIQUE,"
    "[password] TEXT NOT NULL,"
    "FOREIGN KEY([customerId]) REFERENCES[Customers]([customerId])"
    "ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

static const char* createAccountTableCmd =
    "CREATE TABLE IF NOT EXISTS [Accounts]"
    "("
    "[accountId] INTEGER PRIMARY KEY NOT NULL,"
    "[customerId] INTEGER NOT NULL,"
    "[balance] INTEGER NOT NULL,"
    "[openDate] TEXT NOT NULL,"
    "FOREIGN KEY ([customerId]) REFERENCES [Customers] ([customerId])"
    "ON DELETE NO ACTION ON UPDATE NO ACTION"
    ")";

namespace Storage::CustomerMngr
{

bool insert(const Customer& customer, uuidType& outid)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;
    Customer dummyCustomer;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    db.exec(createCustomerTableCmd);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "INSERT INTO Customers"
                        "("
                        "fullname, email, address, birthday"
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

    std::cout << "Executed command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return getByEmail(customer.email(), outid, dummyCustomer);
}

bool getByEmail(const std::string& email, uuidType& outid, Customer& outCustomer)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT * FROM Customers"
                        " WHERE email = \"%s\"",
                        email.c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        // std::cout << "row =" << query.getColumn(0).getName()
        //           << " val=" << query.getColumn(0).getInt() << "\n";
        outid = query.getColumn(0).getInt();
        std::string fullname, email, address, birthday;
        fullname = query.getColumn(1).getString();
        email = query.getColumn(2).getString();
        address = query.getColumn(3).getString();
        birthday = query.getColumn(4).getString();

        outCustomer.setInfo(fullname, email, address, birthday);
        return true;
    }

    return false;
}

bool getByUsername(const std::string& username, uuidType& outid, Customer& outCustomer)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(
        commandArray.data(),
        maxSqlCommandLen,
        "SELECT Customers.customerId, fullname, email, address, birthday FROM Customers"
        " INNER JOIN Credentials"
        " ON Customers.customerId = Credentials.customerId"
        " WHERE Credentials.username = \"%s\"",
        username.c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        outid = query.getColumn(0).getInt();
        std::string fullname, email, address, birthday;
        fullname = query.getColumn(1).getString();
        email = query.getColumn(2).getString();
        address = query.getColumn(3).getString();
        birthday = query.getColumn(4).getString();

        outCustomer.setInfo(fullname, email, address, birthday);
        return true;
    }

    return false;
}

bool update(uuidType customerId, const Customer& customer)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE Customers"
                        " SET fullname = \"%s\","
                        "email = \"%s\","
                        "address = \"%s\","
                        "birthday = \"%s\""
                        " WHERE customerId = %d",
                        customer.fullname().c_str(),
                        customer.email().c_str(),
                        customer.address().c_str(),
                        customer.birthday().c_str(),
                        customerId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return true;
}

} // namespace Storage::CustomerMngr

namespace Storage::CredentialsMngr
{

bool insert(const Credentials& credentials, int32_t customerId, uuidType& outid)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;
    Credentials dummyCred;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
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

    std::cout << "Executed command: " << commandArray.data() << "\n";
    db.exec(commandArray.data());

    return getByCustomerId(customerId, outid, dummyCred);
}

bool getByCustomerId(int32_t customerId, uuidType& outid, Credentials& outCredentials)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT credentialId, username, password FROM Credentials"
                        " WHERE customerId = %d",
                        customerId);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        outid = query.getColumn(0).getInt();
        std::string username, password;
        username = query.getColumn(1).getString();
        password = query.getColumn(2).getString();

        outCredentials.setCredentialsWithHashedPassword(username, password);
        return true;
    }

    return false;
}

bool getByUsername(const std::string& username, uuidType& outid, Credentials& outCredentials)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "SELECT credentialId, username, password FROM Credentials"
                        " WHERE username = \"%s\"",
                        username.c_str());

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed command: " << commandArray.data() << "\n";
    SQLite::Statement query(db, commandArray.data());

    if(query.executeStep()) {
        std::string username, password;

        outid = query.getColumn(0).getInt();
        username = query.getColumn(1).getString();
        password = query.getColumn(2).getString();

        outCredentials.setCredentialsWithHashedPassword(username, password);
        return true;
    }

    return false;
}

bool update(uuidType id, const Credentials& credentials)
{
    std::array<char, maxSqlCommandLen> commandArray{};
    int32_t ret;

    SQLite::Database db(dbFile, SQLite::OPEN_READWRITE);

    ret = std::snprintf(commandArray.data(),
                        maxSqlCommandLen,
                        "UPDATE [Credentials]"
                        "SET [username] = \"%s\","
                        "[password] = \"%s\""
                        "WHERE credentialId = %d",
                        credentials.username().c_str(),
                        credentials.password().c_str(),
                        id);

    if(ret <= 0) {
        return false;
    }

    std::cout << "Executed update command: " << commandArray.data() << "\n";
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