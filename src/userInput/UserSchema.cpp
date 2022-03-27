#include <algorithm>
#include <array>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
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

bool UserSchema::parseSchema(std::ifstream& stream)
{
    // Format: charInput, strijngInput, stringInput

    std::string line;

    while(!stream.eof()) {
        std::getline(stream, line);
        readSchemaLine(line);
        // std::cout << "Read line: " << line << "\n";
    }
}

bool UserSchema::readSchemaLine(const std::string& str)
{
    std::stringstream stream{str};
    std::array<char, 100> arr;
    size_t menuLevel;
    std::cout << "Enter: " << str << '\n';

    menuLevel = std::count(str.begin(), str.end(), '-');
    std::cout << "Menu Level: " << menuLevel << '\n';

    while(!stream.eof()) {
        stream.getline(arr.data(), str.length(), ',');
        std::cout << "data: " << arr.data() << '\n';
    }
}