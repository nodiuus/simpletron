#pragma once
#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory_resource>
#include <string>
#include <type_traits>
#include <vector>
#include <fstream>
#include <sstream>

//fuck

enum Opcodes {
    READ = 10,
    WRITE = 11,
    LOAD = 20,
    STORE = 21,
    ADD = 30,
    SUB = 31,
    DIVIDE = 32,
    MULTIPLY = 33,
    BRANCH = 40,
    BRANCHNEG = 41,
    BRANCHZERO = 42,
    HALT = 43
};

class Simpletron {
public:
    Simpletron() : Memory(MEM_SIZE), Accumulator(0), InstructionRegister(0), ProgramCounter(0) {}
    void Read(std::vector<int> &Memory, int &Operand) {
        std::cout << "Enter an integer: "; 
        std::cin >> Memory[Operand];
    }

    void Write(std::vector<int> &Memory, int &Operand) {
        std::cout << Memory[Operand] << '\n'; 
    }

    void Load(std::vector<int> &Memory, int &Operand) {
        Accumulator = Memory[Operand];
    }

    void Store(std::vector<int> &Memory, int &Operand) {
        Memory[Operand] = Accumulator;
    }

    void Add(std::vector<int> &Memory, int &Operand) {
        Accumulator += Memory[Operand];
    }

    void Subtract(std::vector<int> &Memory, int &Operand) {
        Accumulator -= Memory[Operand];
    }

    void Divide(std::vector<int> &Memory, int &Operand) {
        Accumulator /= Memory[Operand];
    }

    void Multiply(std::vector<int> &Memory, int &Operand) {
        Accumulator *= Memory[Operand];
    }

    void Branch(std::vector<int> &Memory, int &ProgramCounter, int &Operand) {
        ProgramCounter = Operand - 1;
    }

    void BranchNegative(std::vector<int> &Memory, int &ProgramCounter, int &Operand, int &Accumulator) {
       if (Accumulator < 0) {
            ProgramCounter = Operand - 1;
       }
    }

    void BranchZero(std::vector<int> &Memory, int &ProgramCounter, int &Operand, int &Accumulator) {
       if (Accumulator == 0) {
            ProgramCounter = Operand - 1;
       }
    }
    void ResetMachine() {
        Accumulator = 0;
        ProgramCounter = 0;
        InstructionRegister = 0;
        
        for (int i = 0; i < MEM_SIZE; i++) {
            Memory[i] = 0;
        }
    }
    
    void TakeInput() {

    }

    void LoadProgram(std::vector<int> Program) {
        if (Program.size() <= Memory.size()) {
            std::copy(Program.begin(), Program.end(), Memory.begin());
        } else {
            std::cerr << "too large for memory, fuck you" << '\n';
        }
    }

    void GetProgramFromFile(std::string FileName) {
        std::ifstream InputFile(FileName);

        if (!InputFile.is_open()) {
            std::cerr << "failed to open file" << "\n";
        }

        std::vector<char> FileContents(
        (std::istreambuf_iterator<char>(InputFile)),
        (std::istreambuf_iterator<char>()));

        std::vector<int> Integers;
        std::string CurrentInteger; //see its funny because its a string!

        for (char C : FileContents) {
            if (std::isdigit(C) || (C == '-' && CurrentInteger.empty())) {
                CurrentInteger += C;
            } else if (!CurrentInteger.empty()) {
                Integers.push_back(std::stoi(CurrentInteger));
                CurrentInteger.clear();
            }
        }

        if (!CurrentInteger.empty()) {
            Integers.push_back(std::stoi(CurrentInteger));
        }

        std::vector<int> Program;

        for (int Value : Integers) {
            Program.push_back(Value);
        }
        LoadProgram(Program);
        InputFile.close();
    }
    
    void CreateScript() {
        std::cout << "*** Please enter your program one instruction ***" << "\n";
		std::cout << "*** (or data word) at a time. I will type the ***" << "\n";
		std::cout << "*** location number and a question mark (?).  ***" << "\n";
		std::cout << "*** You then type the word for that location. ***" << "\n";
		std::cout << "*** Type the sentinel -99999 to stop entering ***" << "\n";
		std::cout << "*** your program. ***" << "\n";

        std::vector<int> Program;
        int Address = 0;

        while (true) {
            std::cout << Address << "? ";

            int Input;
            std::cin >> Input;

            if (Input == -99999) return;
            Program.push_back(Input);
            Address++;
         
            LoadProgram(Program);
        }
    }

    void ExecuteProgram() {
        while (ProgramCounter < Memory.size()) {
            InstructionRegister = Memory[ProgramCounter];

            int Opcode = InstructionRegister / 100; //extracts opcode from the first two numbers
            int Operand = InstructionRegister % 100; //extracts operand from the last two numbers

            switch (Opcode) {
                case READ:
                    Read(Memory, Operand);
                    break;
                case WRITE:
                    Write(Memory, Operand);
                    break;
                case LOAD:
                    Load(Memory, Operand);
                    break;
                case STORE:
                    Store(Memory, Operand);
                    break;
                case ADD:
                    Add(Memory, Operand);
                    break;
                case SUB:
                    Subtract(Memory, Operand);
                    break;
                case DIVIDE:
                    Divide(Memory, Operand);
                    break;
                case MULTIPLY:
                    Multiply(Memory, Operand);
                    break;
                case BRANCH:
                    Branch(Memory, ProgramCounter, Operand);
                    break;
                case BRANCHNEG:
                    BranchNegative(Memory, ProgramCounter, Operand, Accumulator);
                    break;
                case BRANCHZERO:
                    BranchZero(Memory, ProgramCounter, Operand, Accumulator);
                    break;
                case HALT:
                    std::cout << "HALTED" << '\n';
                    return;
                default:
                    std::cout << "invalid opcode, fuck you " << Opcode << '\n';
                    return;
            }
            ProgramCounter++;

            if (ProgramCounter >= Memory.size()) {
                std::cout << "Program counter out of bounds, terminating program." << '\n';
                 break;
            }
        }
    };
private:
    static const int MEM_SIZE = 1000;
    std::vector<int> Memory;
    int ProgramCounter;
    int Accumulator;
    int InstructionRegister;
};