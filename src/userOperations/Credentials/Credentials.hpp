#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

#include <string>

#include <userInput/PasswordInput.hpp>
#include <userInput/UsernameInput.hpp>

class Credentials
{
public:
    Credentials() = default;
    Credentials(const std::string&, const std::string&);
    Credentials(const std::string&, const std::string&, std::ostream&);
    bool setUsername(const std::string&);
    bool setUsername(const std::string&, std::ostream&);
    bool setPassword(const std::string&);
    bool setPassword(const std::string&, std::ostream&);
    std::string username(void) const;
    std::string password(void) const; // hash
    bool isUsernameValid(void) const;
    bool isUsernameValid(std::ostream&) const;
    bool isPasswordValid(void) const;
    bool isPasswordValid(std::ostream&) const;

private:
    bool hash(std::string&);

    UsernameInput username_;
    PasswordInput password_;
    std::string passwordHash_;
};

#endif