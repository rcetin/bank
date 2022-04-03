#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

#include <string>

#include <userInput/PasswordInput.hpp>
#include <userInput/UsernameInput.hpp>

class Credentials
{
public:
    bool setUsername(const std::string&);
    bool setPassword(const std::string&);
    std::string username(void);
    std::string password(void); // hash

private:
    bool hash(std::string&);

    UsernameInput username_;
    std::string passwordHash_;
};

#endif