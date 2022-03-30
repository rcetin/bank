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

        friend std::ostream& operator<<(std::ostream& o, const BankSchema& schema)
        {
            // o << "[name]:" << schema.name << " [desc]:" << schema.description
            //   << " [opName]:" << schema.opName << "\n";
            o << schema.description;
        }
    };
    using menuElemTree = Tree<BankSchema>;

    menuElemTree menuTree;

    bool parseSchema(std::ifstream&);

    /**
     * @brief 
     * 
     * @param str Line to read
     * @param lineSchema [out] line schema output 
     * @param menuLevel [out] menu level of the line
     * @return true line is parsed successfully
     * @return false parsing is failed
     */
    bool readSchemaLine(std::string& str, BankSchema& lineSchema, int32_t& menuLevel);
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