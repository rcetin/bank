#include <iostream>

#include <userInput/UserSchema.hpp>
#include <userOperations/Credentials/Credentials.hpp>
#include <userOperations/UserOps.hpp>

const char* default_schema = "/home/rcetin/workspace/repos/bank/support/options_schema.txt";

void welcome(void)
{
    std::cout << "Welcome to Bank!\n\n";
}

int main(int argc, char const* argv[])
{
    std::string input;
    UserSchema schema{default_schema};
    UserOps operation{std::cin, std::cout, schema};

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
