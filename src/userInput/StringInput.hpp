#ifndef _STRING_INPUT_HPP
#define _STRING_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class StringInput : public Input<std::string>
{
public:
    using Input::Input;

    virtual std::string data() const override
    {
        return data_;
    }

    virtual bool isValid() const override
    {
        const std::regex stringSchema{"[a-zA-Z0-9,.:;*@/\\s]+"};
        return std::regex_match(data_, stringSchema);
    }

    virtual bool isValid(std::ostream& out) const override
    {
        if(isValid()) {
            return true;
        }

        out << "StringInput should contain 1 or more characters from set [a-z], [A-Z] and white "
               "space.\n";
        return false;
    }
};

#endif