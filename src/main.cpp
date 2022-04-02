#include <iostream>

#include <userInput/UserSchema.hpp>
#include <userOperations/UserOps.hpp>

const char* default_schema =
    "/home/rcetin/workspace/repos/bank_management_system/support/options_schema.txt";

void welcome(void)
{
    std::cout << "Welcome to Bank!\n\n";
}

int main(int argc, char const* argv[])
{
    std::string input;
    UserOps operation;
    UserSchema schema{default_schema};

    welcome();

    while(true) {
        schema.dumpCurrentMenu(std::cout);
        std::cout << "\n";

        std::getline(std::cin, input);
        if(std::cin.bad()) {
            std::cerr << "Invalid input is given. Try again...\n";
            continue;
        }

        auto charInput = *(input.data());
        if(!schema.isOptionValid(charInput)) {
            std::cerr << "Invalid option\n\n";
            continue;
        }

        if(!schema.processUserInput(charInput)) {
            std::cerr << "Input could not be processed.\n";
            continue;
        }

        operation.run(schema.getCurrentOperation());
    }

    return 0;
}
