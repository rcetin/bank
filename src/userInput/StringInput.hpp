#ifndef _STRING_INPUT_HPP
#define _STRING_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class StringInput : public Input<std::string>
{
public:
    StringInput() = default;
    StringInput(const std::string& str)
        : data_(str)
    { }
    StringInput(const char* cstr)
        : data_(cstr)
    { }

    virtual std::string data() const override
    {
        return data_;
    }

    virtual bool isValid() const override
    {
        const std::regex stringSchema{"[a-zA-Z\\s]+"};
        return std::regex_match(data_, stringSchema);
    }

    friend std::istream& operator>>(std::istream& stream, StringInput& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const StringInput& input)
    {
        return stream << input.data_;
    }

private:
    std::string data_;
};

#endif