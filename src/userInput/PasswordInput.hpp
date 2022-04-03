#ifndef _PASSWORD_INPUT_HPP
#define _PASSWORD_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class PasswordInput : public Input<std::string>
{
public:
    using Input::Input;

    virtual std::string data() const override
    {
        return data_;
    }

    virtual bool isValid() const override
    {
        const std::regex lowerCase{"[a-z]+"};
        const std::regex upperCase{"[A-Z]+"};
        const std::regex number{"[0-9]+"};
        const std::regex specialChars{R"([.!@#$%^&*/]+)"};
        const std::regex lengthCheck{".{8,32}"};

        return std::regex_search(data_, lowerCase) && std::regex_search(data_, upperCase) &&
               std::regex_search(data_, number) && std::regex_search(data_, specialChars) &&
               std::regex_match(data_, lengthCheck);
    }

    virtual bool isValid(std::ostream& out) const override
    {
        if(isValid()) {
            return true;
        }

        out << "PasswordInput should contain min 8 and max 32 characters from set [a-z], [A-Z], "
               "[0-9], [.!@#$%^&*/].\n";
        return false;
    }
};

#endif