#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

template <typename Type>
class Input
{
public:
    virtual Type data() const = 0;
    virtual bool isValid() const = 0;
};

#endif