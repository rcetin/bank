#ifndef USER_OPS_HPP
#define USER_OPS_HPP

#include <string>
#include <unordered_map>

class UserOps
{
public:
    void run(const std::string& op)
    {
        if(op == "noOp") {
            return;
        }

        ops.at(op)();
    }

private:
    static const std::unordered_map<std::string, void (*)(void)> ops;
};

#endif