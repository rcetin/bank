#ifndef _INTEGER_INPUT_HPP
#define _INTEGER_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class IntegerInput : public Input<int32_t>
{
public:
    using Input::Input;

    virtual int32_t data() const override
    {
        return std::stoi(data_);
    }

    virtual bool isValid() const override
    {
        const std::regex positive{R"(^[1-9]{1}[0-9]*$)"};
        const std::regex negative{R"(^-\d+$)"};
        const std::regex zero{R"(^0$)"};

        return std::regex_match(data_, positive) || std::regex_match(data_, negative) ||
               std::regex_match(data_, zero);
    }

    virtual bool isValid(std::ostream& out) const override
    {
        if(isValid()) {
            return true;
        }

        out << "IntegerInput should contain 1 or more characters from set [1-9] and should not "
               "start with + sign.\n";
        return false;
    }
};

#endif