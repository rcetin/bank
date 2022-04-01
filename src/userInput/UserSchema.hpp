#ifndef USER_SCHEMA_HPP
#define USER_SCHEMA_HPP

#include <memory>
#include <stack>
#include <string>

#include <userInput/CharInput.hpp>
#include <userInput/StringInput.hpp>
#include <utils/Tree/Tree.hpp>

class UserSchema
{
public:
    UserSchema(const std::string&);
    bool isOptionValid(char);
    bool processUserInput(char);
    std::string getCurrentOptionDescription(void);
    std::string getCurrentOperation(void);
    void dumpCurrentMenu(std::ostream&);

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
        CharInput key;
        StringInput description;
        StringInput operation;
        int32_t menuLevel;

        friend std::ostream& operator<<(std::ostream& o, const BankSchema& schema)
        {
            o << "[" << schema.key << ", " << schema.description << "]";
            return o;
        }
    };
    using menuElemTree = Tree<BankSchema>;
    using menuElemStackElem = std::pair<BankSchema, menuElemTree::node*>;

    menuElemTree menuTree;
    menuElemTree::node* currentParentMenuElem = nullptr;

    bool parseSchema(std::ifstream&);
    bool parseSchemaLine(std::string& str, BankSchema& lineSchema);
    menuElemTree::node* addMenuElementToTree(std::stack<menuElemStackElem> schemaStack,
                                             const BankSchema& lineSchema);
};

#endif