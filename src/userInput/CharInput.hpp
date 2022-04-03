#ifndef OPTION_INPUT_HPP
#define OPTION_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class CharInput : public Input<char>
{
public:
    using Input::Input;

    virtual char data() const override
    {
        return data_.c_str()[0];
    }

    virtual bool isValid() const override
    {
        const std::regex optionCharacter{R"(^[a-zA-Z]{1}$)"};
        return std::regex_match(data_, optionCharacter);
    }

    virtual bool isValid(std::ostream& out) const override
    {
        if(isValid()) {
            return true;
        }

        out << "CharInput should contain 1 character from set [a-z] and [A-Z].\n";
        return false;
    }

    bool operator==(const CharInput& c)
    {
        return data_ == c.data_;
    }
};

#endif