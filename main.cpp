#include <iostream>
#include "random"

/*
 * To build for x86_64, use: `cmake -DCMAKE_OSX_ARCHITECTURES=x86_64 -B build .`
 * Then, to generate the executable: `cd build && make`
 * To run: ./hangman
 */

const char matrices[7][6][8] = {
        {
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ',  ' ', ' ', ' ',  ' ', ' '},
                {' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' '},
                {'/', '\\', ' ', ' ', ' ', ' ',  ' ', ' '},
        },
        {
                {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'|', ' ', ' ',  ' ', ' ', ' ',  ' ', ' '},
                {'|', ' ', ' ', ' ',  ' ', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', ' ', ' ',  ' ', ' '},
                {'/', '\\', ' ', ' ', ' ', ' ',  ' ', ' '},
        },
        {
                {'|', '-', '-', '-', '-', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'|', ' ', ' ',  ' ', ' ', ' ',  ' ', ' '},
                {'|', ' ', ' ', ' ',  ' ', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', ' ', ' ',  ' ', ' '},
                {'/', '\\', ' ', ' ', ' ', ' ',  ' ', ' '},
        },
        {
                {'|', '-', '-', '-', '-', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', '0', ' ', ' ', ' '},
                {'|', ' ', ' ',  '/', '|', '\\', ' ', ' '},
                {'|', ' ', ' ', ' ',  ' ', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', ' ', ' ',  ' ', ' '},
                {'/', '\\', ' ', ' ', ' ', ' ',  ' ', ' '},
        },
        {
                {'|', '-', '-', '-', '-', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', '0', ' ', ' ', ' '},
                {'|', ' ', ' ',  '/', '|', '\\', ' ', ' '},
                {'|', ' ', ' ', ' ',  '|', ' ', ' ', ' '},
                {'|', ' ', ' ', '/', ' ', '\\', ' ', ' '},
                {'/', '\\', ' ', ' ', ' ', ' ',  ' ', ' '},
        },
        {
                {'|', '-', '-', '-', '-', ' ', ' ', ' '},
                {'|', ' ', ' ', ' ', 'X', ' ', ' ', ' '},
                {'|', ' ', ' ',  '/', '|', '\\', ' ', ' '},
                {'|', ' ', ' ', ' ',  '|', ' ', ' ', ' '},
                {'|', ' ', ' ', '/', ' ', '\\', ' ', ' '},
                {'/', '\\', ' ', ' ', ' ', ' ',  ' ', ' '},
        },
        {
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', '\\', ' ', '0', ' ',  '/', ' '},
                {' ', ' ', ' ', '\\', '|', '/', ' ', ' '},
                {' ', ' ', ' ', ' ', '|', ' ',  ' ', ' '},
                {' ', ' ',  ' ', '/', ' ', '\\', ' ', ' '},
        }
};

std::string getWord() {
    const int wordCount = 10;
    const char *words[wordCount] = {"Hello", "Coffee", "Newspaper", "Animal", "Cheese", "Tree", "Window", "Glass",
                                    "Breadboard", "Sink"};

    std::mt19937 mt(time(nullptr)); // NOLINT(cert-msc51-cpp)
    std::uniform_int_distribution<int> dist(0, wordCount);

    int idx = dist(mt);

    std::string word = reinterpret_cast<const char *>(words[idx]);
    return word;
}

std::string stickManProgressDisplay(const int *nFailedGuesses) {
    int n = *nFailedGuesses;
    std::string drawing;
    for (int i = 0; i < 6; ++i) {
        std::string row;
        for (int j = 0; j < 8; ++j) {
            row += matrices[n][i][j];
        }
        drawing += row + "\n";
    }
    return drawing;
}

std::string wordProgressDisplay(char *currentLetters, int nLetters) {
    std::string display;
    for (int i = 0; i < nLetters; ++i) {
        if (currentLetters[i] == '/') { display += "_"; }
        else { display += currentLetters[i]; }
    }
    return display;
}

bool valExists(const char arr[], int len, const std::string &val) {
    bool passes = false;
    for (char i : val) {
        if (i == '/') continue;
        if (std::find(arr, arr + len, i) != arr + len) passes = true;
    }
    return passes;
}

int main() {
    // Set the globals
    const int maxGuesses = 5;
    int nFailedGuesses = 0;
    int nCorrectGuesses = 0;

    // Select a random word
    std::string word = getWord();
    int len = (int) word.length();

    // Then create an array of the letters we have found so far ('/' is used as this won't show up in any of the words)
    char guessedLetters[len];
    for (int i = 0; i < len; ++i) {
        guessedLetters[i] = '/';
    }

    // Start the main process loop
    while (true) {
        // Breakout cases at the start
        if (nCorrectGuesses == len) {
            nFailedGuesses = 6;
            std::cout << stickManProgressDisplay(&nFailedGuesses) << "\n";
            std::cout << "Well done, you guessed the correct word: " << word << "\n";
            break;
        }
        if (nFailedGuesses == maxGuesses) {
            std::cout << stickManProgressDisplay(&nFailedGuesses) << "\n";
            std::cout << "You failed. Better luck next time :).\n The word was: " << word;
            break;
        }

        // Main display stuff
        std::cout << stickManProgressDisplay(&nFailedGuesses) << "\n";

        if (maxGuesses - nFailedGuesses == 1) { std::cout << "You have 1 guess remaining. \n"; }
        else { std::cout << "You have " << maxGuesses - nFailedGuesses << " guesses remaining. \n"; }

        std::cout << "Current progress: " << wordProgressDisplay(guessedLetters, len) << "\n";
        std::cout << "Enter your guess: ";

        // Take the guess
        std::string guess;
        std::cin >> guess;

        // Check the letters of the guess haven't been used already
        if (!valExists(guessedLetters, len, guess)) {

            // For each letter within the guess, perform a check on it
            for (char letter : guess) {
                int rootPos = -1;
                // Search through the whole length of the word to find a match
                while (rootPos <= len) {
                    size_t pos = word.find(letter, rootPos + 1);
                    // Check that we're still at the root position, and if there are no matches, then break
                    if (pos == std::string::npos && rootPos == -1) {
                        std::cout << "Oops, wrong!\n";
                        nFailedGuesses++;
                        break;
                    }

                    if (pos != std::string::npos) {
                        // When you find a match, set the base search pointer to that spot, to find repeated letters
                        rootPos = (int) pos;
                        guessedLetters[pos] = letter;
                        nCorrectGuesses++;
                        continue;
                    }

                    break;
                }
            }
        }
    }

    return 0;
}
