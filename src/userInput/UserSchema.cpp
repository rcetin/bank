#include <algorithm>
#include <array>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#include <UserSchema.hpp>

UserSchema::UserSchema(const std::string& filename)
{
    std::ifstream schemaFile{filename};

    if(!schemaFile.is_open()) {
        throw SchemaError{"Schema File could not be opened..."};
    }

    if(!parseSchema(schemaFile)) {
        throw SchemaError{"Schema format error"};
    }
}

bool UserSchema::isOptionValid(char) { }
std::string UserSchema::getOptionDescription(char) { }
std::string UserSchema::getCurrentOperation(void) { }
std::string UserSchema::listCurrentMenu() { }

UserSchema::menuElemTree::node*
UserSchema::addMenuElementToTree(std::stack<menuElemStackElem> schemaStack,
                                 const BankSchema& lineSchema)
{
    menuElemTree::node* parent = nullptr;
    while(!schemaStack.empty()) {
        const menuElemStackElem& sc = schemaStack.top();
        if(sc.first.menuLevel == (lineSchema.menuLevel - 1)) {
            parent = sc.second;
            break;
        }
        schemaStack.pop();
    }
    return (parent) ? menuTree.addChild(parent, lineSchema) : nullptr;
}

bool UserSchema::parseSchema(std::ifstream& stream)
{
    // Format: charInput, strijngInput, stringInput

    std::string line;
    BankSchema lineSchema;
    std::stack<menuElemStackElem> schemaStack;
    menuElemTree::node* node;
    bool ret;

    BankSchema r{"Menu", "Initial Menu", "noOp", -1};
    node = menuTree.addRoot(r);
    schemaStack.push(menuElemStackElem{r, node});

    while(!stream.eof()) {
        // TODO: Implement stack to add children to correct parents!
        std::getline(stream, line);
        ret = parseSchemaLine(line, lineSchema);
        if(!ret) {
            return false;
        }

        node = addMenuElementToTree(schemaStack, lineSchema);
        if(node == nullptr) {
            return false;
        }
        schemaStack.push(menuElemStackElem{lineSchema, node});
    }
    menuTree.dump();
    return true;
}

bool UserSchema::parseSchemaLine(std::string& str, BankSchema& lineSchema)
{
    std::string lineElem;

    const int32_t commaCount = std::count(str.begin(), str.end(), ',');
    if(commaCount != 2) {
        std::cout << "Comma count " << commaCount << "\n";
        return false;
    }

    lineSchema.menuLevel = std::count(str.begin(), str.end(), '-');
    str.erase(0, lineSchema.menuLevel); // exclude '-'
    std::stringstream stream{str};
    std::getline(stream, lineSchema.name, ',');
    std::getline(stream, lineSchema.description, ',');
    std::getline(stream, lineSchema.opName, ',');

    return true;
}