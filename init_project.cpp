#include <iostream>
#include <filesystem>

#include "CLI11.hpp"


//
// CoLiRu with:
//     g++ init_project.cpp -Os -o init && ./init --help
// 
// Then manually remove these files
//     rm CLI11.hpp init_project.cpp init
//

/**
 * @brief Validator if a string is a valid key for cmake
 * 
 * @note Expanded from https://cliutils.github.io/CLI11/book/chapters/validators.html
 */
struct ValidKeyValidator : public CLI::Validator
{
    ValidKeyValidator()
    {        
        name_ = "VALID_KEY";

        func_ = [](const std::string &str)
        {
            if (!(0 < str.length() && 60 >= str.length())) return std::string("Key must be greater than zero characters and ≤ 60 long");
            
            // Check if the first character is a number
            if (0x30 <= str[0] && 0x39 >= str[0]) return std::string("First character of key cannot be number");
            
            // Check if every character of the string is a number/letter/underscore or dash
            for (auto &&c : str)
            {
                // Check if the character is valid
                if (!(0x30 <= c && c <= 0x39) && !(0x61 <= c && c <= 0x7a) && c != 0x5f && c != 0x2d)
                    return std::string("All characters in key must match pattern of a-z, 0-9, - or _ where the first character is a-z");
            }
            return std::string();
        };
    }
};
const static ValidKeyValidator ValidKey;


/**
 * @brief Called when the program is launched
 * 
 * @param argc Count of command-line arguments
 * @param argv Args, zero is the name of the program
 * @return int An error code
 */
int main(int argc, char *argv[])
{
    // Setup CLI11 stuff
    CLI::App app{"A simple initialisation script", "init_project"};
    
    std::string projName = "default";
    app.add_option("-p,--proj", projName, "The name of the project")
        ->check(ValidKey)
        ->required()
        ->ignore_case();

    std::string execName = "default";
    app.add_option("-e,--exec", execName, "The name of the hello world default executable")
        ->check(ValidKey)
        ->required()
        ->ignore_case();

    ulong projNum = 0;
    app.add_option("-n,--num", projNum, "(optional) Project number, assigned random 6-digit number by default")
        ->check(CLI::Range(0ul,999'999ul))
        ->ignore_case();


    // Init app
    CLI11_PARSE(app, argc, argv);


    // Summarise action
    printf("Initiated project with project name %s and executable name %s\n", projName.c_str(), execName.c_str());
    
    return 0;
}
