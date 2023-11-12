#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "Simpletron.h"

namespace filesystem = std::filesystem;

void PrintDirectoryContents(std::string &Path) {
    for (const auto& Entry : filesystem::directory_iterator(Path)) {
        std::cout << Entry.path().filename() << "\n";
    }
}

int main() {
    Simpletron Computer;
    
   	std::cout << "          *** Welcome to Simpletron! ***         " << "\n";

    std::cout << "Select Mode" << "\n";
    std::cout << "(1) Load From File" << "\n";
    std::cout << "(2) Create and Run Script" << "\n";

    int Mode;
    std::cin >> Mode;

    std::string Path = "Programs/";
    std::string File;

    switch (Mode) {
        case 1:
            std::cout << "Choose a file to run (case sensitive)" << "\n";

            try {
                if (filesystem::exists(Path) && filesystem::is_directory(Path)) {
                    PrintDirectoryContents(Path);
                } else {
                    std::cout << "NOT FOUND" << "\n";
                }
            } catch (const std::exception &e){
                std::cout << e.what() << "\n";
            }

            std::cin >> File;
            Computer.GetProgramFromFile(Path + File);
            Computer.ExecuteProgram();
            break;
        case 2:

            Computer.CreateScript();
            Computer.ExecuteProgram();
            break;
        default:
            std::cout << "Invalid choice, try again" << "\n";
            main(); //LOL
            break;
    }

    return 0;
}