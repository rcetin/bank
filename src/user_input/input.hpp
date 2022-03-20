#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

template <typename Type> class Input
{
  public:
    Type data();
    bool isValid();
    friend std::istream &operator>>(std::istream &, Input &);
};

#endif