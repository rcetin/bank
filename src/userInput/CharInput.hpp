#ifndef OPTION_INPUT_HPP
#define OPTION_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class CharInput : public Input<char>
{
public:
    CharInput() = default;
    CharInput(const std::string& str)
        : data_(str)
    { }
    CharInput(const char* cstr)
        : data_(cstr)
    { }
    CharInput(char charStr)
        : data_(std::string(1, charStr))
    {
        std::cout << "init from char: " << data_ << "\n";
    }

    virtual char data() const override
    {
        return data_.c_str()[0];
    }

    virtual bool isValid() const override
    {
        const std::regex optionCharacter{R"(^[a-zA-Z]{1}$)"};
        std::cout << "Check validiry of charInput: " << data_ << "\n";
        return std::regex_match(data_, optionCharacter);
    }

    bool operator==(const CharInput& c)
    {
        return data_ == c.data_;
    }

    friend std::istream& operator>>(std::istream& stream, CharInput& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const CharInput& input)
    {
        return stream << input.data_;
    }

private:
    std::string data_;
};

#endif