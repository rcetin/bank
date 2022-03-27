#ifndef USER_SCHEMA_HPP
#define USER_SCHEMA_HPP

#include <memory>
#include <string>
#include <vector>

class UserSchema
{
public:
    UserSchema(const std::string&);
    bool searchOption(char);
    std::string getDescription(char);
    std::string getCurrentOp(void);
    std::string listSubmenuElements();

    class SchemaError : public std::runtime_error
    {
    public:
        SchemaError(const char* message)
            : std::runtime_error{message}
        { }
    };

private:
    struct BankSchema
    {
        std::string name;
        std::string description;
        std::string opName;
    };

    bool parseSchema(std::ifstream&);
    bool readSchemaLine(const std::string&);
};

/**
 * menuList
 *  menuItem1
 *      menuSubItem
 * 
 * 
 */

#endif