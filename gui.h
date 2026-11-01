#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>

// A very basic console UI namespace.
namespace ConsoleGUI {

    // Clears the terminal screen.
    void clearScreen();

    // Prints a simple horizontal line.
    void printSeparator(int width = 40, char symbol = '-');

    // Prints a centred title.
    void printTitle(const std::string& title, int width = 40);

    // Displays a menu and returns the user's choice (1-based index).
    // Returns 0 if the user chooses to exit.
    int showMenu(const std::string& title, const std::vector<std::string>& options);

    // Pauses execution until Enter is pressed.
    void waitForEnter();

    // Prints an error message to the console.
    void printError(const std::string& message);

    // Prints a success message to the console.
    void printSuccess(const std::string& message);
}

#endif // GUI_H