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
    bool isOptionValid(char);
    std::string getOptionDescription(char);
    std::string getCurrentOperation(void);
    std::string listCurrentMenu();

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
        int32_t menuLevel;

        friend std::ostream& operator<<(std::ostream& o, const BankSchema& schema)
        {
            o << schema.description;
        }
    };
    using menuElemTree = Tree<BankSchema>;
    using menuElemStackElem = std::pair<BankSchema, menuElemTree::node*>;

    menuElemTree menuTree;

    /**
     * @brief Parse given options schema
     * 
     * @return true 
     * @return false 
     */
    bool parseSchema(std::ifstream&);

    /**
     * @brief Parses the options schema line
     * 
     * @param str Line to read
     * @param lineSchema [out] line schema output 
     * @param menuLevel [out] menu level of the line
     * @return true line is parsed successfully
     * @return false parsing is failed
     */
    bool parseSchemaLine(std::string& str, BankSchema& lineSchema);

    /**
     * @brief Add options schema element to the internal tree structure
     * 
     * @param schemaStack schema stack to pop put elements
     * @param lineSchema line schema input which will be used to push to tree
     * @return menuElemTree::node* 
     */
    menuElemTree::node* addMenuElementToTree(std::stack<menuElemStackElem> schemaStack,
                                             const BankSchema& lineSchema);
};

/**
 * menuList
 *  menuItem1
 *      menuSubItem
 * 
 * 
 */

#endif