#ifndef _INTEGER_INPUT_HPP
#define _INTEGER_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class IntegerInput : public Input<int32_t>
{
public:
    IntegerInput() = default;
    IntegerInput(const std::string& str)
        : data_(str)
    { }
    IntegerInput(const char* cstr)
        : data_(cstr)
    { }

    virtual int32_t data() const override
    {
        return std::stoi(data_);
    }

    virtual bool isValid() const override
    {
        const std::regex positive{R"(^[1-9]{1}[0-9]+$)"};
        const std::regex negative{R"(^-\d+$)"};
        const std::regex zero{R"(^0$)"};

        return std::regex_match(data_, positive) || std::regex_match(data_, negative) ||
               std::regex_match(data_, zero);
    }

    friend std::istream& operator>>(std::istream& stream, IntegerInput& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const IntegerInput& input)
    {
        return stream << input.data_;
    }

private:
    std::string data_;
};

#endif