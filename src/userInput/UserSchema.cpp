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

    parseSchema(schemaFile);
}

bool UserSchema::searchOption(char) { }
std::string UserSchema::getDescription(char) { }
std::string UserSchema::getCurrentOp(void) { }
std::string UserSchema::listSubmenuElements() { }

UserSchema::menuElemTree::node* UserSchema::addMenuElementToTree(menuElemTree::node* parent,
                                                                 const BankSchema& lineSchema)
{
    return menuTree.addChild(parent, lineSchema);
}

bool UserSchema::parseSchema(std::ifstream& stream)
{
    // Format: charInput, strijngInput, stringInput

    std::string line;
    BankSchema lineSchema;
    int32_t menuLevel;
    menuElemTree::node* root;
    menuElemTree::node* node;
    int32_t nodeMenuLevel;
    bool ret;

    root = menuTree.addRoot({"Menu", "Initial Menu", "noOp"});

    node = root;
    nodeMenuLevel = -1;
    while(!stream.eof()) {
        std::getline(stream, line);
        ret = readSchemaLine(line, lineSchema, menuLevel);
        if(!ret) {
            return false;
        }

        auto parent =
            (!menuLevel) ? root : ((menuLevel > nodeMenuLevel) ? node : menuTree.getParent(node));
        if(parent == nullptr) {
            return false;
        }

        node = addMenuElementToTree(parent, lineSchema);
        if(node == nullptr) {
            return false;
        }
        nodeMenuLevel = menuLevel;

        menuTree.dump();
    }
}

bool UserSchema::readSchemaLine(std::string& str, BankSchema& lineSchema, int32_t& menuLevel)
{
    std::string lineElem;
    std::cout << "Enter: " << str << '\n';

    menuLevel = std::count(str.begin(), str.end(), '-');
    std::cout << "Menu Level: " << menuLevel << '\n';

    str.erase(0, menuLevel); // exclude '-'
    std::cout << "Line - excluded: " << str << '\n';

    std::stringstream stream{str};
    std::getline(stream, lineSchema.name, ',');
    std::cout << "lineSchema.name: " << lineSchema.name << '\n';

    std::getline(stream, lineSchema.description, ',');
    std::cout << "lineSchema.description: " << lineSchema.description << '\n';

    std::getline(stream, lineSchema.opName, ',');
    std::cout << "lineSchema.opName: " << lineSchema.opName << '\n';

    return true;
}