#ifndef _USERNAME_INPUT_HPP
#define _USERNAME_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class UsernameInput : public Input<std::string>
{
public:
    UsernameInput() = default;
    UsernameInput(const std::string& str)
        : data_(str)
    { }
    UsernameInput(const char* cstr)
        : data_(cstr)
    { }

    virtual std::string data() const override
    {
        return data_;
    }

    virtual bool isValid() const override
    {
        const std::regex username{R"(^(?=[a-zA-Z0-9._]{6,20}$)(?!.*[_.]{2})[^_.].*[^_.]$)"};

        // std::cout << std::boolalpha << "lower=" << std::regex_search(data_, lowerCase)
        //           << " upper=" << std::regex_search(data_, upperCase)
        //           << " number=" << std::regex_search(data_, number)
        //           << " special=" << std::regex_search(data_, specialChars)
        //           << " length=" << std::regex_match(data_, lengthCheck) << '\n';

        return std::regex_match(data_, username);
    }

    friend std::istream& operator>>(std::istream& stream, UsernameInput& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const UsernameInput& input)
    {
        return stream << input.data_;
    }

private:
    std::string data_;
};

#endif