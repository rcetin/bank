#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

#include <string>

#include <userInput/PasswordInput.hpp>
#include <userInput/UsernameInput.hpp>
#include <utils/Rand.hpp>

class Credentials
{
public:
    Credentials() = default;
    Credentials(const std::string&, const std::string&);
    Credentials(const std::string&, const std::string&, std::ostream&);
    bool setCredentials(const std::string&, const std::string&);
    bool setCredentials(const std::string&, const std::string&, std::ostream&);
    std::string username(void) const;
    std::string password(void) const; // hash
    bool isValid(void) const;
    bool isValid(std::ostream&) const;

    static bool create(const std::string& username, const std::string& password, uuidType ownerId);
    static bool verifyCredentials(const std::string& username,
                                  const std::string& password,
                                  uuidType& outCustomerId);
    static bool changePassword(uuidType customerId, const std::string newPassword);

private:
    bool hash(std::string&);

    bool setUsername(const std::string&);
    bool setUsername(const std::string&, std::ostream&);
    bool setPassword(const std::string&);
    bool setPassword(const std::string&, std::ostream&);

    UsernameInput username_;
    PasswordInput password_;
    std::string passwordHash_;
    uuidType id_;
    uuidType ownerId_;
};

#endif