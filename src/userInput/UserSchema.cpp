#include <algorithm>
#include <array>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

#include <userInput/CharInput.hpp>
#include <userInput/UserSchema.hpp>

UserSchema::UserSchema(const std::string& filename)
{
    std::ifstream schemaFile{filename};

    if(!schemaFile.is_open()) {
        throw SchemaError{"Schema File could not be opened..."};
    }

    if(!parseSchema(schemaFile)) {
        throw SchemaError{"Schema format error"};
    }

    currentParentMenuElem = menuTree.getRoot();
}

bool UserSchema::isOptionValid(char userOption)
{
    CharInput charI{userOption};

    if(!currentParentMenuElem) {
        return false;
    }

    if(!charI.isValid()) {
        return false;
    }

    auto it = menuTree.getIteratorToChild(currentParentMenuElem);
    while(it.get()) {
        if(it->data().key == charI) {
            return true;
        }
        it++;
    }

    return false;
}

bool UserSchema::processUserInput(char userOption)
{
    CharInput charI{userOption};
    if(!isOptionValid(userOption)) {
        return false;
    }

    auto it = menuTree.getIteratorToChild(currentParentMenuElem);
    while(it.get()) {
        if(it->data().key == charI) {
            currentParentMenuElem = it.get();
            return true;
        }
        it++;
    }

    return false;
}

std::string UserSchema::getCurrentOptionDescription(void)
{
    return currentParentMenuElem->data().description.data();
}

std::string UserSchema::getCurrentOperation(void)
{
    return currentParentMenuElem->data().operation.data();
}

void UserSchema::dumpCurrentMenu(std::ostream& out)
{
    out << "[key] [Description]\n";
    out << "-----------------------------\n";
    out << "[" << currentParentMenuElem->data().key << "] ["
        << currentParentMenuElem->data().description << "]\n";

    auto it = menuTree.getIteratorToChild(currentParentMenuElem);
    while(it.get()) {
        out << "\t[" << it->data().key << "] [" << it->data().description << "]\n";
        it++;
    }
}

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

    BankSchema r{"", "Initial Menu", "noOp", -1};
    node = menuTree.addRoot(r);
    schemaStack.push(menuElemStackElem{r, node});

    while(!stream.eof()) {
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
    std::string key;
    std::string description;
    std::string operation;

    const int32_t commaCount = std::count(str.begin(), str.end(), ',');
    if(commaCount != 2) {
        return false;
    }

    lineSchema.menuLevel = std::count(str.begin(), str.end(), '-');
    str.erase(0, lineSchema.menuLevel); // exclude '-'
    std::stringstream stream{str};
    std::getline(stream, key, ',');
    std::getline(stream, description, ',');
    std::getline(stream, operation, ',');

    lineSchema.key = key;
    lineSchema.description = description;
    lineSchema.operation = operation;

    if(!lineSchema.key.isValid() || !lineSchema.description.isValid() ||
       !lineSchema.operation.isValid()) {
        return false;
    }

    return true;
}