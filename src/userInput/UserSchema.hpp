#ifndef USER_SCHEMA_HPP
#define USER_SCHEMA_HPP

#include <memory>
#include <stack>
#include <string>

#include <utils/Tree/Tree.hpp>

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
    using menuElemTree = Tree<std::string>;

    menuElemTree menuTree;

    bool parseSchema(std::ifstream&);
    bool readSchemaLine(std::string&, BankSchema&, int32_t& menuLevel);
    menuElemTree::node* addMenuElementToTree(menuElemTree::node*, const BankSchema&);
};

/**
 * menuList
 *  menuItem1
 *      menuSubItem
 * 
 * 
 */

#endif