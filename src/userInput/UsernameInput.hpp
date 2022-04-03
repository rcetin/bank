#ifndef _USERNAME_INPUT_HPP
#define _USERNAME_INPUT_HPP

#include <regex>
#include <string>

#include <Input.hpp>

class UsernameInput : public Input<std::string>
{
public:
    using Input::Input;

    virtual std::string data() const override
    {
        return data_;
    }

    virtual bool isValid() const override
    {
        const std::regex username{R"(^(?=[a-zA-Z0-9._]{6,20}$)(?!.*[_.]{2})[^_.].*[^_.]$)"};
        return std::regex_match(data_, username);
    }

    virtual bool isValid(std::ostream& out) const override
    {
        if(isValid()) {
            return true;
        }

        out << "UsernameInput should contain min 6 max 20 characters from set [a-z], [A-Z], [0-9] "
               "and [._]. Also, no _ or . at the beginning, no __ or _. or ._ or .. inside and no "
               "_ or . at the end.\n";
        return false;
    }
};

#endif