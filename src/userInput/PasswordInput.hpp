#ifndef _PASSWORD_INPUT_HPP
#define _PASSWORD_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class PasswordInput : public Input<std::string>
{
public:
    PasswordInput() = default;
    PasswordInput(const std::string& str)
        : data_(str)
    { }
    PasswordInput(const char* cstr)
        : data_(cstr)
    { }

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

        // std::cout << std::boolalpha << "lower=" << std::regex_search(data_, lowerCase)
        //           << " upper=" << std::regex_search(data_, upperCase)
        //           << " number=" << std::regex_search(data_, number)
        //           << " special=" << std::regex_search(data_, specialChars)
        //           << " length=" << std::regex_match(data_, lengthCheck) << '\n';

        return std::regex_search(data_, lowerCase) && std::regex_search(data_, upperCase) &&
               std::regex_search(data_, number) && std::regex_search(data_, specialChars) &&
               std::regex_match(data_, lengthCheck);
    }

    friend std::istream& operator>>(std::istream& stream, PasswordInput& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const PasswordInput& input)
    {
        return stream << std::string(input.data_.length(), '*');
    }

private:
    std::string data_;
};

#endif