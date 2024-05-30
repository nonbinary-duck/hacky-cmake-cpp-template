#include <filesystem>
#include <iostream>
#include <random>
#include <regex>

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
            if (0x61 <= str[0] && 0x7a >= str[0]) return std::string("First character of key must be [a-z]");
            
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
    CLI::App app{"A simple project initialisation script, finalises by removing this binary and its source", "init_project"};
    
    std::string projName = "default";
    app.add_option("project_name", projName, "The name of the project")
        ->check(ValidKey)
        ->required()
        ->ignore_case();

    std::string execName = "default";
    app.add_option("exec_name", execName, "The name of the hello world default executable")
        ->check(ValidKey)
        ->required()
        ->ignore_case();

    ulong projNum  = -1;
    app.add_option("-n,--num", projNum, "Project number, assigned ND random 6-digit number by default")
        ->check(CLI::Range(0ul,999'999ul))
        ->ignore_case();
    
    bool skipInput = false;
    app.add_flag("-y,--no-prompt", skipInput, "Use to accept changes to CWD without prompt")
        ->ignore_case();

    bool dryRun    = false;
    app.add_flag("-d,--dry-run", dryRun, "Perform a dry-run execution without making changes")
        ->ignore_case();

    bool selfDestruct = true;
    app.add_flag("!-q,!--self-destruct", selfDestruct, "Do not remove this binary and its source after operation")
        ->ignore_case()->default_val(true);


    // Init app
    CLI11_PARSE(app, argc, argv);

    if (projNum == -1UL)
    {
        // Use a non-deterministic random value for the project ID if no ID was manually specified
        std::random_device rd;
        projNum = rd() % 1'000'000;
    }
    

    // Convert the project number into a 6-digit string
    std::string projNumStr("000000");
    for (size_t i = 0; i < projNumStr.length(); i++)
    {
        projNumStr[i] = std::to_string(( (ulong)( projNum / std::pow(10, 5-i) ) ) % 10)[0];
    }
    

    // By this point we have all infos
    // Now go through CWD and move directories or files that match pattern
    auto cwd = std::filesystem::current_path();

    // Quickly ask for manual input

    

    // Summarise action
    printf("Initiated project with project id %s name %s and executable name %s\n", projNumStr.c_str(), projName.c_str(), execName.c_str());
    
    return 0;
}
