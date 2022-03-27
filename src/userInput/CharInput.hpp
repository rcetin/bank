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

    virtual char data() const override
    {
        return data_.c_str()[0];
    }

    virtual bool isValid() const override
    {
        const std::regex optionCharacter{R"(^[a-zA-Z]{1}$)"};

        return std::regex_match(data_, optionCharacter);
    }

    friend std::istream& operator>>(std::istream& stream, CharInput& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, CharInput& input)
    {
        return stream << input.data_;
    }

private:
    std::string data_;
};

#endif