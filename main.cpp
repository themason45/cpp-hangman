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
    const char *words[5] = {"Hello", "Coffee", "Newspaper", "Animal", "Cheese"};

    std::mt19937 mt(time(nullptr)); // NOLINT(cert-msc51-cpp)
    std::uniform_int_distribution<int> dist(0, sizeof words);
    
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

int main() {
    int maxGuesses = 5;
    int nFailedGuesses = 0;
    int nCorrectGuesses = 0;

    std::string word = getWord();
    int len = (int) word.length();
    char guessedLetters[len];
    for (int i = 0; i < len; ++i) {
        guessedLetters[i] = '/';
    }

    while (true) {
        std::cout << stickManProgressDisplay(&nFailedGuesses) << "\n";

        if (maxGuesses - nFailedGuesses == 1) { std::cout << "You have 1 guess remaining. \n"; }
        else { std::cout << "You have " << maxGuesses - nFailedGuesses << " guesses remaining. \n"; }

        std::cout << "Current progress: " << wordProgressDisplay(guessedLetters, len) << "\n";
        std::cout << "Enter your guess: ";

        std::string guess;
        std::cin >> guess;

        size_t pos = word.find(guess);
        if (pos != std::string::npos) {
            for (int i = 0; i < guess.length(); ++i) guessedLetters[pos + i] = guess[i];
            nCorrectGuesses += (int) guess.length();
        } else {
            std::cout << "Oops, wrong!\n";
            nFailedGuesses++;
        }

        if (nCorrectGuesses == len) {
            nFailedGuesses = 6;
            std::cout << stickManProgressDisplay(&nFailedGuesses) << "\n";
            std::cout << "Well done, you guessed the correct word: " << word << "\n";
            break;
        }
        if (nFailedGuesses == maxGuesses) {
            std::cout << stickManProgressDisplay(&nFailedGuesses) << "\n";
            std::cout << "You failed. Better luck next time :)";
            break;
        }
    }

    return 0;
}
