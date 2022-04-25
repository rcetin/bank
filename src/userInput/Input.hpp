#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

template <typename Type>
class Input
{
public:
    Input() = default;
    Input(const std::string& str)
        : data_(str)
    { }
    Input(const char* cstr)
        : data_(cstr)
    { }
    Input(char charStr)
        : data_(std::string(1, charStr))
    { }

    void set(const std::string& str)
    {
        data_ = str;
    }
    void set(const char* cstr)
    {
        data_ = cstr;
    }
    void set(char charStr)
    {
        data_ = std::string(1, charStr);
    }

    void clear()
    {
        data_.clear();
    }

    operator Type()
    {
        return data();
    }

    virtual Type data() const = 0;
    virtual bool isValid() const = 0;
    virtual bool isValid(std::ostream&) const = 0;

    friend std::istream& operator>>(std::istream& stream, Input& input)
    {
        return stream >> input.data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Input& input)
    {
        return stream << input.data_;
    }

protected:
    std::string data_;
};

#endif