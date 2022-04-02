#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

#include <string>

#include <userInput/PasswordInput.hpp>
#include <userInput/StringInput.hpp>

class Credentials
{
public:
    bool setUsername(const std::string&);
    bool setPassword(const std::string&);
    std::string username(void);
    std::string password(void); // hash

private:
    bool isPasswordValid(const StringInput&);
    bool isUsernameValid(const PasswordInput&);
    bool hash(std::string&);

    StringInput username_;
    std::string passwordHash_;
};

#endif