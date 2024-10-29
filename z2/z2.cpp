#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    const char* filePath = "C:\\Users\\banii\\source\\repos\\z2\\z2\\test.txt";

    std::cout << "Attempting to open file at: " << filePath << std::endl;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file. Make sure the file exists and the path is correct." << std::endl;
        return 1;
    }

    std::cout << "File opened successfully." << std::endl;


    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
        std::cout << "Read line: " << line << std::endl;  
    }
    file.close();


    if (lines.empty()) {
        std::cerr << "File is empty or could not read any lines." << std::endl;
        return 1;
    }


    std::cout << "Switching to new screen buffer..." << std::endl;

    HANDLE hNewBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    if (hNewBuffer == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating console screen buffer" << std::endl;
        return 1;
    }

    if (!SetConsoleActiveScreenBuffer(hNewBuffer)) {
        std::cerr << "Error setting active screen buffer" << std::endl;
        CloseHandle(hNewBuffer);
        return 1;
    }

    DWORD written;
    COORD coord = { 0, 0 }; 

    for (const auto& currentLine : lines) {
        WriteConsoleOutputCharacter(hNewBuffer, currentLine.c_str(), currentLine.length(), coord, &written);
        coord.Y++;  
        std::cout << "Outputting to new buffer: " << currentLine << std::endl;
    }

    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    std::cout << "Back to the original screen buffer." << std::endl;

    CloseHandle(hNewBuffer);

    return 0;
}
